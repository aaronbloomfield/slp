// this needs two pacakges to work: sqlite3 libsqlite3-dev

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>

using namespace std;

stringstream wsgifile;

/** This program will take, as a command-line parameter, a wsgi file
 * (such as Django's wsgi.py), and either add it to, or remove it
 * from, a sqlite db.  The django.conf for apache is then
 * re-generated, checking each existing wsgi file to make sure it's
 * still there.
 */

/** DB schema:
 *
 * id int auto_increment primary key
 * uid int
 * wsgi text
 * valid boolean
 * added datetime
 * removed datetime
 *
 * create table wsgi(id integer primary key asc, uid int, wsgi text, valid boolean, added datetime, removed datetime);
 */

void printUsage(char *argv0, bool doexit = true) {
    cerr << "Usage:  " << argv0 << " -register <wsgi_file>\n"
	 << "\t" << argv0 << " -remove <num>\n"
	 << "\t" << argv0 << " -list\n"
	 << "\t" << argv0 << " -regenrate" << endl;
    if ( doexit )
      exit(0);
}

int get_UID_for_file (char* filename);
char* get_userid_by_uid (int uid);

// this function adapted 
// from http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
inline bool file_exists (const std::string& name) {
    ifstream f(name.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }   
}

// this function from http://www.sqlite.org/quickstart.html
static int list_callback(void *NotUsed, int argc, char **argv, char **azColName) {
  for( int i = 0; i < argc; i++ ) {
    cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL");
    if ( !strcmp(azColName[i],"uid") ) {
      int uid;
      sscanf(argv[i],"%d",&uid);
      cout << " (" << get_userid_by_uid(uid) << ")";
    }
    cout << "\t";
  }
  cout << endl;
  return 0;
}

static int regenerate_callback(void *NotUsed, int argc, char **argv, char **azColName){
  int uid;
  sscanf(argv[1],"%d",&uid);
  char *filename = argv[2];
  string userid(get_userid_by_uid(uid));
  string fullpath(realpath(filename,NULL));
  int pos = fullpath.rfind("/");
  string up1 = fullpath.substr(0,pos);
  string basename = fullpath.substr(pos+1);
  pos = up1.rfind("/");
  string up2 = up1.substr(0,pos);
  wsgifile << "WSGIScriptAlias /django/" << userid << " " << fullpath << "\n"
	   << "WSGIDaemonProcess " << userid << " python-path=" << up2 << "\n"
	   << "WSGIProcessGroup " << userid << "\n"
	   << "<Directory " << up1 << "\n"
	   << "  <Files " << basename << ">\n"
	   << "    Require all granted\n"
	   << "  </Files>\n"
	   << "</Directory>\n\n";
  return 0;
}


int main(int argc, char **argv) {
  enum { MODE_NONE, MODE_REGISTER, MODE_REMOVE, MODE_REGENERATE, MODE_LIST } mode = MODE_NONE;
  char *filename = NULL;
  int uid = getuid();

  // parse command line parameters, get wsgi file and mode
  if ( (argc == 1) || (argc >= 4) )
    printUsage(argv[0]);
  string param(argv[1]);
  if ( (argc == 3) && (param == "-register") )
    mode = MODE_REGISTER;
  else if ( (argc == 3) && (param == "-remove") )
    mode = MODE_REMOVE;
  else if ( (argc == 2) && (param == "-regenerate") )
    mode = MODE_REGENERATE;
  else if ( (argc == 2) && (param == "-list") )
    mode = MODE_LIST;
  else
    printUsage(argv[0]);

  // if we are registering a wsgi file, do basic sanity checks
  if ( mode == MODE_REGISTER ) {
    // does the file exist?
    if ( !file_exists(string(argv[2])) ) {
      cerr << "File '" << argv[2] << "' does not exist!" << endl;
      exit(0);
    }
    // get the file type
    stringstream cmd;
    cmd << "/usr/bin/file " << argv[2];
    FILE *fp = popen(cmd.str().c_str(),"r");
    if ( fp == NULL ) {
      cerr << "Failed to run 'file' command" << endl;
      exit(0);
    }
    char result[1024];
    fgets(result, sizeof(result)-1, fp);
    pclose(fp);
    // is it a Python file?
    stringstream expected;
    expected << argv[2] << ": Python script, ASCII text executable\n";
    string obtained(result);
    if ( obtained != expected.str() ) {
      cerr << "Not a valid WSGI file" << endl;
      exit(0);
    }
    // is it owned by the user executing this script?
    int fuid = get_UID_for_file (argv[2]);
    if ( (uid != 0) && (uid != 1000) && (fuid != uid) ) {
      cerr << "You cannot register a file that you do not own!" << endl;
      exit(0);
    }
  }

  // open the DB
  sqlite3 *db;
  char *errmsg = NULL;
  int rc = sqlite3_open("wsgi.db",&db);
  if ( rc ) {
    cerr << "Can't open the DB" << endl;
    sqlite3_close(db);
    exit(0);
  }

  // register a new entry
  if ( mode == MODE_REGISTER ) {
    // error checks happened above
    stringstream query;
    query << "insert into wsgi values (null," << uid << ",\"" << realpath(argv[2],NULL) 
	  << "\",1,datetime(),null)";
    int ret = sqlite3_exec(db, query.str().c_str(), NULL, NULL, &errmsg);
    if ( ret != SQLITE_OK ) {
      cerr << "SQL error: " << errmsg << endl;
      sqlite3_free(errmsg);
      exit(0);
    }
  }

  // list entries in the DB
  if ( mode == MODE_LIST ) {
    // TODO: if uid 0 or 1000, then list all; otherwise, list for that user
    int ret = sqlite3_exec(db,"select * from wsgi", list_callback, 0, &errmsg);
    if ( ret != SQLITE_OK ) {
      cerr << "SQL error: " << errmsg << endl;
      sqlite3_free(errmsg);
      exit(0);
    }
  }

  // remove an entry from the DB
  if ( mode == MODE_REMOVE ) {
    // TODO: get entry requested (after checking that it's a valid int)
    // TODO: uid 0 or 1000 can remove any; otherwise, check that the uid matches
    // TODO: remove entry
  }

  // all of these perform the generation and apache reload
  int ret = sqlite3_exec(db,"select * from wsgi where valid=1", regenerate_callback, 0, &errmsg);
  if ( ret != SQLITE_OK ) {
    cerr << "SQL error: " << errmsg << endl;
    sqlite3_free(errmsg);
    exit(0);
  }

  // TODO: write wsgifile to file
  cout << wsgifile.str() << endl;

  // TODO: reload apache

  // - add or remove it to/from the sql db
  // - generate the apache django.conf file (go through each wsgi file 
  //   in the db, and if it exists, add it to the django.conf file)
  // - install that file in the right place, and reload apache
  return 0;
}



int get_UID_for_file (char* filename) {
  struct stat buf;
  int uid = stat(filename,&buf);
  return buf.st_uid;
}

char* get_userid_by_uid (int uid) {
  struct passwd *p = getpwuid(uid);
  if ( p == NULL ) {
    cerr << "Unknown user with uid " << uid << endl;
    exit(0);
  }
  return p->pw_name;
}

    

/**
 * wsgi-admin, by Aaron Bloomfield, (c) 2014
 *
 * This is part of the SLP repository
 * (https://github.com/aaronbloomfield/slp), and is released under a
 * CC BY-SA license, like the rest of the repository.
 *
 * This program will help manage the registration and de-registration
 * of wsgi files.  It needs two Ubuntu pacakges to work: sqlite3 and
 * libsqlite3-dev.
 *
 * Full directions for how to use it can be found in the
 * django-getting-started.md (and .html) files in the docs/ directory
 * in the SLP repo.
 *
 * Installation:
 * - Compile with 'make', put it somewhere (such as /usr/local/bin)
 * - You will need this to be in a group that can write to the two
 *   conf files declared in the #defines, below
 * - Run 'chmod 2755 wsgi-admin': this will set the group ID bit when
 *   the program runs
 * - the django.conf and django-ssl.conf files need to be in that
 *   group, and group-writable
 * - the DB file also needs to be in that group and group-writable
 *
 * This program will take, as a command-line parameter, a wsgi file
 * (such as Django's wsgi.py), and either add it to, or remove it
 * from, a sqlite db.  The django.conf for apache is then
 * re-generated, checking each existing wsgi file to make sure it's
 *
 * The callback functions were adapted from
 * http://www.sqlite.org/quickstart.html
 *
 * DB schema:
 *
 * id int auto_increment primary key
 * uid int
 * wsgi text
 * valid boolean
 * added datetime
 * removed datetime
 * app text
 *
 * The line to create this in sqlite3:
 * create table wsgi(id integer primary key asc, uid int, wsgi text, valid boolean, added datetime, removed datetime, app text);
 */

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

// constants to change as per your system configuration
#define DJANGO_CONF_FILE "/etc/apache2/django.conf"
#define DJANGO_SSL_CONF_FILE "/etc/apache2/django-ssl.conf"
#define APACHE2_RELOAD_CMD "/usr/local/bin/reload-apache2"
#define URL_PREFIX "/django"
#define DATABASE_FILE "/etc/apache2/django.db"
#define DEFAULT_APP_NAME "polls"

// some global variables
stringstream wsgifile, wsgisslfile, query;
int count = 0, find_uid = 0;
string find_filename = "";
string appname = string(DEFAULT_APP_NAME);

// for when they invoke it incorrectly...
void printUsage(char *argv0, bool doexit = true) {
    cerr << "Usage:  " << argv0 << " -register <wsgi_file> [<app_name>]\n"
	 << "\t" << argv0 << " -remove <num>\n"
	 << "\t" << argv0 << " -list\n"
	 << "\t" << argv0 << " -regenrate" << endl;
    if ( doexit )
      exit(0);
}

// prints an error message with the query, deallocates that error message, and exits
void sqlite3_die(char *errmsg, string query) {
  cerr << "SQL error: " << errmsg << " on query: " << query << endl;
  sqlite3_free(errmsg);
  exit(0);
}

// prints the error message and exits
void die(string s) {
  cerr << "Error: " << s << endl;
  exit(0);
}

// finds who owns a file
int get_UID_for_file (char* filename) {
  struct stat buf;
  int uid = stat(filename,&buf);
  return buf.st_uid;
}

// maps a UID to a Unix user name
char* get_userid_by_uid (int uid) {
  struct passwd *p = getpwuid(uid);
  if ( p == NULL ) {
    cerr << "Unknown user with uid " << uid << endl;
    exit(0);
  }
  return p->pw_name;
}
    
// checks if a file exists; this function adapted from
//http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
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

// checks if the passed file is a valid WSGI file (really if it's a Python script)
bool is_valid_wsgi_file(char *filename) {
  // get the file type
  stringstream cmd;
  cmd << "/usr/bin/file " << filename;
  FILE *fp = popen(cmd.str().c_str(),"r");
  if ( fp == NULL )
    die ("Failed to run 'file' command");
  char result[1024];
  fgets(result, sizeof(result)-1, fp);
  pclose(fp);
  // check that the output was correct
  stringstream expected;
  expected << filename << ": Python script, ASCII text executable\n";
  string obtained(result);
  return ( obtained == expected.str() );
}

// how to handle the data returned when displaying the list of the DB entries
static int list_callback(void *NotUsed, int argc, char **argv, char **azColName) {
  for( int i = 0; i < argc; i++ ) {
    if ( strcmp(azColName[i],"id") )
      cout << "\t";
    cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL");
    if ( !strcmp(azColName[i],"uid") ) {
      int uid;
      sscanf(argv[i],"%d",&uid);
      cout << " (" << get_userid_by_uid(uid) << ")";
    }
    cout << endl;
  }
  cout << endl;
  return 0;
}

// gets the count(*) field and saves it in a global variable
static int count_callback(void *NotUsed, int argc, char **argv, char **azColName) {
  if ( argc != 1 )
    die("count_callback returned multiple columns!");
  if ( strcmp(azColName[0],"count(*)") )
    die("count_callback not provided a count(*) column");
  sscanf(argv[0],"%d",&count);
  return 0;
}

// this callback allows searching for the 'wsgi' and 'uid' fields in the DB
static int find_callback(void *NotUsed, int argc, char **argv, char **azColName) {
  if ( argc < 2 )
    die("invalid result set to find_callback (1)");
  if ( strcmp(azColName[2],"wsgi") )
    die("invalid result set to find_callback (2)");
  find_filename = string(argv[2]);
  if ( strcmp(azColName[1],"uid") )
    die("invalid result set to find_callback (3)");
  sscanf(argv[1],"%d",&find_uid);
  return 0;
}

// this regenerates the django.conf files
static int regenerate_callback(void *NotUsed, int argc, char **argv, char **azColName){
  int uid;
  sscanf(argv[1],"%d",&uid);
  char *filename = argv[2];
  if ( !file_exists(argv[2]) )
    return 0;
  if ( !is_valid_wsgi_file(argv[2]) )
    return 0;
  string userid(get_userid_by_uid(uid));
  string fullpath(realpath(filename,NULL));
  int pos = fullpath.rfind("/");
  string up1 = fullpath.substr(0,pos);
  string basename = fullpath.substr(pos+1);
  pos = up1.rfind("/");
  string up2 = up1.substr(0,pos);
  stringstream foo;
  //foo << "Alias " << URL_PREFIX << "/" << userid << "/static " << up2 << "/" << appname << "/static\n"
  //<< "<Directory " << up2 << "/" << appname << "/static>\n"
  //<< "  Require all granted\n"
  //<< "</Directory>\n";
  foo << "WSGIScriptAlias " << URL_PREFIX << "/" << userid << " " << fullpath << "\n";
  wsgifile << foo.str();
  wsgisslfile << foo.str();
  wsgifile << "WSGIDaemonProcess " << userid << " python-path=" << up2 << "\n"; // not in the ssl version
  foo.str("");
  foo << "<Location " << URL_PREFIX << "/" << userid << ">\n"
      << "  WSGIProcessGroup " << userid << "\n"
      << "</Location>\n"
      << "<Directory " << up1 << ">\n"
      << "  <Files " << basename << ">\n"
      << "    Require all granted\n"
      << "  </Files>\n"
      << "</Directory>\n\n";
  wsgifile << foo.str();
  wsgisslfile << foo.str();
  return 0;
}

// what does the main() say?
int main(int argc, char **argv) {

  enum { MODE_NONE, MODE_REGISTER, MODE_REMOVE, MODE_REGENERATE, MODE_LIST } mode = MODE_NONE;
  char *filename = NULL;
  int uid = getuid(), ret;

  // parse command line parameters, get wsgi file and mode
  if ( (argc == 1) || (argc > 4) )
    printUsage(argv[0]);
  string param(argv[1]);
  if ( ((argc == 3) || (argc == 4)) && (param == "-register") )
    mode = MODE_REGISTER;
  else if ( (argc == 3) && (param == "-remove") )
    mode = MODE_REMOVE;
  else if ( (argc == 2) && (param == "-regenerate") )
    mode = MODE_REGENERATE;
  else if ( (argc == 2) && (param == "-list") )
    mode = MODE_LIST;
  else
    printUsage(argv[0]);

  // open the DB
  sqlite3 *db;
  char *errmsg = NULL;
  if ( !file_exists(DATABASE_FILE) )
    die("database file does not exist");
  ret = sqlite3_open(DATABASE_FILE,&db);
  if ( ret != SQLITE_OK )
    die("Can't open the DB");

  // register a new entry
  if ( mode == MODE_REGISTER ) {

    // sanity check the file name
    for ( int i = 0; i < strlen(argv[2]); i++ )
      if ( (argv[2][i] == '\\') || (argv[2][i] == '"') || (argv[2][i] == '\'') || (argv[2][i] == ';') )
	die ("Try choosing a file name that does *not* lend itself to SQL injection attacks");

    // does the file exist?
    if ( !file_exists(string(argv[2])) ) {
      cerr << "File '" << argv[2] << "' does not exist!" << endl;
      exit(0);
    }

    // is it a Python file?
    if ( !is_valid_wsgi_file(argv[2]) )
      die("Not a valid WSGI file");

    // is it owned by the user executing this script?
    int fuid = get_UID_for_file (argv[2]);
    if ( (uid != 0) && (uid != 1000) && (fuid != uid) )
      die ("You cannot register a file that you do not own");

    // check for duplicate entries of that file
    query.str("");
    query << "select count(*) from wsgi where wsgi=\""
	  << realpath(argv[2],NULL) << "\" and valid=1";
    count = 0;
    ret = sqlite3_exec(db, query.str().c_str(), count_callback, 0, &errmsg);
    if ( ret != SQLITE_OK )
      sqlite3_die(errmsg,query.str());
    if ( count > 0 )
      die("This wsgi file has already been registered");

    // check for existing entries by this UID
    query.str("");
    query << "select count(*) from wsgi where uid=" << uid << " and valid=1";
    count = 0;
    ret = sqlite3_exec(db, query.str().c_str(), count_callback, 0, &errmsg);
    if ( ret != SQLITE_OK )
      sqlite3_die(errmsg,query.str());
    if ( count > 0 )
      die("You can only have one WSGI file registered, so you must first remove it via -remove (and you can find it via -list)");

    // get the app name
    if ( argc == 4 )
      appname = string(argv[3]);
    else
      // the appname field is set to the default above
      cout << "No app name provided, so assuming '" << DEFAULT_APP_NAME << "'" << endl;

    // insert entry into DB
    query.str("");
    query << "insert into wsgi values (null," << uid << ",\"" << realpath(argv[2],NULL) 
	  << "\",1,datetime(),null,\"" << appname << "\")";
    ret = sqlite3_exec(db, query.str().c_str(), NULL, NULL, &errmsg);
    if ( ret != SQLITE_OK )
      sqlite3_die(errmsg,query.str());

    cout << "entry registered as /django/" << get_userid_by_uid(uid) << endl;
  }

  // list entries in the DB
  if ( mode == MODE_LIST ) {
    query.str("");
    query << "select * from wsgi where valid=1";
    if ( (uid != 0) && (uid != 1000) ) // uids 0 and 1000 see all...
      query << " and uid=" << uid;
    ret = sqlite3_exec(db, query.str().c_str(), list_callback, 0, &errmsg);
    if ( ret != SQLITE_OK )
      sqlite3_die(errmsg,query.str());
  }

  // remove an entry from the DB
  if ( mode == MODE_REMOVE ) {

    // parse int value provided
    int id;
    int ret = sscanf(argv[2],"%d",&id);
    if ( ret != 1 )
      die("Invalid numerical ID value for -remove");

    // create query, and pull that data from the DB
    query.str("");
    query << "select * from wsgi where id=" << id << " and valid=1";
    find_filename = string("");
    ret = sqlite3_exec(db, query.str().c_str(), find_callback, 0, &errmsg);
    if ( ret != SQLITE_OK )
      sqlite3_die(errmsg,query.str());
    if ( find_filename == "" )
      die("no such entry exists");

    // check if UID matches
    if ( (uid != 0) && (uid != 1000) )
      if ( uid != find_uid )
	die("you can't remove that entry");

    // remove entry
    query.str("");
    query << "update wsgi set valid=0, removed=now() where id=" << id;
    ret = sqlite3_exec(db, query.str().c_str(), NULL, NULL, &errmsg);
    if ( ret != SQLITE_OK )
      sqlite3_die(errmsg,query.str());

    cout << "entry removed" << endl;
  }

  // all of these perform the generation
  query.str("");
  query << "select * from wsgi where valid=1";
  ret = sqlite3_exec(db, query.str().c_str(), regenerate_callback, 0, &errmsg);
  if ( ret != SQLITE_OK )
    sqlite3_die(errmsg, query.str());

  if ( mode != MODE_LIST ) {

    // write to django.conf and django-ssl.conf files
    ofstream fout(DJANGO_CONF_FILE);
    fout << wsgifile.str();
    fout.close();
    ofstream fout2(DJANGO_SSL_CONF_FILE);
    fout2 << wsgisslfile.str();
    fout2.close();
    cout << "django.conf file regenerated" << endl;

    // reload apache
    cout << "reloading the web server..." << endl;
    if ( !file_exists(APACHE2_RELOAD_CMD) )
      die("unable to find apache2 reload command");
    stringstream cmd;
    cmd << APACHE2_RELOAD_CMD << " > /dev/null";
    system(cmd.str().c_str());
  }

  return 0;
}

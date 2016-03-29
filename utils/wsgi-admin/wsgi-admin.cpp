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
 * This program will take, as a command-line parameter, a wsgi file
 * (such as Django's wsgi.py), and either add it to, or remove it
 * from, a sqlite db.  The django.conf for apache is then
 * re-generated, checking each existing wsgi file to make sure they
 * are valid.
 *
 * Full directions for how to use it can be found in the
 * django-getting-started.md (and .html) files in the docs/ directory
 * in the SLP repo.
 *
 * Installation:
 * - Compile with 'make', put it somewhere (such as /usr/local/bin)
 * - You will need this to be in a group that can write to the two
 *   conf files (in the directories declared in the #defines, below)
 * - Run 'chmod 4755 wsgi-admin': this will set the set-uid bit when
 *   the program runs.  This means the program will run as the user
 *   who owns it, who (presumably) can edit those files.
 * - the django.conf file needs to be writable by the user who owns
 *   the compiled file
 * - the DB file also needs to be writable by that user
 *
 * Note that the app name is no longer used (it was previously used as
 * part of the static directory), but has not been removed from this
 * program yet.
 *
 * The callback functions were adapted from
 * http://www.sqlite.org/quickstart.html
 *
 *
 * Apache2 Configuration
 *
 * Note that that multiple apache2 sites (such as http:// and
 * https://) can not *both* define the WSGIDaemonProcess.  The site
 * that is brought up first (likely http://) must define it.  For that
 * reason, that command is enclosed in an <IfModule> command.  To
 * include it in the http:// site (likely 000-default.conf), have the
 * following stanza:
 *
 *
 * <IfDefine NoDaemonProcess>
 *   UnDefine NoDaemonProcess
 * </IfDefine>
 * Include django.conf
 *
 * For the htts:// site (likely default-ssl.conf), have the following
 * stanza:
 *
 * Define NoDaemonProcess
 * Include django.conf
 *
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
 * rootdir boolean default 0
 * staticdir text
 * urluserid text
 *
 * The line to create this in sqlite3:
 * create table wsgi(id integer primary key asc, uid int, wsgi text, valid boolean, added datetime, removed datetime, app text, rootdir boolean default 0, staticdir text, urluserid text);
 *
 * Note that the urluserid allows for the URL to be different than the
 * userid; if not set, it defaults to the userid.  While this program
 * does pay attention to that value, it can not be set via this
 * program, and must be manually set in the django.db.
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
#define APACHE2_RELOAD_CMD "/usr/local/bin/reload-apache2"
#define URL_PREFIX "/django"
#define DATABASE_FILE "/etc/apache2/django.db"
#define DEFAULT_APP_NAME "polls"

// some global variables
stringstream wsgifile, query;
int count = 0, find_uid = 0, reg_root = 0, remove_id = 0, force_uid = 0;
string find_filename = "", appname = string(DEFAULT_APP_NAME), wsgi_file_name, staticdir;
bool check_file = true, compact_list = false, show_all = false;

// for when they invoke it incorrectly...
void printUsage(char *argv0, bool doexit = true) {
    cerr << "Usage:  " << argv0 << " -register -file <wsgi_file> [-app <app_name>] [-root]\n"
         << "\t" << argv0 << " -remove -id <num>\n"
         << "\t" << argv0 << " -list [-compact]\n"
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
int get_UID_for_file (const char* filename) {
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
bool is_valid_wsgi_file(const char *filename) {
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
    string obtained(result);
    stringstream expected;
    expected << filename << ": Python script, ASCII text executable\n";
    if ( obtained == expected.str() )
        return true;
    expected.str("");
    expected << filename << ": Python script, ASCII text executable, with CRLF line terminators\n";
    return ( obtained == expected.str() );
}

// how to handle the data returned when displaying the list of the DB entries
static int list_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for( int i = 0; i < argc; i++ ) {
        if ( compact_list ) {
            cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "; ";
        } else {
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
static int regenerate_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int uid, rootdir;
    sscanf(argv[1],"%d",&uid);
    char *filename = argv[2];
    if ( !file_exists(argv[2]) )
        return 0;
    if ( !is_valid_wsgi_file(argv[2]) )
        return 0;
    sscanf(argv[7], "%d", &rootdir);
    string userid(get_userid_by_uid(uid)); // userid of who registered this
    string fullpath(realpath(filename,NULL)); // full path of the wsgi.py file
    int pos = fullpath.rfind("/");
    string up1 = fullpath.substr(0,pos); // the directory that the wsgi.py file is in
    string basename = fullpath.substr(pos+1); // typically 'wsgi.py', the actual file name
    string app(argv[6]); // the app name
    pos = up1.rfind("/");
    string up2 = up1.substr(0,pos); // the directory *above* the wsgi.py file
    string staticdir; // the static directory, which is determined below
    stringstream foo;
    if ( (argv[8] == NULL) || (strlen(argv[8]) == 0) ) {
        // the assumption is that there is a static/ directory in the main project directory...
        foo << up2 << "/static";
        staticdir = foo.str();
    } else
        /// ... unless they have specified otherwise
        staticdir = string(argv[8]);
    string urluserid;
    if ( (argv[9] == NULL) || (strlen(argv[9]) == 0) )
        // the assumption is that url userid is the same as their userid
        urluserid = userid;
    else
        /// ... unless they have specified otherwise
        urluserid = string(argv[9]);

    wsgifile << "Alias " << (rootdir?"":URL_PREFIX) << "/" << urluserid << "/static " << staticdir << "\n"
        << "<Directory " << staticdir << ">\n"
        << "  Require all granted\n"
        << "</Directory>\n";
    wsgifile << "WSGIScriptAlias " << (rootdir?"":URL_PREFIX) << "/" << urluserid << " " << fullpath << "\n";
    wsgifile << "<IfDefine !NoDaemonProcess>\n"
	     << "  WSGIDaemonProcess " << urluserid << " python-path=" << up2 << "\n"
	     << "</IfDefine>\n";
    wsgifile << "<Location " << (rootdir?"":URL_PREFIX) << "/" << urluserid << ">\n"
        << "  WSGIProcessGroup " << urluserid << "\n"
        << "</Location>\n"
        << "<Directory " << up1 << ">\n"
        << "  <Files " << basename << ">\n"
        << "    Require all granted\n"
        << "  </Files>\n"
        << "</Directory>\n\n";
    return 0;
}

// what does the main() say?
int main(int argc, char **argv) {

    enum { MODE_NONE, MODE_REGISTER, MODE_REMOVE, MODE_REGENERATE, MODE_LIST } mode = MODE_NONE;
    char *filename = NULL;
    int uid = getuid(), ret;

    if ( argc == 1 )
        printUsage(argv[0]);

    // parse command line parameters, get wsgi file and mode
    for ( int i = 1; i < argc; i++ ) {
        string param(argv[i]);
        if ( param == "-help" )
            printUsage(argv[0]);
        else if ( param == "-register" )
            mode = MODE_REGISTER;
        else if ( param == "-remove" )
            mode = MODE_REMOVE;
        else if ( param == "-regenerate" )
            mode = MODE_REGENERATE;
        else if ( param == "-compact" )
            compact_list = true;
        else if ( param == "-root" ) {
            //if ( (uid != 0) && (uid != 1000) )
            //die ("You are not allowed to use the -root flag");
            reg_root = 1;
        } else if ( param == "-list" )
            mode = MODE_LIST;
        else if ( param == "-all" ) {
            if ( (uid != 0) && (uid != 1000) )
                die ("You are not allowed to use the -all flag");
            show_all = true;
        } else if ( param == "-staticdir" ) {
            if ( argc == i+1 )
                die ("Must supply a directory name to -staticdir");
            staticdir = string(argv[++i]);
        } else if ( param == "-nocheck" ) {
            if ( (uid != 0) && (uid != 1000) )
                die ("You are not allowed to use the -nocheck flag");
            check_file = false;
        } else if ( param == "-app" ) {
            if ( argc == i+1 )
                die ("Must supply a app name to -app");
            appname = string(argv[++i]);
        } else if ( param == "-uid" ) {
            if ( (uid != 0) && (uid != 1000) )
                die ("You are not allowed to use the -uid flag");
            int ret = sscanf(argv[++i], "%d", &force_uid);
            if ( ret != 1 )
                die ("Invalid integer format to -uid");
        } else if ( param == "-id" ) {
            if ( argc == i+1 )
                die ("Must supply a numerical id to -id");
            int ret = sscanf(argv[++i], "%d", &remove_id);
            if ( ret != 1 )
                die ("Invalid integer format to -id");
        } else if ( param == "-file" ) {
            if ( argc == i+1 )
                die ("Must supply a file name to -file");
            wsgi_file_name = string(argv[++i]);
            if ( !file_exists(wsgi_file_name) )
                die ("File does not exist!");
        } else
            printUsage(argv[0]);
    }

    // check the parameters
    if ( (wsgi_file_name == "") && (mode == MODE_REGISTER) )
        die("Must supply a file name with -register");
    if ( (remove_id == 0) && (mode == MODE_REMOVE) )
        die("Must supply a id (via -id) with -remove");

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
        for ( int i = 0; i < wsgi_file_name.length(); i++ )
            if ( (wsgi_file_name[i] == '\\') || (wsgi_file_name[i] == '"') || (wsgi_file_name[i] == '\'') || (wsgi_file_name[i] == ';') )
                die ("Try choosing a file name that does *not* lend itself to SQL injection attacks");

        // does the file exist?
        if ( !file_exists(wsgi_file_name) ) {
            cerr << "File '" << wsgi_file_name << "' does not exist!" << endl;
            exit(0);
        }

        // is it a Python file?
        if ( check_file && !is_valid_wsgi_file(wsgi_file_name.c_str()) )
            die("Not a valid WSGI file");

        // is it owned by the user executing this script?
        int fuid = get_UID_for_file (wsgi_file_name.c_str());
        if ( force_uid != 0 )
            fuid = force_uid;
        if ( (uid != 0) && (uid != 1000) && (fuid != uid) )
            die ("You cannot register a file that you do not own");

        // check for duplicate entries of that file
        query.str("");
        query << "select count(*) from wsgi where wsgi=\""
              << realpath(wsgi_file_name.c_str(),NULL) << "\" and valid=1";
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

        // we are not using the app name anymore (see the comments at the
        // top of this file), so there is no need anymore to warn about
        // using the default app name...
        //if ( appname == string(DEFAULT_APP_NAME) )
        //cout << "Using the default app name of '" << DEFAULT_APP_NAME << "'" << endl;

        // insert entry into DB
        query.str("");
        query << "insert into wsgi values (null," << uid << ",\"" << realpath(wsgi_file_name.c_str(),NULL)
              << "\",1,datetime(),null,\"" << appname << "\"," << reg_root << ",\"" << staticdir << "\",\"\")";
        ret = sqlite3_exec(db, query.str().c_str(), NULL, NULL, &errmsg);
        if ( ret != SQLITE_OK )
            sqlite3_die(errmsg,query.str());

        cout << "entry registered as /django/" << get_userid_by_uid(uid) << endl;
    }

    // list entries in the DB
    if ( mode == MODE_LIST ) {
        query.str("");
        query << "select * from wsgi";
        if ( !show_all )
            query << " where valid=1";
        if ( (uid != 0) && (uid != 1000) ) // uids 0 and 1000 see all...
            query << " and uid=" << uid;
        ret = sqlite3_exec(db, query.str().c_str(), list_callback, 0, &errmsg);
        if ( ret != SQLITE_OK )
            sqlite3_die(errmsg,query.str());
    }

    // remove an entry from the DB
    if ( mode == MODE_REMOVE ) {

        // create query, and pull that data from the DB
        query.str("");
        query << "select * from wsgi where id=" << remove_id << " and valid=1";
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
        query << "update wsgi set valid=0, removed=datetime() where id=" << remove_id;
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

        // write to django.conf
        ofstream fout(DJANGO_CONF_FILE);
        fout << wsgifile.str();
        fout.close();
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

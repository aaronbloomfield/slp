Managing Local Settings
=======================

[Go up to the main SLP documents page](index.html) ([md](index.md))

When a system is used or deployed in multiple locations, there will be differences in the configuration of each system.  This is especially true of the group projects, where it is being used on each team member's local machine, on the course server, and on the final deployment host.

This document discusses ways to properly manage local settings, including passwords.  The full set of directions are specific to Django, and the addendums at the end discuss using the same process for Ruby on Rails or CakePHP.

### Django instructions

1. Create a `local_settings.py`, and put it in the same directory as the `settings.py`.  Do ***NOT*** commit this file to the repository!
2. Create a `.gitignore` file in that same directory, and put `local_settings.py` as the one (and likely only) line in that file.  Stage the `.gitignore` file to be committed: `git add .gitignore`.
3. Put any and all local settings in your `local_settings.py` file.  While these are going to include the database information (engine, host, user, password, and database name), they might also include values for such things as the `STATIC_ROOT` and `STATIC_URL` fields that are in `settings.py`.  An example might look like the following; note that we are (intentionally) using lower case variable names here:
```
database_engine = "django.db.backends.mysql"
database_name = "slp_project"
database_user = "project"
database_password = "abcdefg"
database_host = "localhost"
static_root = /home/slp/project/project/static
static_url = '/project/static/'
```
4. In `settings.py`, *at the top*, import that file: `from project import local_settings`
5. Also in `settings.py`, modify the values listed therein to use the values imported from the module.  Thus, the relevant sections of your `settings.py` might look like the following (obviously your values will be different):
```
DATABASES = {
        'default': {
            'ENGINE': local_settings.database_engine,
            'NAME': local_settings.database_name,
            'USER': local_settings.database_user,
            'PASSWORD': local_settings.database_password,
            'HOST': local_settings.database_host,
        }
}
...
STATIC_URL = local_settings.static_url
STATIC_ROOT = local_settings.static_root
```
6. Create a `local_settings.py.template` file, which is a copy of `local_settings.py`.  But change any sensitive data over to some default.  In this example, you would change the password of "abcdefg" to "password".  Other things to change are API keys, etc.  It's fine to keep non-sensitive information, such as the rest of the database information (other than the password), and the static lines, in the `local_settings.py.template` file.  We are going to add this file to the repo -- it shows how to create a `local_settings.py`, but does not include any passwords.  Thus, stage *this* file to be committed: `git add local_settings.py.template`.
7. Include, somewhere in your directions, a brief how-to about creating the `local_settings.py` file (tell them to copy `local_settings.py.template` over and modify the fields).
8. Push your changes: `git push`.
9. If you -- or anybody else -- *ever* made a push that contained a commit with a password (such as your database password), you ***MUST*** change that password.  There are existing bots that will scan the entire history of github repositories just to look for passwords.  You can do this in MySQL via `set password = password('bananananana');` in the MySQL prompt.
10. Have everybody create their own `local_settings.py` in their respective clone that will have their own local settings.  Users can use different database settings by editing that file. Because the `local_settings.py` file is in the `.gitignore`, any changes to that file will not be committed to the repository.
11. When configuring Travis, copy the `local_settings.py` file over to `settings.py` via a `cp` command in the `before_script` section.  You can just keep the password as it is (likely just "password"), and tell Travis, in the database setup section, to have "password" be the DB password.
12. Keep in mind that you can use this file to switch over to an SQLite3 database.  Your `local_settings.py` example from above, configured for a SQLite3 database, might look like the following:
```
database_engine = "django.db.backends.sqlite3"
database_host = "don't care"
database_name = os.path.join(BASE_DIR, 'db.sqlite3')
database_user = "don't care"
database_password = "don't care"
static_root = /home/slp/project/project/static
static_url = '/project/static/'
```
In this example, some of the database fields are not used for SQLite3 (in particular, the host, user, and password), so we can set them to any values.

### Ruby on Rails instructions

Ruby on Rails uses a `database.yml` file for the database configuration.  So you can create a `database.yml.template` file following the process described above.  If there are other non-database settings that you want to allow for easy local configuration, you can create a `local_settings.rb` file, `require` that file into the appropriate existing configuration file, and it will work the same.  Note that this will mean that you have *two* different files that have a template version (and are thus in your `.gitignore`).

### CakePHP instructions

The CakePHP configuration is similar, but you will have a `app_local.php` file, which you will `require` into `app.php`, instead of Python files.

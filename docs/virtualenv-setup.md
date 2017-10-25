SLP: Python's virtualenv setup
==============================

[Go up to the main SLP documents page](index.html) ([md](index.md))

Python has a [virtualenv](https://pypi.python.org/pypi/virtualenv)
package, which allows one to keep the installed Python packages, as
well as various Python versions, stored locally.  This allows users to
install the packages, and the Python version can differ from the
system default.

**EVERYBODY SHOULD BE ON PYTHON 3!** We aren't using Python 2 any
more.

The following are the steps to set up a Django app via `virtualenv`.
This all works on the course server as a regular user.

1. Create the virtualenv directory.  Do this via
   `virtualenv -p /usr/bin/python3 ~/venv-django-1.11`.
    - The `-p /usr/gbin/python3` is how you specify the Python
      version, and we are using Python 3.
    - This will create a `/home/slp/user/venv-django-1.11` directory
      (where `user` is the user name).
    - It doesn't matter what your directory is named, as long as it's
      a reasonable name.
	    - However, it should be in your home directory (the `~/`
          before the directory name did that), and it should **NOT**
          be in your repo.
	    - And don't bother putting in the bugfix part of the version
          number in the directory (the `.6` of Django version
          `1.11.6`), since that is likely to change throughout the
          project.
2. Install the packages that you need.
    - From a command prompt, run `source ~/venv-django-1.10/bin/activate`.
		- This command will set the environment variables so that
          package installs go into your virtual environment, rather
          than to the entire system (which your user can't do).
		- You will notice a difference in the prompt so that you can
          tell if you are in this environment.
		- You can run `deactivate` to exit this environment without
          logging out (or you can just log out).
	- Once you are in this environment, install your packages as you
      would normally: `pip3 install foo`, 
      `pip3 install Django==1.11.6`, `pip3 install -r requirements.txt`,
	  etc.
	- Note that you are using Python 2 (which you shouldn't be), you
      would use `pip` and not `pip3`.
3. Install the Django package into your virtual environment!
	- The system one may change, and your app may not work properly
      with a changed Django version.
	- Once in the virtualenv environment (from the previous step), run
      `pip3 install Django==1.11.6`.
	- Unless you have a reason to do so otherwise, always install the
      latest Django version by explicitly stating the version number
      in the `pip3` command.
	- The latest version can be found at
      [https://www.djangoproject.com/](https://www.djangoproject.com/).
4. Get the virtualenv library directory name.
    - This will be needed in the next step.
	- If your directory created in step one was `~/venv-django-1.11`,
      then your directory will be something like
      `/home/slp/user/venv-django-1.11/lib/python3.5/site-packages`.
    - Note that the user name, virtual environment directory name, and
      virtual environment directory will likely be different for you.
	- You'll need the full path name, not a shortcut (i.e., don't use
      `~`, so `~/venv-django-1.11/lib/python3.5/site-packages` is not
      valid).
	- We'll call this the "library path".
5. De-register the old Django app from the web server.
    - Run `wsgi-admin -list`, and note the ID number (first line).
	- Run `wsgi-admin -remove -id 123`, where "123" is the ID number
      from above.
6. Re-register the Django application.
	- There are a number of parameters that you need to supply:.
		- `-register`: because we are registering a file
		- `-file mysite/mysite/wsgi.py`: the wsgi.py file for your
          project; you can do a absolute path or a relative path, but
          it's value will change depending on your directory structure.
		- `-root`: this registers it as `/user` instead of
          `/django/user`.  **Only** use this for the project sites, as
          we will only be looking at `/django/user` to grade the homeworks.
		- `-path
          /home/slp/user/venv-django-1.11/lib/python3.5/site-packages`:
          this is the library path from the previous step.
	- Thus, a full wsgi-admin command might be one of the following
      (the first one doesn't have `-root`, the second one does):
        - `wsgi-admin -register -file mysite/mysite/wsgi.py -path /home/slp/user/venv-django-1.11/lib/python3.5/site-packages`
        - `wsgi-admin -register -file mysite/mysite/wsgi.py -root -path /home/slp/user/venv-django-1.11/lib/python3.5/site-packages`
7. Test the site at http://server/django/user or http://server/user,
   where "server" is the course server, and "user" is the user who
   registered the file.
    - If something does not work, there could be many reasons.
      Viewing the apache error log will help -- run `view-apache2-log`
      to see the error messages printed.
	- Note, however, that Django has an annoying habit of printing the
      error message only the first time it occurs after a reload or
      restart.
	- Thus, you may want to reload the web server (`reload-apache2`),
      reload the site in your browser, and then look at the apache2
      log then.
    - If the apache log is giving problems about MySQL ("Did you
      install mysqlclient or MySQL-python?"), try *uninstalling* all
      MySQL packages, and seeing if the default on the system works.

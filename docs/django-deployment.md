SLP: Django: Deployment
=======================

[Go up to the main SLP documents page](index.html) ([md](index.md))

This page is for how to *deploy* your Django apps on an Ubuntu server -- specifically, Ubuntu 14.04.  This is not needed for your local development, as you can run `python manage.py runserver` to test it out locally, or test it out on the course server provided.  However, if you ***do*** want to get it running through Apache, read on...

Installing packages
-------------------

To install Django on your own system, under Ubuntu 14.04, just enter `sudo apt-get install python-django python-mysqldb`, and it will do the rest.  As of the writing of this tutorial (Aug 31, 2014), this installs Python version 2.7.6 and Django version 1.6.1.

Configuring Apache
------------------

This part specifically deals with configuring *multiple* django apps on the same web server, as that is what is needed for this course.  For *each* django app, the following 8 lines need to be added to `/etc/apache2/sites-available/000-default.conf`, right above the end `</VirtualHost>` line:

```
Alias /django/user/static /home/slp/user/mysite2/polls/static
<Directory /home/slp/user/mysite2/polls/static>
  Require all granted
</Directory>
WSGIScriptAlias /django/user /home/slp/user/djangohw/djangohw/wsgi.py
WSGIDaemonProcess user python-path=/home/slp/user/djangohw
<Location /django/user>
  WSGIProcessGroup user
</Location>
<Directory /home/slp/user/djangohw/djangohw>
  <Files wsgi.py>
    Require all granted
  </Files>
</Directory>
```

A bunch of notes:

- `user` is the username of whomever is running the Django app, and `djangohw` is the name of the project.
- The first four lines deal with the `static` directory -- since there are multiple Django apps, there will be multiple static directories.  See the [Django getting started](django-getting-started.html) ([md](django-getting-started.md)) page for how to configure a given Django project to use the correct static directory.
- The [WSGIScriptAlias](https://code.google.com/p/modwsgi/wiki/ConfigurationDirectives#WSGIScriptAlias) line indicates the URL (after the server) links to the particular Django app.  As we want them in sub-directories, we have the URL part be `/django/user`.  Note that we do *not* have to create the `/django/` directory in the HTML document root (likely `/var/www/html`).
- The [WSGIProcessGroup](https://code.google.com/p/modwsgi/wiki/ConfigurationDirectives#WSGIProcessGroup) creates a process group, which allows one (or more) apps to run as a specific user. The `user` part of that line needs to be a valid user name on the system
- The [WSGIDaemonProcess](https://code.google.com/p/modwsgi/wiki/ConfigurationDirectives#WSGIDaemonProcess) line indicates that this particular Django app will run under the `user` user, and the path to the django app is provided. The `user` part of that line needs to match the one in the WSGIProcessGroup.
- The `Require all granted` line allows the viewing of the Django app.  If you are using Apache 2.2 (or earlier), replace `Require all granted` with two lines: `Order deny,allow` and then `Allow from all`.

Sine there are going to be many users in a system, it will be easier to put this in a separate file (say, called `/etc/apache2/django.conf`), and insert a `Include django.conf` line in `/etc/apache2/sites-available/000-default.conf` (also right above the `</VirtualHost>` line).

If you are enabling these through SSL, there is a slight change.  When Apache restarts (or reloads), the default (i.e., non-SSL) site is brought up first, followed by the SSL site.  As the daemon process has already been declared in 000-default.conf (for the non-SSL site), it will cause an error to declare it again for the SSL site.  Thus, the SSL version should not have the `WSGIDaemonProcess` line; all the other lines remain the same.  This implies that the file included from default-ssl.conf will be a different one than the one included from 000-default.ssl.

Managing multiple users
-----------------------

The issue with the WSGI module is that if **any one** of the WSGI files it is configured with does not exist, then all of the WSGI modules don't work.  This means you can't pre-generate the list of WSGI links for a class, as none of them will work until everybody has it set up, which is not viable.

To work around this, you can see the [wsgi-admin.cpp](../utils/wsgi-admin/wsgi-admin.cpp.html) ([src](../utils/wsgi-admin/wsgi-admin.cpp)), which will allow students to register and remove their WSGI apps.  It will update the necessary Apache configuration files, and then reload the web server.  It can also be used just to do the regeneration and reloading -- and if a file is not found (or is not valid), then it is excluded from the configuration files.  Installation is tricky, but the process is described in the comments at the top of that file.  The directions for how to use it can be found on the [Django getting started](django-getting-started.html) ([md](django-getting-started.md)) page.

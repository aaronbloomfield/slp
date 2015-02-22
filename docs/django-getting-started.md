SLP: Django: Getting Started
============================

[Go up to the main SLP documents page](index.html) ([md](index.md))

### Install Django

Django has already been installed on the VirtualBox image provided.

To install Django on your own system, under Ubuntu 14.04, just enter `sudo apt-get install python-django python-mysqldb`, and it will do the rest.  As of the writing of this tutorial (Aug 31, 2014), this installs Python version 2.7.6 and Django version 1.6.1.


### Setting up a new Django app

Much of these directions are based on the [Django intro tutorial](https://docs.djangoproject.com/en/1.6/intro/) from [Django Proect](https://www.djangoproject.com/).  Note that the default version for much of the tutorial is 1.7 (the latest Django release), and you have to change it, via the link in the lower right, to 1.6, which is the version we are using.

To create a Django app, follow these steps.  They are adapted from [part 1 of the Django tutorial](https://docs.djangoproject.com/en/1.6/intro/tutorial01/).

1. Enter `django-admin startproject mysite`.  This will create a `mysite/` directory with a bunch of files in it.
    - Note that the binary we are calling is `django-admin`, not `django-admin.py` (Ubuntu installs it as the former, whereas the tutorial indicates it is named the latter).
2. To view your app, you can run `python manage.py runserver` from inside the `mysite/` directory.  This will print a URL, such as `http://127.0.0.1:8000/` -- you can view that URL in your browser, and it should look exactly like the image at the bottom of this page.
3. To update the database information, edit mysite/mysite/settings.py
    - To use MySQL, change the `ENGINE` value in `DATABASES` from `django.db.backends.sqlite3` to `django.db.backends.mysql`
	- Enter values for your DB credentials: `USER`, `NAME`, `PASSWORD`, and `HOST`.
	- A completed `DATABASES` section might look like the following:
```
DATABASES = {
        'default': {
            'ENGINE': 'django.db.backends.mysql',
            'NAME': 'mst3k',
            'USER': 'mst3k',
            'PASSWORD': 'password',
            'HOST': '127.0.0.1',
        }
}
```
	- Note that there is no easy way to add a DB table prefix to every table -- instead, you would have to specify the name of *every* table that Django uses.  You can see a whole bunch of people getting twitchy over this [here](https://code.djangoproject.com/ticket/891).
    - And edit `TIME_ZONE` to match our time zone (EST).
4. From the `mysite/` directory, run `python manage.py syncdb`.  This will set up the DB tables.
    - The 'superusers' that it prompts you for are a login for your web system -- you can use your userid, and pick any password that you'd like

At this point, you Django app is up and running, even if it doesn't do much.  You can now start about half-way down on the [Django tutorial, part 1](https://docs.djangoproject.com/en/1.6/intro/tutorial01/#creating-models) page (start at the "Creating models" header).

### Viewing it locally

As mentioned in [part 1 of the Django tutorial](https://docs.djangoproject.com/en/1.6/intro/tutorial01/), to view your Django app locally, you run `python manage.py runserver`, and then view it in the URL provided (likely `http://127.0.0.1:8000/`).

### Configuring it on Apache on the course server

When running it locally, you view it via `python manage.py runserver`.  But on the course server, you will have to configure it run through the apache web server.

##### Registering the Django project

When you have created your Django project, you will need to tell the web server that your file exists.  The module that runs Python apps on the Apache web server is called WSGI (Web Server Gateway Interface), pronounced "wus-gee".  On the server, there is a `wsgi-admin` command-line tool to do exactly that, since you can't edit the web server configuration files yourself.  Note that you can only have one WSGI app registered at a time.  You will need to find the wsgi.py file in your django app (likely in `mysite/mysite/wsgi.py`, where `mysite` is the name of your Django app).

There are four main "modes" to that command:

- `-register` will register a wsgi file, supplied with the `-file` flag
- `-list` will list your existing entry, including the ID (which is needed to remove that entry)
- `-remove` will remove a previously registered wsgi file via entry's ID number, which is specified via the `-id` flag
- `-regenerate` will regenerate the web server configuration files and reload the web server.  This is automatically done on a `-register` or `-remove`.  But it could be the case that somebody accidentally deleted their Python app files, which will cause **ALL** of the WSGI apps to stop working -- the `-regenerate` command will re-create the configuration files without the missing Django apps.

There are a few other flags to the above:

- `-file <wsgi_file>` specifies which file to register; it must be the `wsgi.py` file
- `-id <id_num>` will remove the wsgi file entry with the specified *integer* ID; the IDs can be found via the `-list` flag
- `-app <app_name>` will assume that the app name is what is passed; it defaults to 'polls' (since that is what the tutorial uses).  This really on matters for the static directory name.
- `-compact` will cause the output of the `-list` to be one line per entry
- `-staticdir <dir>` specifies what the static directory is; this must be a full path name

Lastly, there are a few flags that are restricted as to who can use them:

- `-uid <uid_num>` will register the wsgi file as a different user (you can find the UID of a user via `getent passwd <user>`)
- `-nocheck` will skip the check as to whether the passed wsgi file is a valid wsgi file or not
- `-root` will register the file as http://server/user, rather than http://server/django/user
- `-all` will list all the registered wsgi files, including the ones that have been removed

So, to register your file for the tutorial:

```
wsgi-admin -register -file djangohw/djangohw/wsgi.py
```

To find the existing entries that you have:

```
wsgi-admin -list
```

To remove your entry with ID 42:

```
wsgi-admin -remove -id 42
```


##### Configuring the Django app's URLs

The next thing to do is to update your urls.py (the project-wide one, not the app-specific one); this may be in `~/djangohw/djangohw/urls.py`.  A typical one will have the following as the `urlpatterns` array:

```
urlpatterns = patterns('',
    url(r'^polls/', include('polls.urls', namespace="polls")),
    url(r'^admin/', include(admin.site.urls)),
)
```
 
This describes a route for two URLs: http://server/django/mst3k/polls and http://server/django/mst3k/admin.  And, indeed, both of those URLs work.  But it doesn't define a route for the http://server/django/mst3k/ URL.  To do that, add a line similar to the following into that urlpatterns array (above the existing 'polls' line):


```
    url(r'^', include('polls.urls', namespace="polls")),
```

This will cause the base URL of the Djagno project (http://server/django/mst3k) to display the contents of the polls/ app.

##### Setting the static directory

There are many static files that are needed for a Django project; these are described in more detail in [part 6 of the Djago tutorial](https://docs.djangoproject.com/en/1.6/intro/tutorial06/).  Normally, the static files are found in http://server/static.  However, as there are many Django projects running on this web server -- and thus many *different* static directories, the URL is different: ours will be at http://server/django/mst3k/static.  You need to tell the Django app where these files are.  In the settings.py file (which you edited, above), you will need to change the `STATIC_URL` line to look like the following (replacing mst3k with your userid):

```
STATIC_URL = '/django/mst3k/static/'
```

Once that is done, the tutorial (specifically, [part 6](https://docs.djangoproject.com/en/1.6/intro/tutorial06/) proceeds normally, and no changes to the tutorial are needed.

### Updating the Django app

If you have updated your Django app, you need to tell the web server that this has occurred.  While you can reload the web server (via the `reload-apache2` command, it's much better to update the timestamp on your wsgi.py file.  To do this, use the `touch` command:

```
touch mysite/mysite/wsgi.py
```

This will update the timestamp on the wsgi.py file to the current time.  The web server (actually the WSGI module) will detect this, and reload the app.

### Initial Djano app image

![](images/django-initial.png)

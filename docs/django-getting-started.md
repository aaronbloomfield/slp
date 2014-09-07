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

### Configuring it on Apache

...


### Initial Djano app image

![](images/django-initial.png)

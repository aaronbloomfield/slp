SLP: Frameworks Homework
========================

[Go up to the main SLP documents page](index.html) ([md](index.md))

In this homework, you will create [MVC](http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller) systems using different frameworks: Ruby on Rails, and Django.

All of these systems will use the SAME database, and one of the important aspects of this homework is to make sure that they all use *different* tables in that database.

### Server setup

We are providing you a server to use for this homework.  In an effort not to broadcast the name of the course server -- and to allow it to be changed without having to update everything herein -- it is just called "the server" throughout these documents.  The server name and the login credentials are in the Post'Em tool in Collab.  It will likely be far easier to develop this on your own machine (presumably the VirtualBox image), and then move it to the server when it is ready.

The easiest way is to download the Virtual Box image from Collab (in Resources).  You can read the directions on the [VirtualBox introduction](virtualbox-intro.html) ([md](virtualbox-intro.md)) page for how to set it up.  If you want to configure your own machine, you can view the steps taken to create the image on the [VirtualBox image details](virtualbox-image-details.html) ([md](virtualbox-image-details.md)) page.

The provided course server has a valid (and properly signed) SSL certificate, so you can view any of the links through http:// or https://.

### MySQL

First, you will need to become familiar with MySQL, if you are not already.  Depending on your familiarity level, you may want to go through the [MySQL Introduction](mysql-intro.html) ([md](mysql-intro.md)) page.  Both new and advanced users will want to refer to the [MySQL Reference](mysql-reference.html) ([md](mysql-reference.md)) page throughout this course.

~~Note that the server has phpMyAdmin installed at `https://server/phpmyadmin/` (where "server" is the server used for the course).~~ Due to vulnerabilites in phpMyAdmin, it is disabled for the time being on the course server.  You will have to use the command line interface for now.

You will need to create a series of tables as part of this process - keep track of the MySQL commands that you issue, as you will have to repeat them on the server.  Alternatively, you can dump your entire MySQL database via `mysqldump -u user -p mst3k > db.sql`.  Here, `user` is whatever user can access the directory (perhaps "student", perhaps your UVa userid).  Or you can do it via phpMyAdmin.  Either way, you should have a file that contains all the SQL commands to create the necessary database tables - we'll need that shortly.

You will need to create a database on your local machine.  The easiest way is to have it be the same username and password as is on the server, so that you can use the same database configuration files.

- Start mysql as root: `mysql -u root -p`, and enter the root password (likely 'password')
- Create the database: `create database mst3k;`
- Grant permission for your user to use it: `grant all on mst3k.* to 'mst3k' identified by 'password';`
    - Note the `.*` after the database name (the first 'mst3k')
	- Use the same username and password as was provided on the server
- Call that line again, but put a `@'localhost'` after the userid:`grant all on mst3k.* to 'mst3k'@'localhost' identified by 'password';`
- Exit mysql

At this point, you have the same databse config on both your local machine as on the server.

### Uploading files to the server

If you are working at home, you will likely want to upload the files to the server.  To do so, you can try this command (presumably from your ~/html directory):

```
rsync -a --del --progress foobar/ mst3k@server:~/html/foobar/
```

Note that this command is very particular about the parameters!  If you were to specify `foobar` instead of `foobar/` (this is after the `--progress` parameter), then it will do something very different.

This command will upload all the *changed* files to the server.  The `-a` flag causes it to upload the entire directory structure.  The `--del` flag tells rsync to *delete* files on the remote server that have been deleted on the local copy.  And the `--progress` flag tells it to display the progress.

One of the issues is that this will overwrite files that have been modified, and this may not always be what you want.  For example, you may have differet files that specify the database user name and password.  So if you were to log into the server, and manually change the files, then it would overwrite them with the next rsync call.

To prevent this, you can use the `--exclude` flag, which causes it to not bother uploading certain files:

```
rsync -a --del --progress --exclude '.htaccess' foobar/ mst3k@server:~/html/foobar/
```

This will cause rsync to not upload any files called `.htaccess`.  Note that you should upload *everything* the first time (i.e., using the first rsync command given), then log into the server and manually change the .htaccess files.  You should then call the second rsync command from then on.

Which files you exclude will depend on which ones differ between your development machine and the server.  If you use the same database config, as described above, then you can use the same database credential files as well.

If you did a mysqldump on your local machine to a file called `db.sql`, you can load that data into the database on the server by entering:

```
cat db.sql | mysql -u mst3k -p mst3k
```

Replace mst3k with your userid; it will prompt you for your password.  This command must be run on the server.  To log into the server, enter:

```
ssh mst3k@server
```

Where 'mst3k' is your userid, and 'server' is the *full* name of the server.  Once you enter your password, you will be logged into a command prompt on the server.

### Ruby on Rails

[Ruby](https://en.wikipedia.org/wiki/Ruby_(programming_language)) is the programming language, and [Rails](https://en.wikipedia.org/wiki/Ruby_on_Rails) is the framework (library).  When used together it is called "Ruby on Rails".  While it is possible to use Ruby without Rails (or Rails without Ruby), that is rarely done.

First, read through the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page.  The assumption is that you will test it locally via `rails server`, and the deploy it on the course server via the directions on that page.  If you want to try your hand configuring your *own* Rails server, you can look at the [Ruby on Rails deployment](rubyrails-deployment.html) ([md](rubyrails-deployment.md)) page -- but be warned, it's a real pain in the rear to configure a Rails server.

We are using Ruby version 2.4.1 and Rails version 5.1.3 for this assigment.  If you have a more recent patch level (the last of the three digits of the version number), that's fine.  But you can't have a different major version (the first of those three numbers) or a different minor version (the middle of those three numbers).

On the course server, your app *must* be called `railshw`, and it *must* be in your home directory.  In other words, there must be a `/home/slp/mst3k/railshw/public` directory on the course server, as this is what the webserver will be looking for.  If you change anything in that path, the web server will never find your app.  You can then view your app at `http://server/rails/mst3k`, where "server" is the course server.

This part of the assignment is to complete the blog tutorial [here](http://guides.rubyonrails.org/v4.3/getting_started.html).  Recall that we are using Rails 5.1.3.

- That page makes some assumptions as to what is installed, which are all valid for both the VirtualBox image and the course server
- To create the Rails application, you just enter `rails new railshw -d mysql` in your home directory (this is in section 3.2 of that web page; be sure to name it `railshw` so the URL is correct)

Lastly, to see how to upload your Rails app to the server, see the "Uploading to the server" section of the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page.

### Django

First, read through the [Django getting started](django-getting-started.html) ([md](django-getting-started.md)) page.  The assumption is that you will test it locally via `python manage.py runserver`, and the deploy it on the course server via the directions on that page.  If you want to try your hand configuring your *own* Apache server to run Django, you can look at the [Django deployment](django-deployment.html) ([md](django-deployment.md)) page -- but be warned, it's a real pain in the rear to configure a Django server (although not as bad as Rails).

Unlike Ruby on Rails, you can name the Django project anything you want, and put it in any directory that you want.  Although naming it `djangohw` and putting it in our home directory on the course server would not be bad ideas.

The task for the Django part of this homework is to go through all *seven* parts of the introductory tutorial, found [here](https://docs.djangoproject.com/en/1.11/intro/) (the "Tutorial" line in the "First steps" section).  Make sure you are going through the 1.11 version!

A few notes on that tutorial (these were notes from a previous version of the tutorial, so take these with a grain of salt):

- When editing the models.py file, you may have to put two additional imports at the top that the tutorial does not mention (`from django.utils import timezone` and `import datetime`); this is described in more detail [here](https://code.djangoproject.com/ticket/19793)
- If python complains about not knowing what 'timezone' is, figure out which file it is running into this issue (this is listed in the stack trace), and put `from django.utils import timezone` at the top of that file
- Note that one of the tests (specifically, `test_detail_view_with_a_future_poll()`) only succeeds if a future poll returns a 404 page, which is what the original `results()` view did.  However, when the view was switched over to Django's generic views (the bottom of the [tutorial page 4](https://docs.djangoproject.com/en/1.11/intro/tutorial04/)), that is no longer the default behavior.  Thus, that test can be removed.  This will make much more sense once you have read through the end of the testing section in the tutorial.

When you have updated your Django project, remember to touch the wsgi.py file to make the web server reload your project; see the [Django getting started](django-getting-started.html) ([md](django-getting-started.md)) page for details.

It is much more difficult to rename your tables in Django -- you have to do it individually for each table.  But if the Ruby on Rails tables all start with `ruby_`, then you can leave the Django talbes to have the default name.  However, the names used in the Django tutorial should not conflict with the other tables therein.

For the submission, we are not going to be configuring it on the web server, for reasons that will be explained elsewhere.  Instead, we will look at your project files on the server, and run it through `python manage.py runserver`.  Thus, you MUST take the following steps:

- Upload your ENTIRE django project onto the course server; put it somewhere in your root directory so we can find it
    - There should be only three directories in your home directory: html, railshw (which contains the Rails part), and the directory that contains the Django part (you are welcome to have more, if you want; but those three are the ones we expect)
- Make ***SURE*** that the DB on the course server contains all the tables that you need for this.  You can do a mysqldump of the DB on your VirtualBox image, and then restore that to your DB on the course server.

### Submission

None!  We have access to your files on the course server, and we can view your projects at the following URLs.  It must be at these EXACT URLs, or else your assignment will not be graded.

- Ruby on Rails: `http://server/rails/mst3k`
- Django: `http://server/django/mst3k`
    - You *MUST* enter the urls.py rule to make that URL work; see the [Django getting started](django-getting-started.html) ([md](django-getting-started.md)) page for details (specifically, the "Configuring the Django project's URLs" section)

For any sites that have user authentication, you MUST allow user `kermit` with password `frog` to log in and see any and all features.  If `frog` doesn't work (it's too short), try `frog1234`.  (You do this via `python manage.py createsuperuser`).  For the Rails app, we will also try authenticating `dhh` and `secret`, which is what the tutorial tells you to use (so either one is fine for the Rails app).

Whether your program is late or not will be determined by the timestamp on the files in your home directory on the course server.

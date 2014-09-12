SLP: Frameworks Homework
========================

[Go up to the main SLP documents page](index.html) ([md](index.md))

In this homework, you will create [MVC](http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller) systems using three different frameworks: CakePHP, Ruby on Rails, and Django.

All of these systems will use the SAME database, and one of the important aspects of this homework is to make sure that they all use *different* tables in that database.

### Server setup

We are providing you a server to use for this homework.  In an effort not to broadcast the name of the course server -- and to allow it to be changed without having to update everything herein -- it is just called "the server" throughout these documents.  The server name and the login credentials are in the Post'Em tool in Collab.  It will likely be far easier to develop this on your own machine (presumably the VirtualBox image), and then move it to the server when it is ready.

The easiest way is to download the Virtual Box image from Collab (in Resources).  You can read the directions on the [VirtualBox introduction](virtualbox-intro.html) ([md](virtualbox-intro.md)) page for how to set it up.  If you want to configure your own machine, you can view the steps taken to create the image on the [VirtualBox image details](virtualbox-image-details.html) ([md](virtualbox-image-details.md)) page.

The provided course server has a valid (and properly signed) SSL certificate, so you can view any of the links through http:// or https://.

### MySQL

First, you will need to become familiar with MySQL, if you are not already.  Depending on your familiarity level, you may want to go through the [MySQL Introduction](mysql-intro.html) ([md](mysql-intro.md)) page.  Both new and advanced users will want to refer to the [MySQL Reference](mysql-reference.html) ([md](mysql-reference.md)) page throughout this course.

Note that the server has phpMyAdmin installed at `https://server/phpmyadmin/` (where "server" is the server used for the course).

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
rsync -a --del --progress cakephp/ mst3k@server:~/html/cakephp/
```

Note that this command is very particular about the parameters!  If you were to specify `cakephp` instead of `cakephp/` (this is after the `--progress` parameter), then it will do something very different.

This command will upload all the *changed* files to the server.  The `-a` flag causes it to upload the entire directory structure.  The `--del` flag tells rsync to *delete* files on the remote server that have been deleted on the local copy.  And the `--progress` flag tells it to display the progress.

One of the issues is that this will overwrite files that have been modified, and this may not always be waht you want.  For example, in CakePHP, you will have set up two .htaccess files, and they will be different on your local machine and the remote server.  So if you were to log into the server, and manually change the files, then it would overwrite them with the next rsync call.

To prevent this, you can use the `--exclude` flag, which causes it to not bother uploading certain files:

```
rsync -a --del --progress --exclude '.htaccess' cakephp/ mst3k@server:~/html/cakephp/
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

### CakePHP

First, proceed through the [CakePHP getting started](cakephp-getting-started.html) ([md](cakephp-getting-started.md)) page -- this should get you a first CakePHP web page working.  Be sure you use the 3.0 API of CakePHP!  The URL for that page ***MUST*** be `http://server/~mst3k/cakephp/` (where "mst3k" is your userid, and "server" is the server used for the course).  Note the lack of capitalization in your URL!  We are not going to go searching for your page, so if it is not at that URL, then you will get a zero.

The purpose of this part of the assignment is to complete the [CakePHP blog tutorial](http://book.cakephp.org/3.0/en/tutorials-and-examples/blog/blog.html).  An easier way to get CakePHP set up is to download the .zip of the latest release (as of the time of this writing, it's 3.0.0-beta1), and unzip it in a `~/html/cakephp/` directory.  For CakePHP version 3.0.0-beta1, the .zip file is [here](https://github.com/cakephp/cakephp/releases/download/3.0.0-beta1/cakephp-3-0-0-beta1.zip).  This allows you to effectively skip the "Getting Started" section of the tutorial.

Note that the CakePHP tutorial has a [part two](http://book.cakephp.org/3.0/en/tutorials-and-examples/blog/part-two.html) that must be completed as well.

A few notes:

- The directions here are written assuming that you are developing the system on your VirtualBox image, and then uploading that to the server once completed; if not, that's totally fine, but you may have to adapt them somewhat.
- You will want to unzip the CakePHP files into ~/html/cakephp/ on your local machine.  You can then view it via `http://localhost/~mst3k/cakephp/` (localhost means the machine that you are currently on).  You will see some warning messages and/or error messages, but proceeding through the [CakePHP getting started](cakephp-getting-started.html) ([md](cakephp-getting-started.md)) page should resolve all of them.  Once finished, the CakePHP splash page should show all green.
- Note that you can set a MySQL table *prefix* in config/app.php, such as `cake_`.  This will ensure that all of your CakePHP tables are not accidentally used by your other frameworks.

### Ruby on Rails

First, read through the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page.  The assumption is that you will test it locally via `rails server`, and the deploy it on the course server via the directions on that page.  If you want to try your hand configuring your *own* Rails server, you can look at the [Ruby on Rails deployment](rubyrails-deployment.html) ([md](rubyrails-deployment.md)) page -- but be warned, it's a real pain in the rear to configure a Rails server.

We are using Ruby version 2.1.2 and Rails version 4.1.5 for this assigment.  If you have a more recent patch level (the last of the three digits of the version number), that's fine.  But you can't have a different major version (the first of those three numbers) or a different minor version (the middle of those three numbers).

Your app *must* be called `railshw`, and it *must* be in your home directory.  In other words, there must be a `/home/slp/mst3k/railshw/public` directory, as this is what the webserver will be looking for.  If you change anything in that path, the web server will never find your app.  You can then view your app at `http://server/rails/mst3k`, where "server" is the course server.

This part of the assignment is to complete the blog tutorial [here](http://guides.rubyonrails.org/getting_started.html)

- That page makes some assumptions as to what is installed, which are all valid for both the VirtualBox image and pegasus
- To create the Rails application, you just enter `rails new railshw -d mysql` in your home directory (this is in section 3.2 of that web page; be sure to name it `railshw` so the URL is correct)

Lastly, since you will be using the same database for all three frameworks, you may want to have each Ruby table have given prefix.  You can do this by editing the three files in config/environments/ (the files are: development.rb, production.rb, and test.rb), and adding the following line to each (here, `ruby_` is the prefix, but use whatever you would like, within reason).  This line can go at the end, just before the last line (which is `end`).

```
config.active_record.table_name_prefix = "ruby_"
```

### Django

First, read through the [Django getting started](django-getting-started.html) ([md](django-getting-started.md)) page.  The assumption is that you will test it locally via `python manage.py runserver`, and the deploy it on the course server via the directions on that page.  If you want to try your hand configuring your *own* Apache server to run Django, you can look at the [Django deployment](django-deployment.html) ([md](django-deployment.md)) page -- but be warned, it's a real pain in the rear to configure a Django server (although not as bad as Rails).

Unlike Ruby on Rails, you can name the Django app anything you want, and put it in any directory that you want.  Although naming it `djangohw` and putting it in our home directory on the course server would not be bad ideas.

The task for the Django part of this homework is to go through all six parts of the introductory tutorial, found [here](https://docs.djangoproject.com/en/1.6/intro/) (the "Tutorial" line in the "First steps" section).  Make sure you are going through the 1.6 version!

When you have updated your Django app, remember to touch the wsgi file to make the web server reload your app; see the [Django getting started](django-getting-started.html) ([md](django-getting-started.md)) page for details.

It is much more difficult to rename your tables in Django -- you have to do it individually for each table.  But if the CakePHP tables all start with `cake_`, and the Ruby on Rails tables all start with `ruby_`, then you can leave the Django talbes to have the default name.

### Submission

None!  We have access to your files on the course server, and we can view your apps at the following URLs:

- CakePHP: `http://server/~mst3k/cakephp/`
- Ruby on Rails: `http://server/rails/mst3k`
- Django: `http://server/django/mst3k`

For any sites that have user authentication, you MUST allow user `kermit` with password `frog` to log in and see any and all features.

Whether your program is late or not will be determined by the timestamp on the files in your home directory on the course server.

SLP: Framework Homework
=======================

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

You will need to create a series of tables as part of this process - keep track of the MySQL commands that you issue, as you will have to repeat them on the server.  Alternatively, you can dump your entire MySQL database via `mysqldump -p mst3k > db.sql`.  Or you can do it via phpMyAdmin.  Either way, you should have a file that contains all the SQL commands to create the necessary database tables - we'll need that shortly.

### CakePHP

First, proceed through the [CakePHP getting started](cakephp-getting-started.html) ([md](cakephp-getting-started.md)) page -- this should get you a first CakePHP web page working.  Be sure you use the 3.0 API of CakePHP!  The URL for that page ***MUST*** be `http://server/~mst3k/cakephp/` (where "mst3k" is your userid, and "server" is the server used for the course).  Note the lack of capitalization in your URL!  We are not going to go searching for your page, so if it is not at that URL, then you will get a zero.

The purpose of this part of the assignment is to complete the [CakePHP blog tutorial](http://book.cakephp.org/3.0/en/tutorials-and-examples/blog/blog.html).

A few notes:

- The directions here are written assuming that you are developing the system on your VirtualBox image, and then uploading that to the server once completed; if not, that's totally fine, but you may have to adapt them somewhat.
- You will want to unzip the CakePHP files into ~/html/cakephp/ on your local machine.  You can then view it via `http://localhost/~mst3k/cakephp/` (localhost means the machine that you are currently on).  You will see some warning messages and/or error messages, but proceeding through the [CakePHP getting started](cakephp-getting-started.html) ([md](cakephp-getting-started.md)) page should resolve all of them.  Once finished, the CakePHP splash page should show all green.
- Note that you can set a MySQL table *prefix* in config/app.php, such as `cake_`.  This will ensure that all of your CakePHP tables are not accidentally used by your other frameworks.

------------------------------------------------------------

EVERYTHING BELOW THIS IS NOT YET READY
======================================

And it's likely to all change, so go away.

------------------------------------------------------------


### Ruby on Rails

First, read through the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page.  The assumption is that you will test it locally via `rails server`, and the deploy it on the course server via the directions on that page.  If you want to try your hand configuring your *own* Rails server, you can look at the [Ruby on Rails deployment](rubyrails-deployment.html) ([md](rubyrails-deployment.md)) page -- but be warned, it's a real pain in the rear to configure a Rails server.

We are using Ruby version 2.1.2 and Rails version 4.1.5 for this assigment.  If you have a more recent patch level (the last of the three digits of the version number), that's fine.  But you can't have a different major version (the first of those three numbers) or a different minor version (the middle of those three numbers).

Your app *must* be called `rubyrails`, and it *must* be in your home directory.  In other words, there must be a `/home/slp/mst3k/rubyrails/public` directory, as this is what the webserver will be looking for.  If you change anything in that path, the web server will never find your app.  You can then view your app at `http://server/rails/mst3k`, where "server" is the course server.

This part of the assignment is to complete the blog tutorial [here](http://guides.rubyonrails.org/getting_started.html)

- That page makes some assumptions as to what is installed, which are all valid for both the VirtualBox image and pegasus
- To create the Rails application, you just enter `rails new rubyrails -d mysql` in your home directory (this is in section 3.2 of that web page; be sure to name it `rubyrails` so the URL is correct)

Lastly, since you will be using the same database for all three frameworks, you may want to have each Ruby table have given prefix.  You can do this by editing the three files in config/environments/ (the files are: development.rb, production.rb, and test.rb), and adding the following line to each (here, `ruby_` is the prefix, but use whatever you would like, within reason).

```
config.active_record.table_name_prefix = "ruby_"
```

### Django

The directions for installing Django and writing your first site can be found [here](https://docs.djangoproject.com/en/1.6/intro/).  Note that this link is to the 1.6 version, as we will be using Django version 1.6.1 -- if you Google for that site, it will likely take you to the default 1.8 version page, which is not what we are looking for.

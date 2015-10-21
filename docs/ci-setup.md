SLP: Continuous Integration Testing: Getting Started
====================================================

[Go up to the main SLP documents page](index.html) ([md](index.md))

There are seven sections in this document, in four parts:

1. A common introductory part for everybody to read, which contains the intrudction and details about the database setup
2. A part with one section per development platform (CakePHP, Rails, or Django); you only need to read the one that pertains to the platform you are using
3. A part with one section for each of the two Continuous Integration testing services (we are using [Travis CI](http://travis-ci.com) and [CircleCI](http://circleci.com/)); you only need to read the one that pertains to the service you are using
4. A common conclusion part for everybody to read, which contains troubleshooting tips and how to proceed from here

You will need to read the introduction, the section on your platform, the section on your assigned CI service, and the conclusion.

Note that this document does not describe *how* to write unit tests, but it links to other documents for each platform that describe how to do that.  This document is concerned with getting the projects configured with the CI services.

----

Part 1: Common Information
==========================

Everybody needs to read this entire part.


Introduction
------------

This document will get you up and running using either of the two Continuous Integration (CI) testing services: [Travis CI](http://travis-ci.com) and [CircleCI](http://circleci.com/); your project will be assigned to one or the other.

The two CI servers work the same way: whenever someone pushes commits to Github, a notification is sent to the CI server.  The CI server will then clone the repo a [Linux Container](http://en.wikipedia.org/wiki/LXC), and run the tests.  This happens on *every* push.  While we are using two CI servers, we only have one process (container) per server.  This means that if two of the Travis groups push at the same time, one will be queued until the other one completes.  Likewise for Circle (and if you are ssh'ing into your Circle container, it blocks everything else until the container is destroyed).  As we progress through the year, this will become more noticeable, as your tests can easily take 30 minutes or so to complete.

You will configure your system to run (many) unit tests, all through a single command.  The CI service will then execute that command, and run all of the project's unit tests.

As you work through this document, you may run into unexpected issues.  The conclusion section, at the very end, has some troubleshooting tips.

#### Database Information

When a unit test is run, the database is cleared out, and only a limited amount of information -- just enough to run the test -- is loaded ino the DB.  This information is called a *fixture*.  The intent is to clear out the *test* database, not the production (or development) database.  One aspect of configuring your unit tests is pre-populating the database with the necessary information through fixtures.

It may be the case that you want to use a different type of database for the unit tests.  [SQLite](http://en.wikipedia.org/wiki/SQLite) is a database that stores all of the information in a file on the filesystem.  You can use the sqlite client (`sqlite3`) to enter basic SQL commands on that database.  One benefit is that you do not need a server, since the database is just a single file.  SQLite is good for small databases (which is what our test databases will be), but not good for large databases or databases in which the data will often change (which is what we need for our projects).

The configuration in this document assumes that you are using the MySQL test databases for the CakePHP and Rails projects, and that you are using SQLite for Django projects.  CakePHP and Rails projects are welcome to switch to SQLite, if that is preferred, but this document does not cover that switch.

In either case, the database configuration files in the repo should ***NOT*** contain the real passwords (it's okay if they contain the host ('localhost') or the username).  They can either have "fake" passwords (i.e., not the real ones that you are using on the course server), or they can have blank passwords.  One option that people use is to store a default database credentials file in the DB, and locally modify it after a clone; they then run `git update-index --assume-unchanged <file>` on the database credentials file, as that will prevent git from committing that change.  Regardless, the credentials that we will be using on Travis will be the ones stored in the repository.

**NOTE:** You must *FIRST* change the test database name in your database settings file (config/database.yml for Rails, config/app.php for CakePHP, and settings.py for Django).  What the tests do is they *wipe* the database, and completely rebuild it.  Thus, if your test database is the same name as your regular database, it will **ERASE** everything!  All the groups have a `project_test` database (with the same permissions) specifically for this purpose.

----

Part 2: Development Platforms
=============================

You only need to read the part that pertains to the platform you are using.


CakePHP
-------

***NOTE:*** The CakePHP installation comes with a .travis.yml script that is used to test the framework (i.e., not the code that *you* write).  If you push a commit with that file, it will kick off those tests, and those tests take about 40 minutes to complete.  Thus, if it is not already done for you, then you should rename that file to .travis.yml.orig (or something else, or delete it, etc.).

This tutorial assumes that you have completed the [CakePHP Blog Tutorial](http://book.cakephp.org/3.0/en/tutorials-and-examples/blog/blog.html) and the [CakePHP Blog Tutorial, part 2](http://book.cakephp.org/3.0/en/tutorials-and-examples/blog/part-two.html), which is what was done in the [Frameworks homework](hw-frameworks.html) ([md](framework-hw.md)).

In order to run these tests, you will have to install the `phpunit` package on your development host (`sudo apt-get install phpunit`); this was already done on the provided VirtualBox image.

**NOTE:** You must *FIRST* change the test database name in config/app.php.  What the tests do is they *wipe* the database, and completely rebuild it.  Thus, if your test database is the same name as your regular database, it will **ERASE** everything!  All the groups have a `project_test` database (with the same permissions) specifically for this purpose.

In the blog tutorial, you hand-entered (or cut-and-pasted) the contents for the model, views, and the controller.  But at this point you should consider [using Bake](http://book.cakephp.org/3.0/en/console-and-shells/code-generation-with-bake.html).  If you use Bake to create the MVC components, then it will also create (blank) tests for them.  Alternatively, you can have it just create the tests via the following commands.

```
bin/cake bake test Table Articles
bin/cake bake test Controller Articles
bin/cake bake fixture Articles
```

Performing these commands will get you started on testing, but they do not write test cases for you.  To run the tests, just enter `phpunit` from the root directory of your cloned repository.

After you bake the testing components from the tutorial, one of the tests will fail -- specifically, it will complain that there are no tests found in ArticlesTableTest; to fix this, put the following method in tests/Model/Table/ArticlesTableTest.php:

```
public function testFoo() {
  return true;
}
```

Now when you run `phpunit`, it will mention that a bunch of tests were incomplete, but none should have failed.  The incomplete tests are in the tests/TestCase/Controller/ArticlesControllerTest.php file, and they can be fixed later.

To get started on actually writing the tests, start working through the [CakePHP: Testing page](http://book.cakephp.org/3.0/en/development/testing.html).  This uses the code base created in the [CakePHP Blog Tutorial, part 2](http://book.cakephp.org/3.0/en/tutorials-and-examples/blog/part-two.html); thus, you will likely have to adapt the tests described for your specific project.  Once you get through the beginning of the "Running Tests" section, you can move on (you'll come back to the rest later).  A quick note about the testing tutorial: the ProgressHelper should go into src/View/Helper/ProgressHelper.php, it should extend `Helper` (not `AppHelper`), and it should have a `use Cake\View\Helper;` line after the `namespace` line.

The testing command (`phpunit`) is what we will be using on our Continuous Integration servers.  Assuming that works (i.e., successfully runs all the tests), then you are ready to move onto the CI service configuration.

Ruby on Rails
-------------

This tutorial assumes that you have completed the [Getting Started with Rails Tutorial](http://guides.rubyonrails.org/getting_started.html), which is what was done in the [Frameworks homework](hw-frameworks.html) ([md](framework-hw.md)).

**NOTE:** You must *FIRST* change the test database name in config/database.yml.  What the tests do is they *wipe* the database, and completely rebuild it.  Thus, if your test database is the same name as your regular database, it will **ERASE** everything!  All the groups have a `project_test` database (with the same permissions) specifically for this purpose.

To get started, start working through the [Guide to Testing Rails Applications](http://guides.rubyonrails.org/testing.html).  This uses the code base created in the [Getting Started with Rails Tutorial](http://guides.rubyonrails.org/getting_started.html); thus, you will likely have to adapt the tests described for your specific project.  Once you have written the first two tests, you can move onto the configuration with the CI tool.  This means you have to get through the end of section 3.2 (but ignore, for now, the error-producing test case at the very end of section 3.2); you'll come back to the rest later.  A few notes (these will make more sense once you have read through the testing tutorial):

- The test harnesses are created when you run `bin/rails generate model` and `bin/rails generate controller`
- Of the two tests that you have to work through in that tutorial, the first test always returns true, and the second test actually tests something.  That's fine for now.
- After you generate your scaffold, you will have to set up your test database; to do this, run `bin/rake db:migrate RAILS_ENV=test`
- Note that the command to kick off the tests is: `bin/rake test test/models/post_test.rb`; however, we are shortly going to use a different command.

If you run `bin/rake test test/models/post_test.rb`, you will notice (on the last line) that there are 2 runs and 2 assertions.  That command only runs one set of tests, but we want to run more.  To do that, we enter `rake test`, which runs *all* of the tests, including the two that we just entered.

It turns out that just running `rake` will do the same thing as `rake test` (`rake` is like `make`: if no parameters are given, it runs the first "target", which is "test").  To ensure that rake is run in the context of all of our gems (we don't want it accidentally using a wrong gem version), a better command is `bundle exec rake`: the `bundle exec` part ensures that the correct set of gems are loaded.

The testing command (`bundle exec rake`) is what we will be using on our Continuous Integration servers.  Assuming that works (i.e., successfully runs all the tests), then you are ready to move onto the CI service configuration.


Django
------

This tutorial assumes that you have completed the [Django Getting Started Tutorial](https://docs.djangoproject.com/en/1.6/intro/), which is what was done in the [Frameworks homework](hw-frameworks.html) ([md](framework-hw.md)).

While going through that [Django Getting Started Tutorial](https://docs.djangoproject.com/en/1.6/intro/), you went through writing tests for the Django app; this is described on [page 5 of the tutorial](https://docs.djangoproject.com/en/1.6/intro/tutorial05/).  We will make a few modifications from what is said on that page, but we will generally follow what they say.  Note that there were some problems and errors in what was presented in the tutorial with regard to testing; this is described in the Django section of the [Frameworks homework](hw-frameworks.html) ([md](framework-hw.md)).

Django does not seem to want to allow setting the test database to an arbitrary name; whatever you specify, it will insist on prepending `test_` to that name (presumably so we don't kill our production or development databse, but it would still be nice to allow for different naming conventions).  So we are going to use a [SQLite](http://en.wikipedia.org/wiki/SQLite) database, as described in the introduction section, above.  In order to change the database setup for the testing suite, we will use the following hack (adapted from [here](http://stackoverflow.com/questions/6353124/running-django-tests-with-sqlite)).  This should go in your settings.py file, right ***AFTER*** the `DATABASES` array.

```
import sys
if 'test' in sys.argv:
    DATABASES['default']['ENGINE'] = 'django.db.backends.sqlite3'
    DATABASES['default']['NAME'] = os.path.join(BASE_DIR, 'db.sqlite3')
```

Working through [page 5 of the tutorial](https://docs.djangoproject.com/en/1.6/intro/tutorial05/), and adapting those tests to be valid tests for your project, is enough to get you up and running on writing unit tests.  The full page on Django testing can be found [here](https://docs.djangoproject.com/en/1.6/topics/testing/).

The command to run the tests is `python manage.py test`, and that command is what we will be using on our Continuous Integration servers.  Assuming that works (i.e., successfully runs all the tests), then you are ready to move onto the CI service configuration.

----

Part 3: Continuous Integration Services
=======================================

You only need to read the one that pertains to the service you are using.

Travis CI
---------

Travis is a well-known service because it provides free testing for open source projects -- specifically, if it is a public repository on Github, then Travis will allow you to run tests on it for free.  To run tests on a private repository, you have to purchase a subscription.  To this end, they have two websites: [http://travis-ci.org](http://travis-ci.org) (notice the '.org' ending) is the website for open source builds (i.e., public repositories), and [http://travis-ci.com](http://travis-ci.com) (notice the '.com' ending) is the website for non-open source builds (i.e., private repositories).  Travis has generously donated a free subscription to this course; thus we can test our private repos.  This means that we will be using the '.com' version of their website.

You will need to sign in to [Travis](http://travis-ci.com) -- this is done via authorizing Travis to authenticate you via Github (when you click "sign in" on [Travis](http://travis-ci.com), it guides you through that process).  One you have logged in, you will see any and all repositories that you have access to.  In addition to your project repository, you will see three example repositories for configuring Travis with the three platforms in the course; those repos are [rails-test](https://github.com/uva-slp/rails-test), [cake3-test](https://github.com/uva-slp/cake3-test), and [django-test](https://github.com/uva-slp/django-test).  These are described in the Troubleshooting section of the Conclusions, below.  The specific URL for the testing of your project repo will be [https://magnum.travis-ci.com/uva-slp/project](https://magnum.travis-ci.com/uva-slp/project) where "project" is your project tag (i.e., the same name as your github repo).

If your project was assigned to Travis, then Travis has already been configured to work with your repository.  Every time commits are pushed to the repo, then Travis will look for a .travis.yml file (described next) -- if it finds one, it will run tests; if it doesn't, then it will do nothing.  Thus, as soon as you push a commit with a .travis.yml file, Travis will go to work.

#### Configuration

To configure Travis, you need to create a .travis.yml file in the root directory of your repository.  This [YAML](http://en.wikipedia.org/wiki/YAML) file is plain text, and tells Travis the details about your project: what language you are using (and what version), the database configuration, etc.

Here is a sample .travis.yml file for Rails apps:

```
language: ruby

rvm:
  - 2.2.3

before_script:
  - "mysql -u root -e 'create database asb2t_test;'"
  - "mysql -u root -e \"grant all on asb2t_test.* to 'asb2t';\""

sudo: false
```

Here is a sample .travis.yml file for CakePHP apps:

```
language: php

php:
  - 5.5

before_script:
  - /home/travis/.phpenv/versions/5.5/bin/composer self-update
  - sh -c "composer require 'cakephp/cakephp-codesniffer:dev-master'"
  - "mysql -u root -e 'create database asb2t_test;'"
  - "mysql -u root -e \"grant all on asb2t_test.* to 'asb2t';\""

sudo: false
```

Here is a sample .travis.yml for Django apps:

```
language: python

python:
  - "2.7"

install:
  - pip install -q Django==1.8.4

script: python manage.py test

sudo: false
```

Let's take each part one at a time.

**Language**

The `language` stanza is where you specify which language your code is using.  You only pick one of these stanzas, depending on your platform language.

```
language: php
```
```
language: python
```
```
language: ruby
```

This is pretty self-explanitory about what this does.  The choices for us are PHP, Python, and Ruby, depending on what platform you are using.  Travis supports [many other languages](http://docs.travis-ci.com/user/getting-started/) as well.  In addition, each language has it's own page that lists the configuration options: [PHP](http://docs.travis-ci.com/user/languages/php/), [Python](http://docs.travis-ci.com/user/languages/python/), and [Ruby](http://docs.travis-ci.com/user/languages/ruby/).  Obviously, you only pick one of these lines.  The next stanza will be specific to your language as well.

**Language configuration**

You only pick one of these stanzas, depending on your platform language.

```
php:
  - 5.5
```
While we are using PHP version 5.5.9, only the major and minor versions (i.e., 5.5) are specifed; the patch level (i.e., the ".9") is not specified.

```
rvm:
  - 2.2.3
```

[RVM](http://rvm.io/) is the Ruby Version Manager, which is a tool to manage Ruby and Rails versions.  We are using [rbenv](https://github.com/sstephenson/rbenv) (Ruby Environment) on the course server, but they both accomplish essentially the same thing.  The second line in this stanza specifies the Ruby version that we want to use (2.2.3).  Note that the Rails version is specified in our Gemfile, so we do not specify it here.

```
python:
  - "2.7"
```

This specifies the Python versionl the Django version is specified later.  While we are using Python version 2.7.6, only the major and minor versions (i.e., 2.7) are specifed; the patch level (i.e., the ".6") is not specified.  Note that, unlike the other languages, the Python version is enclosed in double quotes.

**Installing necessary software**

Travis will configure the necessary version of the language, and install any obvious dependencies (standard PHP and Python libraries, run `bundle install` for Rails, etc.).  However, your system might need additional software to be installed.

For CakePHP, there are two things that need to be done, which are done in the following stanza.  All CakePHP programs will need this stanza.

```
before_script:
  - /home/travis/.phpenv/versions/5.5/bin/composer self-update
  - sh -c "composer require 'cakephp/cakephp-codesniffer:dev-master'"
```

The first thing is to update composer to the latest version (not strictly required, but it will give you lots of warnings if you don't do that); this is done via the `self-update` command.  The second thing is to install all the dependencies, which is what the second of those two commands (the `composer require` one) does.  Note that there are two database commands that are also included in the `before_script` section; these are described below.

If you have any additional dependencies that were installed with pip (or other commands), you would specify them as such:

```
install:
  - "pip install ..."
```

In particular, the Django projects need to install Django; we have to specify which version when we install it.  Version 1.8.4 is what this course is using.

```
install:
  - pip install -q Django==1.8.4
```

Note that, if we were testing against multiple versions of Django, or if we wanted to know that version in our testing scripts, we could do the previous commands by setting (and then using) an environment variable:

```
env:
  - DJANGO_VERSION=1.8.4
install:
  - pip install -q Django==$DJANGO_VERSION
```

One can install packages as well via the `sudo` command.  However, the `sudo: false` option in the Travis configuration file currently prevents this (this option can be removed, but issues arrise if it is).  Most packages are installed via the language-specific commands (`bundle install` for Ruby, `pip` for Python, and `composer` for PHP).  Thus, it is not expected that any additional packages will need to be installed that way, so this functionality is not covered here.

**Database setup**

The following sets up the necessary database. Note that there are two composer commands that are also included in the `before_script` section; these are described above.

```
before_script:
  - "mysql -u root -e 'create database asb2t_test;'"
  - "mysql -u root -e \"grant all on asb2t_test.* to 'asb2t';\""
```

This is the database setup.  The database credentials in the repo specify that the database to be used is `asb2t_test`, and that the `asb2t` user should be able to access that database.  Note that we do not require a password for the MySQL user here.  You obviously will need to adapt that to your particular setup -- in particular, your database is not "asb2t".  Because this is in the `before_script` stanza, that setup is created prior to running the tests (hence, the "before" part of "before_script").  Note that the syntax here is very precise, especially with regards to all those quotes.  Travis has [more information on database setup](http://docs.travis-ci.com/user/database-setup/), including a section specifically on [MySQL](http://docs.travis-ci.com/user/database-setup/#MySQL).

If your user needs a password, then you will have change the `grant` command to something like the the following:

```
  - "mysql -u root -e \"grant all on asb2t_test.* to 'asb2t' identified by 'password';\""
```

Note that the Django apps use a sqlite DB for testing -- partly because it's faster in their case, and partly because Django doesn't like using a custom named DB.  This means that the Django scripts do not need this stanza to set up the database.

**Testing command**

Travis makes some assumptions about what command to run in order to test the code base.  For Ruby, it assumes `bundle exec rake`; for PHP, it assumes `phpunit`; for Django, it assumes incorrectly, and we have to tell it how to correctly run the tests.  If you are using the default commands (for Cake and Rails), then you don't have to specify which command to use.

Django needs to specify the testing command, as Travis does not figure it out.  This is done as follows.

```
script: python manage.py test
```

If you have multiple test commands, you have to write a separate script to run each of the invidiaul testing commands.  The catch is that for *each* of the separate testing calls in your script, you have to catch the error code, and then return non-zero if ANY of them fail (note that returning 0 means success, and returning non-zero means failure).

**Additional configuration**

Travis allows a very large number of configuration options for your build; you can view [the documentation for all of them online](http://docs.travis-ci.com/).  The only other one discussed here is notifications.  By default, Travis will send an email if either (1) a build is broken, or (2) a previously broken build was just fixed.  Those emails are sent to the committer and the commit author (typically the are the same person).  This can be changed through the [Travis Notifications](http://docs.travis-ci.com/user/notifications/) section.

#### Final Steps

When you view your repo on Travis, there is a "build|passing" image on the upper-right (or "build|failing", but we hope it's passing).  This image is called a "status badge".  You should put this in your repo's readme.md file at the top.  To do this, click on that image in Travis, select Markdown, and it will give you the Markdown to cut-and-paste into your project's readme.md file.  This image also serves as a link to the Travis page for your repo.

CircleCI
--------

[CircleCI](https://circleci.com/) is a well-known testing service that runs a large number of platforms.  They only provide testing for users who have paid subscription, and thus are generally used for private repositories.  Circle has generously donated a free subscription to this course; thus we can test our private repos.

You will need to sign in to [Circle](http://circleci.com) -- this is done via authorizing Circle as a Github application (when you click "sign in" on [Circle](http://circleci.com), it guides you through that process).  One you have logged in, you will see any and all repositories that you have access to.  In addition to your project repository, you will see three example repositories for configuring Circle with the three platforms in the course; those repos are [rails-test](https://github.com/uva-slp/rails-test), [cake3-test](https://github.com/uva-slp/cake3-test), and [django-test](https://github.com/uva-slp/django-test).  These are described in the Troubleshooting section of the Conclusions, below.  The specific URL for the testing of your project repo will be [https://circleci.com/gh/uva-slp/project](https://circleci.com/gh/uva-slp/project) where "project" is your project tag (i.e., the same name as your github repo).

If your project was assigned to Circle, then Circle has already been configured to work with your repository. Every time commits are pushed to the repo, then Circle will look for a circle.yml file (described next) -- if it finds one, it will run tests; if it doesn't, then it will do nothing. Thus, as soon as you push a commit with a circle.yml file, Circle will go to work.

#### Configuration

To configure Circle, you need to create a circle.yml file in the root directory of your repository.  This [YAML](http://en.wikipedia.org/wiki/YAML) file is plain text, and tells Circle the details about your project: what language you are using (and what version), the database configuration, etc.

Here is a sample circle.yml file for Rails apps:

```
machine:
  ruby:
    version: 2.2.3

dependencies:
  pre:
    - "mysql -u root -e 'create database asb2t_test;'"
    - "mysql -u root -e \"grant all on asb2t_test.* to 'asb2t';\""
    - "/bin/rm -rf /home/ubuntu/project/vendor/bundle/ruby/2.1.0/specifications"
```

Here is a sample circle.yml file for CakePHP apps:

```
machine:
  php:
    version: 5.5.8

dependencies:
  pre:
    - "mysql -u root -e 'create database asb2t_test;'"
    - "mysql -u root -e \"grant all on asb2t_test.* to 'asb2t';\""
    - "ln -s /var/run/mysqld/mysqld.sock /tmp/mysql.sock"
```

Here is a sample circle.yml for Django apps:

```
machine:
  python:
    version: 2.7.6

dependencies:
  pre:
    - pip install -q Django==1.8.4
```

Let's take each part one at a time.

**Machine**

The `machine` stanza is where you specify the language and language version for your code.  You only pick one of these stanzas, depending on your platform language.

```
machine:
  php:
    version: 5.5.8
```
```
machine:
  python:
    version: 2.7.6
```
```
machine:
  ruby:
    version: 2.2.3
```

This is pretty self-explanitory about what this does.  The choices for us are PHP, Python, and Ruby, depending on what platform you are using.  Circle supports [many other languages](https://circleci.com/docs/environment) as well.  While there are other options that can be specified in the machine stanza, we won't need to use any of them now.

**Dependencies**

There are many types of dependencies that you can put into a circle.yml file, but we are only going to focus here on *pre* dependencies, which means it is done before anything else (including before the `bundle install` command for Rails apps).

All of the remaining commands described in this section are meant to be in the dependencies/pre section of the circle.yml file.  There is a common section on databases, followed by a section on each platform.

**Database setup**

First is the setup of the database:

```
- "mysql -u root -e 'create database asb2t_test;'"
- "mysql -u root -e \"grant all on asb2t_test.* to 'asb2t';\""
```

This is the database setup.  The database credentials in the repo specify that the database to be used is `asb2t_test`, and that the `asb2t` user should be able to access that database.  Note that we do not require a password for the MySQL user here.  You obviously will need to adapt that to your particular setup.  Because this is a pre-dependency, that setup is created prior to running the tests.  Note that the syntax here is very precise, especially with regards to all those quotes.  Circle has [more information on database setuphttps://circleci.com/docs/environment#databases).

If your user needs a password, then you will have change the `grant` command to something like the the following:

```
  - "mysql -u root -e \"grant all on asb2t_test.* to 'asb2t' identified by 'password';\""
```

Note that the Django apps use a sqlite DB for testing -- partly because it's faster in their case, and partly because Django doesn't like using a custom named DB.  This means that the Django scripts do not need these options to set up the database.

**Rails specific**

All Rails apps need to have this line in the pre-dependencies section:

```
- "/bin/rm -rf /home/ubuntu/project/vendor/bundle/ruby/2.1.0/specifications"
```

Recall that on the course server, we are installing the gems locally (recall that the command is `bundle install --path vendor/bundle` -- the `--path vendor/bundle` part tells the `bundle` command to install them locally instead of system-wide).  This causes problems for Circle, as it is not sure how to manage the versions that are installed locally versus the versions installed system-wide.

To solve this, we remove all of the locally installed gems, and the just reinstall everything; the reinstallation is system-wide on Circle, but that doesn't really matter for us.  While this takes a bit longer (say, 10 more seconds), it also allows it to work properly.  Note that the gems are installed via `bundle install`, which Circle will automatically run for us after the pre-dependencies.

***NOTE:*** you have to replace `project` in that above command with your project's name (i.e., the github repo name)!  Otherwise it will not delete the files.

**Cake specific**

All CakePHP apps need have this line in the pre-dependencies section:

```
- "ln -s /var/run/mysqld/mysqld.sock /tmp/mysql.sock"
```

The MySQL socket file is the file that all mysql clients access in order to connect to the database; this includes the PHP mysql routines.  PHP expects the socket file to be /var/run/mysqld/mysqld.sock, which is what it is on both the course server and on the VirtualBox image.  But on the Circle container, it is /tmp/mysql.sock, and PHP can't seem to figure this out.  So we create a [*symlink*](http://en.wikipedia.org/wiki/Symbolic_link) to link one to the other.  Then, even though PHP is looking for /var/run/mysqld/mysqld.sock, it will find /tmp/mysql.sock through the symlink.

**Django specific**

All Django apps need to have this line in the pre-dependencies section:

```
- pip install -q Django==1.8.4
```

We have to install the right version of Django, and this is done through pip, as described below.

**Installing necessary software**

Circle will configure the necessary version of the language, and install any obvious dependencies (standard PHP and Python libraries, run `bundle install` for Rails, etc.).  However, your system might need additional software to be installed.

If you have any additional dependencies that were installed with pip (or other commands), you would specify them as such:

```
install:
  - "pip install ..."
```

In particular, the Django projects need to install Django; we have to specify which version when we install it.  Version 1.8.4 is what this course is using.

```
install:
  - pip install -q Django==1.8.4
```

One can install packages as well via the `sudo` command.  Most packages are installed via the language-specific commands (`bundle install` for Ruby, `pip` for Python, and `composer` for PHP).  Thus, it is not expected that any additional packages will need to be installed that way, so this functionality is not covered here.

**Additional configuration**

Circle allows a very large number of configuration options for your build; you can view [the documentation for all of them online](https://circleci.com/docs).  The only other one discussed here is notifications.  By default, Circle will send an email if either (1) a build is broken, or (2) a previously broken build was just fixed.  Those emails are sent to the committer and the commit author (typically the are the same person).  This can be changed through the [Circle Notifications](https://circleci.com/docs/configuration#notify) section.

#### Final Steps

When you view your repo on Circle, there is a status indicator that indicates if it is passing or not (we hope it's passing).  You can also create a "status badge", which is an image that indicates if it is passing or not.  You should put this in your repo's readme.md file at the top.  To do this, go to the setings for your repo in Circle (the gear icon in the upper-right), and then click on Status Badges. You will have to create an API token for this; create one of the 'Status' type.  The Status Badges part of the options will generate the Markdown needed, and you can cut-and-paste that into your project's readme.md file.  Note that the default status badge is of the 'badge' type, and we are using the 'shield' type -- to change this, in the image URL provided, change `style=badge` to `style=shield`.  This image also serves as a link to the Circle page for your repo.

----

Part 4: Common conclusions
==========================

Troubleshooting
---------------

Did something not work correctly?  If so, here is where to find the solutions.

There are three repos that where all this configuration was performed for each language / CI tool combination.  You can look at those repos for help.  In particular, the commits take very specific steps, so seeing the changes from one commit to another may help (you can do this easily through github's interface).  Those repos are: [rails-test](https://github.com/uva-slp/rails-test), [cake3-test](https://github.com/uva-slp/cake3-test), and [django-test](https://github.com/uva-slp/django-test).  Each of those repos are configured with *both* Travis and Circle.  All of those repos start with the default site, run through the tutorials described in the [Frameworks homework](hw-frameworks.html) ([md](framework-hw.md)), and then proceed to configure the CI tests.  The .travis.yml and circle.yml files in those repos are the sample files shown above in this document.

What you must do
----------------

You have to do the following tasks:

1. Ensure that no database passwords are kept in the github repo
2. Configure your project to properly run some unit tests (just running 2 is fine at this point; you'll write more in step (5), below)
3. Configure your project to properly run on Travis or Circle, as per your assignment; it must run the tests successfully
4. Have the status badge for your project's build shown at the *top* of your project's readme.md file, which will cause it to display on github when you view the project; for Circle projects, this needs to be the `shield` type, and not the `badge` type
    - This is shown in the sample repos: [rails-test](https://github.com/uva-slp/rails-test), [cake3-test](https://github.com/uva-slp/cake3-test), and [django-test](https://github.com/uva-slp/django-test).
5. Write a number of unit tests; these tests are beyond what was done in (2).  Specifically:
    - Each *person* in the group must write at least 5 (five!) unit tests; you must commit your own unit tests to the repo
    - Each person in the group must write at least one fixture, which is used in some/all of their unit tests; you must also commit your own fixture (need not be a separate commit)
    - These unit tests must be actually test something, and it must be a valid unit test
    - They must all succeed (if they don't, then fix the code to make them succeed)
6. Ensure that all the unit tests written by the group (at least 5 times the group size) run successfully on the CI service

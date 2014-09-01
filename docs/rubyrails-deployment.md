SLP: Ruby on Rails: Deployment
==============================

This page is for how to *deploy* your Ruby on Rails apps on an Ubuntu server.  This is not needed for your local development, as you can run `rails server` to test it out locally, or test it out on the course server provided.  However, if you ***do*** want to get it running through Apache, read on...

Ruby on Rails is run through Apache via a module called Phusion Passenger (or just "Passenger").  It is that module that we have to install and configure.

**NOTE:** By following the directions on the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page, the paths and ruby versions are set to custom values.  The instructions here were NOT run by that user, and I don't know what would happen if they were.

Installing Passenger
--------------------

First, we need to install Passenger.  Following the directions [here](https://www.phusionpassenger.com/documentation/Users guide Apache.html), specifically [section 2.3](https://www.phusionpassenger.com/documentation/Users%20guide%20Apache.html#install_on_debian_ubuntu), I ran the following commands:

1. Run the following commands:
```
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 561F9B9CAC40B2F7
sudo apt-get install apt-transport-https ca-certificates
```
    It may well be the case that the second of those two commands does not install any new packages.
2. Edit /etc/apt/sources.list.d/passenger.list (a new file), and add the following line:
```
deb https://oss-binaries.phusionpassenger.com/apt/passenger trusty main
```
    If you are using a version of Ubuntu that is *not* 14.04, see the above URL for the different repository to use.
3. Make sure that the passenger.list file you created is owned by root (I didn't bother with the chmod command they stated)
4. Update apt-get: `sudo apt-get update`
5. Install the package: `sudo apt-get install libapache2-mod-passenger`.  This installs passenger version 4.0.50, and it will install a bunch of other packages (16 or so).  However, this installs ruby versions 1.9.3 and 2.0.0, which are not the versions we want to use.

Install Ruby 2.1
----------------

As the directions in the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page use Ruby version 2.1, we want to do that also.  Following the directions [here](http://brightbox.com/blog/2014/01/09/ruby-2-1-ubuntu-packages/) and [here](http://askubuntu.com/questions/452243/what-versions-of-ruby-are-supported-in-14-04):

1. Add a new repository: `sudo add-apt-repository ppa:brightbox/ruby-ng`
2. Update apt-get: `apt-get update`
3. Install Ruby 2.1: `apt-get install ruby2.1`; this will likely on need to install 2 packages.

At this point, running `/usr/bin/ruby -v` should report version 2.1.2 (possibly with a more recent patch level).  This matches the Ruby version in the getting started page.

Configure Passenger
-------------------

1. Check /etc/apache2/mods-available/passenger.conf.  The PassengerDefaultRuby line should be using /usr/bin/ruby, which means (assuming the previous section was completed successfully) that Passenger is using Ruby 2.1.2.
2. Ensure passenger is enabled for apache: `sudo a2enmod passenger`.  We will restart the web server shortly.
3. Create a new Ruby app in a local direction via `rails new myapp`, or similar; see the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page for how to configure Ruby and Rails in a user directory.
4. Edit your /etc/apache2/sites-available/000-default.conf file, and add the following lines:
```
    Alias /railstest /home/rails/myapp/public
    <Location /railstest>
        PassengerBaseURI /railstest
        PassengerAppRoot /home/rails/myapp
    </Location>
    <Directory /home/rails/myapp/public>
        Allow from all
        Options -MultiViews
        # Uncomment this if you're on Apache >= 2.4:                                                                                                      Require all granted
    </Directory>
```
5. In /etc/apache2/sites-available/passenger.conf, add `PassengerDefaultUser www-data` as the last line inside the IfModule clause.
6. Restart apache: `sudo service apache2 restart`
7. View the app at http://localhost/railstest.  While it should show a Passenger page, at this point, it should complain about being unable to run bundler/setup (specifically, "cannot load such file -- bundler/setup (LoadError)").
8. Run `sudo gem install bundler`
9. Reload http://localhost/railstest`.  Now it should complain about not *having* the gems installed, which is a different issue.

The problem, now, is that all of the gems were installed by the local user via `bundle install`, which just finds the gems that are needed to be installed.  However, the Ruby on Rails that is running on the Apache web server (via Passenger) does not use those gems -- instead, it uses the gems in /usr/lib/ruby/gems/2.1.0/ -- and the particular gems needed for that web app have not (yet) been installed.

You can install gems one-by-one.  While this is rather tedious, it will work.  In the passenger error page, about 8 lines down, it will say something like, "Could not find rake-10.3.2 in any of the sources (Bundler::GemNotFound)".  That says that the gem that is needed is "rake" and the version is "10.3.2".  So we can install that particular gem via `sudo gem install rake -v 10.3.2`.

Alternatively, you can cd into the Rails app directory (which, in the example above, was /home/rails/myapp), and run `bundle install` as root.

Some packages may complain about not finding /usr/lib/ruby/include/ruby.h -- see the troubleshooting section, below, for how to fix this.

Todo
----

- It's currently getting an internal server error: "Missing 'secret_key_base' for 'production' environment, set this value in 'config/secrets.yml'"
- Go over how to set it up for many users in the apache2 config files
- How to easily allow them to install their gems?


Troubleshooting
---------------

### /usr/lib/ruby/include/ruby.h not found

In either case, you may run into a problem when trying to install certain gems -- this happened with the mysql2 gem:

```
gem install mysql2 -v 0.3.16
Building native extensions.  This could take a while...
ERROR:  Error installing mysql2:
        ERROR: Failed to build gem native extension.

    /usr/bin/ruby2.1 extconf.rb
mkmf.rb can't find header files for ruby at /usr/lib/ruby/include/ruby.h

extconf failed, exit code 1

Gem files will remain installed in /var/lib/gems/2.1.0/gems/mysql2-0.3.16 for inspection.
Results logged to /var/lib/gems/2.1.0/extensions/x86_64-linux/2.1.0/mysql2-0.3.16/gem_make.out
```

The Ruby development files (and, specifically, /usr/lib/ruby/include/ruby.h), were not installed.  To do so, run `sudo apt-get install ruby2.1-dev`.  The "2.1" will install the right set of packages from the repository added earlier in these instructions.

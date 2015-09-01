SLP: Ruby on Rails: Deployment
==============================

[Go up to the main SLP documents page](index.html) ([md](index.md))

This page is for how to *deploy* your Ruby on Rails apps on an Ubuntu server -- specifically, Ubuntu 14.04.  This is not needed for your local development, as you can run `rails server` to test it out locally, or test it out on the course server provided.  However, if you ***do*** want to get it running through Apache, read on...

Ruby on Rails is run through Apache via a module called Phusion Passenger (or just "Passenger").  It is that module that must be installed and configured.

**NOTE:** By following the directions on the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page, the paths and ruby versions are set to custom values within the user's directory.  The instructions here were NOT run by that user, and I don't know what would happen if they were.

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
5. Install the package: `sudo apt-get install libapache2-mod-passenger`.  This installs passenger version 4.0.50, and it will install a bunch of other packages (16 or so).  However, this installs ruby versions 1.9.3 and 2.0.0, which are not the versions we want to use.  That leads us to...

Install Ruby 2.2
----------------

As the directions in the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page use Ruby version 2.2, we want to do that also.  Following the directions [here](http://brightbox.com/blog/2014/01/09/ruby-2-1-ubuntu-packages/) and [here](http://askubuntu.com/questions/452243/what-versions-of-ruby-are-supported-in-14-04):

1. Add a new repository: `sudo add-apt-repository ppa:brightbox/ruby-ng`
2. Update apt-get: `sudo apt-get update`
3. Install Ruby 2.2: `sudo apt-get install ruby2.2`; this will likely on need to install 2 packages.
4. Ensure that the /usr/bin/ruby symlink is pointing to the proper version of Ruby: `update-alternatives --set ruby /usr/bin/ruby2.2`.  Also update gem: `update-alternatives --set gem /usr/bin/gem2.2`.  Also update gem: 

At this point, running `/usr/bin/ruby -v` should report version 2.2.3 (possibly with a more recent patch level).  This matches the Ruby version in the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page.

Configure Passenger
-------------------

1. Check /etc/apache2/mods-available/passenger.conf.  The `PassengerDefaultRuby` line should be using `/usr/bin/ruby`, which means (assuming the previous section was completed successfully) that Passenger is using Ruby 2.2.3.
2. In /etc/apache2/sites-available/passenger.conf, add `PassengerDefaultUser www-data` as the last line inside the `<IfModule>` clause.
3. Ensure passenger is enabled for apache: `sudo a2enmod passenger` (it might already be enabled).  We will restart the web server shortly.
4. As a user that can run sudo, create a new Ruby app in a local directory via `rails new myapp`, or similar; see the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page for how to configure Ruby and Rails in a user directory.  You will need to configure the database and set the secret\_key\_base values.
5. Edit your /etc/apache2/sites-available/000-default.conf file, and add the following lines.  Here, `/home/rails/myapp` is where the Rails app is installed on the local file system, and `/rails/railstest` is the URL for the Rails app (specifically, it will be at http://server/rails/railstest).  In this example, all the rails apps are kept in a `rails/` directory (and that directory was in the HTML document root, which is likely `/var/www/html/`), but that need not be the case.  The lines to add to 000-default.conf:
```
RailsEnv development
RailsBaseURI /rails/railstest
<Directory /home/rails/myapp>
  Options -MultiViews
</Directory>
```
6. In the HTML document root (likely `/var/www/html`), make that `rails` sub-directory.  In that directory, run `ln -s /home/rails/myapp/public railstest`, where `/home/rails/myapp` and `railstest` are the values from step 5, above.
7. Reload apache: `sudo service apache2 reload`
8. View the app at `http://localhost/rails/railstest`.  While it should show a Passenger page, at this point, it should complain about being unable to run bundler/setup (specifically, "cannot load such file -- bundler/setup (LoadError)").
8. Run `sudo /usr/bin/gem install bundler` (this will only work correctly if you updated the gem version in the previous section via `update-alternatives`)
9. Reload `http://localhost/rails/railstest`.  Now it should complain about not *having* the gems installed ("It looks like Bundler could not find a gem"), which is a different issue.  This is progress!

The problem, now, is that all of the gems were installed by the local user via `bundle install` (or, more likely, `bundle install --path vendor/bundle`), whcih installs them to the user's own directory.  However, the Ruby on Rails that is running on the Apache web server (via Passenger) does not use those gems -- instead, it uses the gems in `/var/lib/gems/2.2.0/gems` -- and the particular gems needed for that web app have not (yet) been installed.

A few hard ways to manage this before we get to the easy way:

- You can install gems one-by-one.  While this is rather tedious, it will work.  In the passenger error page, about 8 lines down, it will say something like, "Could not find rake-10.3.2 in any of the sources (Bundler::GemNotFound)".  That says that the gem that is needed is "rake" and the version is "10.3.2".  So we can install that particular gem via `sudo gem install rake -v 10.3.2`.
- Alternatively, you can cd into the Rails app directory (which, in the example above, was /home/rails/myapp), and run `bundle install` as root.

The easist way is to have the users run `bundle install --path vendor/bundle` instead of `bundle install`.  This will install the gems into their rails app directory (specifically, `myapp/vendor/bundle/ruby/2.2.0/gems/`).  Passenger will look there for the gems, in addition to the system directory (which is `/var/lib/gems/2.2.0/gems`).  Having them install the gems locally will take up more disk space (maybe 50-100 Mb per user), but will be far easier, as the students can manage their own gems without sudo privledges.

Some packages may complain about not finding /usr/lib/ruby/include/ruby.h -- see the troubleshooting section, below, for how to fix this.

Allowing users to reload apache
--------------------------------

Reloading apache needs to be run as root.  We will allow all users of a given group to execute these functions; we'll call the group 'rails'.

Compile this file, and call it `reload-apache2`:

```
#include <stdlib.h>
int main() {
  system("service apache2 reload");
  return 0;
}
```

Take that binaries (`reload-apache2`), and move it to /usr/local/bin/.  The run `chown root:rails reload-apache2` and `chown 4750 reload-apache2`.  These commands mean that anybody who runs that binary (which, in this case, can only be done by somebody in the rails group) will execute those commands AS ROOT.

You can add a user to a group via `usermod -a -G rails user`, but that user will have to completely log out and log back in for that change to take effect.

Deploying multiple Rails apps on one server
-------------------------------------------

Some of these steps were done in the Passenger configuration section, above.

1. In the HTML document root (which is specified in /etc/apache2/sites-enabled/000-default.conf), create a `rails/` directory (this was the same as discussed in steps 5 and 6 in the Configure Passenger section, above).
2. In that directory, make symlinks to the public folders of each of the apps you want to have on the server: `ln -s ~userid/myapp/public user`
3. Edit /etc/apache2/sites-available/000-default.conf
    - Add all of this right above the very last `</VirtualHost>` line
	- Add the line: `RailsEnv development`
	- For ***each*** rails app to deploy, add the following lines:
```
RailsBaseURI /rails/user
<Directory /home/user/myapp>
  Options -MultiViews
</Directory>
```
	- Here, `/rails/user` is the symlink (from the apache2 document root done in step 2, above -- which means the full path to that symlink is likely something like `/var/www/html/rails/user`), and `/home/user/myapp` is where the app itself is stored
	- Alternatively, you can put all of those 4-line stanzas in a separate file, such as `/etc/apache2/rails.conf`, and then put in the following two lines into `/etc/apache2/sites-available/000-default.conf` (and, presumably, `/etc/apache2/sites-available/default-ssl.conf`):
```
RailsEnv development
Include rails.conf
```	
4. Reload the web server (`service apache2 reload`)
5. At this point, you should be able to view your app at `http://server/rails/user` (and possibly the https version of the URL, if that was configured).
6. If you swtich from development to production in step 3, the app claims it cannot find the file; see the troubleshooting section, below, for details.


Easy user Rails installation
----------------------------

This section is intended for those running a server that many users will be setting up Rails apps.  If you are just running Rails apps under one account, then this section doesn't apply to you.

To set up Rails, as per the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page, you have to follow the directions that are listed [here](https://gorails.com/setup/ubuntu/14.04).  These instructions take about 30 minutes to run, and if anything is done wrong, it will likely mean they have to start over.

Instead, you can run those directions from a given user's directory (user 'rails', for example).  It will be easiest if you actually create an app for that user (and use the `-d mysql` flag when doing so), as that will locally install the gems needed.  Then you can tar/gzip the files.  To do so, you need to include a number of directories and files:

```
tar cfz rails-setup.tgz .gem .gemrc .rbenv .subversion bashrc-mods.txt
```

The first four of the included files/directories (.gem, .gemrc, rbenv, and .subversion) were created by the instructions [here](https://gorails.com/setup/ubuntu/14.04).  The last one lists the three lines that the users have to add to their end of their .bashrc.  Those modifications were also created by [those instructions](https://gorails.com/setup/ubuntu/14.04).

The [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page indicates that this file is kept in `/usr/local/`, and it is how it instructs students to set up Rails in their account.

A few important notes:

- zip/unzip seemed to cause some problems, likely with how symlinks are handled.  Or it could have been a user error, but either way, tar/gzip works fine.
- That file is likely not portable to other machines unless the configuration is the exact same.
- **You will still have to keep the rails account**.  Many of the paths in the files that you have created via the `tar` command have hard-coded paths to the `rails` user.  The gems are still installed locally (via `bundle install --path vendor/bundle`).

Troubleshooting
---------------

### Generic error with no known cause

Rails apps have a log/ directory (such as /home/rails/myapp/log), and therein provide a lot of detail about what is going wrong.

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

Gem files will remain installed in /var/lib/gems/2.2.0/gems/mysql2-0.3.16 for inspection.
Results logged to /var/lib/gems/2.2.0/extensions/x86_64-linux/2.2.0/mysql2-0.3.16/gem_make.out
```

The Ruby development files (and, specifically, /usr/lib/ruby/include/ruby.h), were not installed.  To do so, run `sudo apt-get install ruby2.1-dev`.  The "2.1" will install the right set of packages from the repository added earlier in these instructions.

### "The page you were looking for doesn't exist"

This problem seems to occur when running the Rails apps in production mode.  For the reason why (and clues how to fix it), see the answer [here](http://stackoverflow.com/questions/22026492/heroku-rails-setup-the-page-you-were-looking-for-doesnt-exist)

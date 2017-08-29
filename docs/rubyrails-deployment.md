SLP: Ruby on Rails: Deployment
==============================

[Go up to the main SLP documents page](index.html) ([md](index.md))

### Introduction

This page is for how to *deploy* your Ruby on Rails apps on an Ubuntu server -- specifically, Ubuntu 16.04.  This is not needed for your local development, as you can run `rails server` to test it out locally, or test it out on the course server provided.  However, if you ***do*** want to get it running through Apache, read on...

These instructions are as of August 2017; you can see an [older version of this document](https://github.com/aaronbloomfield/slp/blob/f62e5120144954b34a50a5df26dbf9c9f6b78bf8/docs/rubyrails-deployment.md) online.

This assumes we are running on Ubuntu 16.04, and want to install Ruby 2.4 (2.4.1 is the latest as of this writing) and Rails 5.1 (5.1.3 is the latest as of this writing).

### Quick Start

- Install Ruby 2.4.1 on the system (the version we are using) via the directions [here](https://www.brightbox.com/blog/2017/01/13/ruby-2-4-ubuntu-packages/)
- Ensure that the the default `ruby` and `gem` comamnds are set `update-alternatives --set ruby /usr/bin/ruby2.4` and `update-alternatives --set gem /usr/bin/gem2.4` (the apt-get install may do this, but it's here just in case it doesn't, or if there are multiple ruby/gem versions on the system).
- Install passenger via the directions at [here](https://www.phusionpassenger.com/library/install/apache/install/oss/xenial/) (if this doesn't enable the passenger module, run `a2enmod enable passenger` and then `service apache2 restart`)
    - I had a bunch of problems with this, as it was not installing correctly.  For the last line of those instructions, I had to run `dpkg --purge libapache2-mod-passenger`, `apt-get clean`, and then `apt-get install libapache2-mod-passenger`.
- Sanity check: Passenger uses the version of Ruby specified in the `/etc/apache2/mods-available/passenger.conf` file (specifically, the `PassengerDefaultRuby` value).  Run that with -v to make sure that the version is correct: `/usr/bin/passenger_free_ruby -v`
- Create a ~root/.gemrc file to prevent documentation from always being installed; the contents are one line: `gem: --no-ri --no-rdoc` (and putting that in /etc/skel/ is not a bad idea)
- Install two gems locally:
    - `gem install bundler`
    - `gem install rails` (this will install a bunch of dependencies)
- As a non-privledged user, run `rails new myapp`
- It will ask for your password to install the gems globally; hit Ctrl-C, and then (in the `myapp` directory) run `bundle install --path vendor/bundle`
- Configure that with apache (see below), and ensure it works
- Once all that was working, create a rails app with a user that *can* sudo, and when installing the packages, enter that user's password.  This will install some gems in `/var/lib/gems/2.4.0/gems`, which will prevent them from always being copied to a user's directory.

### Configuring Apache

1. Check /etc/apache2/mods-available/passenger.conf.  The `PassengerDefaultRuby` line should be using `/usr/bin/ruby`, which means (assuming the previous section was completed successfully) that Passenger is using the correct version of Ruby.
2. Edit your /etc/apache2/sites-available/000-default.conf file, and add the following lines.  Here, `/home/rails/myapp` is where the Rails app is installed on the local file system, and `/rails/railstest` is the URL for the Rails app (specifically, it will be at http://server/rails/railstest).  In this example, all the rails apps are kept in a `rails/` directory (and that directory was in the HTML document root, which is likely `/var/www/html/`), but that need not be the case.  The lines to add to 000-default.conf:
```
RailsEnv development
RailsBaseURI /rails/railstest
<Directory /home/rails/myapp>
  Options -MultiViews
</Directory>
```
3. In the HTML document root (likely `/var/www/html`), make that `rails` sub-directory.  In that directory, run `ln -s /home/rails/myapp/public railstest`, where `/home/rails/myapp` and `railstest` are the values from step 5, above.
4. Reload apache: `sudo service apache2 reload`
5. View the app at `http://localhost/rails/railstest`.  It should show the image displayed at the bottom of the [Ruby on Rails getting started](rubyrails-getting-started.html) ([md](rubyrails-getting-started.md)) page.

### Users installing gems

The easist way is to have the users run `bundle install --path vendor/bundle` instead of `bundle install`.  This will install the gems into their rails app directory (specifically, `myapp/vendor/bundle/ruby/2.4.0/gems/`).  Passenger will look there for the gems, in addition to the system directory (which is `/var/lib/gems/2.4.0/gems`).  Having them install the gems locally will take up more disk space (maybe 50-100 Mb per user), but will be far easier, as the students can manage their own gems without sudo privledges.

### Allowing users to reload apache

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

### Deploying multiple Rails apps on one server

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


### Troubleshooting

#### Generic error with no known cause

Rails apps have a log/ directory (such as /home/rails/myapp/log), and therein provide a lot of detail about what is going wrong.

#### /usr/lib/ruby/include/ruby.h not found

This should have been solved by installing the `ruby2.4-dev` package; if not, see the [older version of this document](https://github.com/aaronbloomfield/slp/blob/f62e5120144954b34a50a5df26dbf9c9f6b78bf8/docs/rubyrails-deployment.md).

#### "The page you were looking for doesn't exist"

This problem seems to occur when running the Rails apps in production mode.  For the reason why (and clues how to fix it), see the answer [here](http://stackoverflow.com/questions/22026492/heroku-rails-setup-the-page-you-were-looking-for-doesnt-exist)

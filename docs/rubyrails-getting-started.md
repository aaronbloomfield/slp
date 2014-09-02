SLP: Ruby on Rails: Getting Started
===================================

[Go up to the main SLP documents page](index.html) ([md](index.md))

### Install Ruby and Rails

Note that Ruby is the programming language, and Rails is the framework.  When all is completed, you ***MUST*** have Ruby version 2.1.2 installed and Rails 4.1.5 installed.  It's okay if you have a different patch level version (i.e., Ruby 2.1.3 or rails 4.1.6, for example), but you can't have a different minor or major version.

If you are running this on the VirtualBox image, then this step has already been performed.

If you are running this on your *own* machine, then you will want to follow the instructions at [https://gorails.com/setup/ubuntu/14.04](https://gorails.com/setup/ubuntu/14.04) if you are running Ubuntu 14.04.  If you are running Mac OS X, that site has directions [here](https://gorails.com/setup/osx).  Likewise, other versions of Ubuntu can be found from those two links.  If you are using another operating system, you are on your own (but note that the versions ***MUST*** match what we are using (Ruby 2.1.2 and Rails 4.1.5)).

To install this on the course server, you could follow the directions from https://gorails.com/setup/ubuntu/14.04.  However, those directions create a series of directories, which have already been partially done for you.  Instead, follow these steps.  Note that this will only work on the course server!

1. Enter `cd` to ensure you are in your home directory
2. Enter `tar xfz /usr/local/rails-setup.tgz`
3. Edit your `.bashrc` file, and add the three lines indicated in the `bashrc-mods.txt` file
4. Log out, then log back in again
5. Try the "final steps" commands from [those directions](https://gorails.com/setup/ubuntu/14.04).  In particular, if you run `rails new myapp`, then it should work.  (You will likely want to delete the myapp/ directory once you've tested this)

You will have to run through the rest of the "final steps" commands to configure your rails application.  As your first rails application needs to be called "rubyrails", you will need to enter `rails new rubyrails -d mysql`, and then the rest of the commands listed there.

### Setting up a new Rails app

To set up a new Rails app, you will need to follow the "final steps" instructions from [here](https://gorails.com/setup/ubuntu/14.04).  Specifically:

1. Run `rails new myapp -d mysql`, where "myapp" is the name of the app you are creating
2. Edit myapp/config/database.yml, and enter your MySQL credentials (change username, password, and database).  For now, change ***all*** the fields (username and password appear twice, and database appears three times).  For the homework, you can list your password in plaintext in that file.  However, for your project app, you are ***NOT*** to have the plain text passwords in the files in the repository -- see the Security section, next, for how to handle the password.
3. From the myapp/ directory, run `rake db:create`
4. Change the production secret\_key\_base in config/secrets.yml.  Again, for your homework, you can put the value in that file, but for your project, you will need to follow the instructions in the Security section, below.
5. Run `bundle install` in your app directory
6. Reload the apache web server by running `/usr/local/bin/reload-apache2`

### Security

There are a few values that must ***NOT*** be kept in the repositories -- they are the MySQL passwords in config/database.yml, and the secret\_key\_base in config/secrets.yml.  The default files for config/database.yml and config/secrets.yml access environment variables, but the Apache Passenger module doesn't handle those very well for individual users.

The easiest way is to copy secrets.yml to secrets.yml.template, and like for database.yml to database.yml.template.  Edit the actual files (i.e., not the templates), and put the passwords or keys directly into those files.  Add the two *.template files via git, but do NOT add the database.yml and secrets.yml files.  Create a .gitignore file that lists two lines, which are the names of those two files (database.yml and secrets.yml).

What this does is commit to the git repo the tempaltes, but the actual files -- the ones with the passwords -- are not kept in the repo.  This means, however, that each person who checks out the repo will have to set those values manuall.

For those who are interested, there are more options listed [here](http://railsapps.github.io/rails-environment-variables.html).


### Installing new gems

1. Edit the Gemfile, and add the line to add the gem: `gem 'foobar', group: :development`, or similar
2. Run `bundle install`
    - Depending on the gem, it may prompt you to enter `bundle install --path vendor/bundle` instead
3. You should be able to view your app through `rails server`, as you have installed it locally (in ./.rbenv/versions/2.1.2/lib/ruby/gems/2.1.0/gems/, if you are interested).
4. The previous command installs the gem(s) in a single `gems/` directory that *all* of your Ruby apps can use.  However, the webserver cannot access that directory.  Thus, you need to run `bundle --deployment`.  This will install all the gems *again*, but this time to vendor/bundle, which the web server *can* find.
4. Reload the apache web server by running `/usr/local/bin/reload-apache2`

### Viewing your Rails application

If you want to view your rails application locally, you can view it via the rails server, as described in [those directions](https://gorails.com/setup/ubuntu/14.04).  This will allow you to view it at `http://localhost:3000` in your favorite web browser.

Your Rails app will have a VERY specific directory that it must be installed in, as the web server is already configured for that directory.  As long as it is in that directory, it will be visible through Apache.

When you view your app, it should look like the image below:

![](images/rails-initial.png)

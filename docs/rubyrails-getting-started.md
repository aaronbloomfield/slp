SLP: Ruby on Rails: Getting Started
===================================

[Go up to the main SLP documents page](index.html) ([md](index.md))

### Install Ruby and Rails

Ruby is the programming language, and Rails is the framework.  They are typically used at the same time, so the entire setup is often just called "Rails".

Note that Ruby is the programming language, and Rails is the framework.  When all is completed, you ***MUST*** have Ruby version 2.2.3 installed and Rails 4.2.3 installed.  It's okay if you have a different patch level version (i.e., Ruby 2.2.4 or rails 4.2.4, for example), but you can't have a different minor or major version.

If you are running this on the VirtualBox image, then this step has already been performed.  See below for getting this working on the course server.

If you are running this on your *own* machine, then you will want to follow the instructions at [https://gorails.com/setup/ubuntu/14.04](https://gorails.com/setup/ubuntu/14.04) if you are running Ubuntu 14.04.  If you are running Mac OS X, that site has directions [here](https://gorails.com/setup/osx).  Likewise, other versions of Ubuntu can be found from those two links.  If you are using another operating system, you are on your own (but note that the versions ***MUST*** match what we are using (Ruby 2.2.3 and Rails 4.2.3)).

To install this on the course server, you could follow the directions from [that site](https://gorails.com/setup/ubuntu/14.04).  However, those directions take about a half hour to complete.  Instead, follow the following steps.  Note that this will only work on the course server!

1. Enter `cd` to ensure you are in your home directory
2. Enter `tar xfz /usr/local/rails-setup.tgz`
3. Edit your `.bashrc` file, and add the three lines indicated in the `bashrc-mods.txt` file
4. Log out, then log back in again
5. Try the "final steps" commands from [those directions](https://gorails.com/setup/ubuntu/14.04).  In particular, if you run `rails new myapp`, then it should work.  (You will likely want to delete the myapp/ directory once you've tested this)

You will have to run through the rest of the "final steps" commands to configure your rails application.  As your first rails application needs to be called "railshw", you will need to enter `rails new railshw -d mysql`, and then the rest of the commands listed there.

### Setting up a new Rails app

To set up a new Rails app, you will need to follow the "final steps" instructions from [here](https://gorails.com/setup/ubuntu/14.04).  Specifically:

1. Run `rails new railshw -d mysql`, where "railshw" is the name of the app you are creating
    - For this homework, your Rails app MUST be named "railshw", and it MUST be in your home directory, as this is how the web server is configured.  In particular, there must be a ~mst3k/railshw/public directory.
    - For your project (when you are assigned to your project later), the name of the Rails app should match your project tag
    - If it asks you for your password for sudo, hit Control-C.  It wants to install the Ruby gems (the libraries) system-wide, and we are going to do it in your individual user account.  To do this, enter `cd railshw` to move into the Rails application directory that you just created, then enter `bundle install --path vendor/bundle`.  This will take a minute or two to complete.
2. Edit railshw/config/database.yml, and enter your MySQL credentials (change username, password, and database).  For now, change ***all*** the fields (username and password appear twice, and database appears three times).  The database name is the same as your userid.  For the homework, you can list your password in plaintext in that file.  However, for your project app, you are ***NOT*** to have the plain text passwords in the files in the repository -- see the Security section, next, for how to handle the password.
3. From the railshw/ directory, run `bundle install` (or, if on the course server, `bundle install --path vendor/bundle`).
4. This no longer works, so ignore this. ~~If you want to use a database table *prefix* (which you do for the homework), then you will have to edit the three files in railshw/config/environments/ (the files are: development.rb, production.rb, and test.rb), and adding the following line to each: `config.active_record.table_name_prefix = "ruby_"`.  Hhere, `ruby_` is the prefix, but use whatever you would like, within reason.  This line can go at the end, just before the last line (which is `end`).~~
5. From the railshw/ directory, run `rake db:create`.  It will say that 'mst3k already exists', twice -- that's fine.
    - For the fall 2015 semester, an upgrade to the mysql2 gem has broken it, and it reports a number of errors (you can see details about that [here](https://github.com/brianmario/mysql2/issues/675) and [here](https://github.com/rails/rails/issues/21544), if you are interested).  To fix this, you need to revert to an earlier mysql2 gem.  To do so, replace the `gem 'mysql2'` line in your Gemfile with `gem 'mysql2', '~> 0.3.20'`, and run `bundle install --path vendor/bundle`.  At that point, `rake db:migrate` should work.
6. Install the gems locally to your Rails app: from ~/railshw, run `bundle install --path vendor/bundle`; this will take a few minutes to run.

If you are running this on the course server, you will need to reload the apache web server by running `/usr/local/bin/reload-apache2`.  At this point, you should be able to view your Rails app at `http://server/rails/mst3k`.  Note that there is no tilde ("~") there!  And obviously replace "mst3k" with your userid.  It should look ***exactly*** like the image at the bottom of this page.


### Security

There are a few values that must ***NOT*** be kept in the repositories -- they are the MySQL passwords in config/database.yml, and the secret\_key\_base in config/secrets.yml.  The default files for config/database.yml and config/secrets.yml access environment variables, but the Apache Passenger module doesn't handle those very well for individual users.

The easiest way is to copy secrets.yml to secrets.yml.template, and like for database.yml to database.yml.template.  Edit the actual files (i.e., not the templates), and put the passwords or keys directly into those files.  Add the two *.template files via git, but do NOT add the database.yml and secrets.yml files.  Create a .gitignore file that lists two lines, which are the names of those two files (database.yml and secrets.yml).

What this does is commit to the git repo the tempaltes, but the actual files -- the ones with the passwords -- are not kept in the repo.  This means, however, that each person who checks out the repo will have to set those values manuall.

For those who are interested, there are more options listed [here](http://railsapps.github.io/rails-environment-variables.html).  Keeping this information in environment variables, which is the recommended way to handle this, causes conflits with how Passenger works (Passenger is the Apache module that runs your Rails app)


### Installing new gems

1. Edit the Gemfile, and add the line to add the gem: `gem 'foobar', group: :development`, or similar
2. Run `bundle install`
    - Depending on the gem, it may prompt you to enter `bundle install --path vendor/bundle` instead
3. You should be able to view your app through `rails server`, as you have installed it locally (in ./.rbenv/versions/2.2.3/lib/ruby/gems/2.2.0/gems/, if you are interested).
4. The previous command installs the gem(s) in a single `gems/` directory that *all* of your Ruby apps can use.  However, the webserver cannot access that directory.  Thus, you need to run `bundle --deployment`.  This will install all the gems *again*, but this time to vendor/bundle, which the web server *can* find.
4. Reload the apache web server by running `/usr/local/bin/reload-apache2`

### Uploading to the server

The server is 64-bit, and your local development platform is likely 32-bit (especially if you are using the VirtualBox image).  Thus, in addition to uploading, you will have to reinstall the gems.  To do so:

- Upload the files to pegasus: `rsync -a railshw/ mst3k@server:~/railshw`
    - The syntax for that command (such as all the forward slashes) must be exact!
    - At this point, your rails page should give a Passenger error with "wrong ELF class" listed there
- Log into pegasus: `ssh mst3k@server`
- Move into the railshw/ directory that was just created when we uploaded the directory: `cd railshw`
- Delete the vendor/bundle directory: `/bin/rm -rf vendor/bundle`
- Reinstall the gems: `bundle install --path vendor/bundle`
  - At this point, you should be able to view your application

You may need to transfer the DB, or run db:migrate, depending on the details of your app

### Troubleshooting

If you get a Passenger error on the server that states, "wrong ELF class", as shown [here](images/rails-bad-elf.png), then the problem is that you have installed all of your gems (via `bundle install --path vendor/bundle`) on your local ***32-bit*** machine or image, and then uploaded them to the server, which is a 64-bit machine.  To fix, you must delete the ***entire*** bundle directory on the server: `/bin/rm -rf ~/railshw/vendor/bundle`, and then reinstall them all again (via `bundle install --path vendor/bundle`).

### Viewing your Rails application

If you want to view your rails application locally, you can view it via the rails server, as described in [those directions](https://gorails.com/setup/ubuntu/14.04) (basically, run `rails server`, and then view it at `http://localhost:3000` in your favorite web browser).

Your Rails app will have a VERY specific directory that it must be installed in, as the web server is already configured for that directory.  As long as it is in that directory, it will be visible through Apache.

When you view your app, it should look ***exactly*** like the image at the image below (albeit with a different URL):

![](images/rails-initial.png)

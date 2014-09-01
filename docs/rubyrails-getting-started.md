SLP: Ruby on Rails: Getting Started
===================================

[Go up to the main SLP documents page](index.html) ([md](index.md))

Install Ruby and Rails
----------------------

Note that Ruby is the programming language, and Rails is the framework.

If you are running this on the VirtualBox image, then this step has already been performed.

If you are running this on your *own* machine, then you will want to follow the instructions at https://gorails.com/setup/ubuntu/14.04 if you are running Ubuntu 14.04.  If you are running Mac OS X, that site has directions [here](https://gorails.com/setup/osx).  Likewise, other versions of Ubuntu can be found from those two links.  If you are using another operating system, you are on your own (but note that the versions ***MUST*** match what we are using (Ruby 2.1.2 and Rails 4.1.5)).

To install this on the course server, you could follow the directions from https://gorails.com/setup/ubuntu/14.04.  However, those directions create a series of directories, which have already been partially done for you.  Instead, follow these steps.  Note that this will only work on the course server!

1. Enter `cd` to ensure you are in your home directory
2. Enter `tar xfz /usr/local/rails-setup.tgz`
3. Edit your `.bashrc` file, and add the three lines indicated in the `bashrc-mods.txt` file
4. Log out, then log back in again
5. Try the "final steps" commands from [those directions](https://gorails.com/setup/ubuntu/14.04).  In particular, if you run `rails new myapp`, then it should work.

You will have to run through the rest of the "final steps" commands to configure your rails application.  As your first rails application needs to be called "rubyrails", you will need to enter `rails new rubyrails -d mysql`, and then the rest of the commands listed there.


Viewing your Rails application locally
--------------------------------------

If you want to view your rails application locally, you can view it via the rails server, as described in [those directions](https://gorails.com/setup/ubuntu/14.04).  This will allow you to view it at `http://localhost:3000` in your favorite web browser.

Viewing your Rails application via Apache
-----------------------------------------





...

SLP: Docker Usage
=================

[Go up to the main SLP documents page](index.html) ([md](index.md))


Docker startup

- Download the docker image
    - Run `docker login <server>:5000`, where `<server>` is the provided docker server
	- Run `docker pull <server>:5000/slp:latest`
        - You can run that last command in the future to upgrade to the latest version
        - That being said, the above commands only need to be run once (or if you are upgrading), and not each time you start up a container
    - Instead of the two docker commands above, you could build the image yourself from the included [Dockerfile](../utils/docker/Dockerfile), but that will take much longer
- Choose a directory to save your work do
    - If you save it in the container, if the container is removed (as often happens), then your work is lost, hence why we want to save it locally
	- We'll call that directory /path/to/work
- Start up docker: `docker run -it -p 3000:3000 -v /path/to/work:/root/work slp`
    - This will provide you with a command prompt *inside* the container

To create and run a rails app:

- Enter `cd` to make sure you are in root's home directory (not the / directory, which is the default)
- Run `rails new myapp` (we aren't specifying a DB, so it defaults to sqlite3)
    - Change "myapp" to the name of your project
- Change the last line in your Gemfile from `gem 'tzinfo-data', platforms: [:mingw, :mswin, :x64_mingw, :jruby]` to just `gem 'tzinfo-data'`
- Remove the Gemfile.lock file in the project directory
- Run `bundle install` in the project directory
- You can now run `rails server` from the project directory
- You can view your app from http://0.0.0.0:3000


To create and run a django app:

- Run `django-admin startproject mysite`
    - Change "mysite" to the name of your project
- In the mysite/ directory, run `python3 manage.py runserver 0.0.0.0:3000`
    - Note that you have to specify the IP address and port!
- You can view your app from http://0.0.0.0:3000

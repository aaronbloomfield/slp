Development Environment Setup
=============================

[Go up to the main SLP documents page](index.html) ([md](index.md))

This page is meant to help you get your local development environment up and running.  The assumption is that you are developing on the provided Linux VirtualBox image.  Mac OS X will work somewhat similarly, but you are on your own for configuring this on Windows.

In the directions below, your "local machine" is whatever account you are doing the development on; this will often be the VirtualBox image, but could be other things as well.  On the course server, `team` is the tag of your team (which is the same as the account name).

### Automatic MySQL login

To make it easier to automatically log into MySQL, you should edit a file named `~/.my.cnf`.  It should contain exactly two lines:

```
[client]
password=foobar
```

Set `foobar` to your password.  When you enter `mysql`, it will attempt to log you in as the same user you are already logged into Linux as (your UVa userid, or your team account), and the provided password.  You will want to do this on both your local machine and the group account on the course server.

If you want to create mysql users or databases, or grant permissions to them for mysql users to access certain databases, see the [MySQL introduction](mysql-intro.html) ([md](mysql-intro.md)) and [MySQL reference](mysql-reference.html) ([md](mysql-reference.md)) pages.

### Automatic SSH login

If you want to gain a command prompt on another machine, you can enter `ssh foo@bar.edu`, which will attempt to log you in to the bar.edu server as the `foo` user.  It will prompt you for a password each time.  To prevent this, you can use ssh authentication keys.

To do so, enter `ssh-keygen` on your local machine; you will likely not want to enter a password.  This will create two files: `~/.ssh/id_rsa.pub` and `~/.ssh/id_rsa`.  These are your public and private keys, respectively, for logging in to places.

Log into the team account on the course server.  Edit `~/.ssh/authorized_keys` (on the course server), creating the file if it does not exist.  Copy and paste the contents of your `~/.ssh/id_rsa.pub` into that file, save, and exit.

Now when you attempt to log into the team account, it should not ask you for a password.

### Automatic Github login

From the [Github :: profile :: SSH keys](https://github.com/settings/ssh) page, you can enter your public key (the contents of the `~/.ssh/id_rsa.pub` file) as an SSH key.  This will allow you to automatically log into github.  Note that you will want to clone a repo via `git clone git@github.com:org/repo`, and *not* via the https URL.

### Download of the team database

This section will describe how to automatically download the team database from the course server to your local machine.  A number of things need to be set up first:

- You need to be able to automatically log in, via ssh, to the course server as the team account (see "automatic ssh login", above)
- You need to be able to log in, without a password, to mysql as the team user on the course server
- You need to be able to log in, without a password, to mysql as the team user on your *local* machine
  - If you use the team username and password on your local machine, then you don't have to change any of the configuration settings

You can then execute the following command to download the database contents from the course server.  Note that this will ***ERASE*** the local database!

```
ssh team@server 'mysqldump database | gzip -9c -' | gzip -dc | mysql database
```

Make sure the format is exactly as shown, including the location of the single quotes!  You will need to change `team`, `server`, and `database` in that command.  This command does a mysqldump on the server, and compresses it, and downloads that -- which is then uncompressed and restored to the local database.

This is a lot to remember -- and get right -- so you can put it into a Makefile:

```
syncdb:
	@echo Downloading the DB from the course server...
	@ssh team@server 'mysqldump database | gzip -9c -' | gzip -dc | mysql database
```

Remember that indented lines in a Makefile MUST be tabbed, and not spaces.  You then only have to type `make syncdb`, and it will perform the download for you.  The `@` before the various commands causes it to only display the results of that command, and not the command itself.

### Deployment options

There are two ways to easily deploy your project to the team account.  They don't work well with each other, so you should likely choose one or the other.  They two options are rsync and github.

The rsync version allows you to deploy the current version you are working on, and you do not need to commit it (and push to github) to deploy a version to test.  The github is easier, but you have to do a push to have any changes pushed to the server.  Either one can be used, but the entire team will have to use the same method.

### Rsync deployment

To configure rsync deployment, we are assuming that you can automatically log in, via ssh, to the course server as the team account (see "automatic ssh login", above).

What you are going to do here is enter an rsync command to upload the files to the course server.  Rsync is tricky to get right, but it's a very powerful tool.  Rsync is described in a bit more detail on the "uploading files to the server" section of the [framework homework](hw-frameworks.html) ([md](framework-hw.md)).

One you are in your project root's directory, you will enter a command similar to:

```
rsync -C --del --progress -a ./ team@server:~/html/
```

There are a lot of things that need to be changed on that line to make it work for you: `team`, `server`, and the directory (only the CakePHP projects will go into `~/html`).  Furthermore, you may want to exclude certain files, such as your .htaccess files (for CakePHP), or the settings (for all projects).  You can do this via the `--exclude file` flag.

The other issue with this command is that it MUST be executed from the `~/html/` (or equivalent) directory.  It, too, is tricky to get right.  So you can put it as a Makefile target:

```
deploy:
	rsync -C --del --progress -a --exclude foo/settings.py ./ team@server:~/html/
```

Then, you only have to type `make publish` to publish it to the course server.

One can also deploy to localhost (the machine you are using).  This will be of most use for the CakePHP projects, as the Rails and Django folks view the app differently.  You can create a user on your local machine that has the same name as the team account on the course server, and do a publish there -- that command would look similar to the rsync of the previous section, but the course server is replaced with `localhost`.  This works well as a `local` target, meaning you enter `make local` to publish it to the local machine.  You will have to set up automatic ssh login.  And note that any files that you exclude still have to be uploaded the very first time (and possibly edited).

### Github deployment

This information has moved to the [Github webhook setup page](github-webhook.html) ([md](github-webhook.md)).

### Hybrid approach

ALternatively, you should set up a hybrid approach.  You can use a deployment key to clone the git repo in the project account, but rather than have that updated automatically via a web hook, you instead only update it manually.  This can be done via a single ssh command -- which, with auto-login set up, can easily be put into a Makefile:

```
ssh project@server 'cd ~/html; git pull`
```

Obviously, you will have to change the directory (and user and server host) as necessary.

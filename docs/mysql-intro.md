SLP: MySQL Introduction
=======================

[Go up to the main SLP documents page](index.html) ([md](index.md))

This page is meant to allow you to use enough MySQL so that you can complete the first set of tutorials.  It is not meant to be a reference; the [MySQL Reference](mysql-reference.html) ([md](mysql-reference.md)) page does that.  And it's not meant to be an introduction to MySQL commands, either - in fact, no actual data entry (or extraction) commands are listed here.

### Users and permissions ###

MySQL has, as far as this tutorial is concerned, two types of users: ones who can create databases and users, and ones who cannot.

The 'root' user can create databases.  On the VirtualBox image, the password for the root user is 'password'.  You will need to create a MySQL user that has your userid (we'll use 'mst3k' in this tutorial for your userid).

### Starting MySQL as root ###

To start MySQL from the command line, enter:

```
mysql -u root -p
```

The `-u root` flag is specifying the user to log in as.  The `-p` flag is stating that you want it to prompt you for a password.  On the VirtualBox image, the password is 'password'.  You can also specify the password on the command line (such as `mysql -u root -ppassword` (notice no space between the `-p` and the password; if there is a space, then MySQL will assume that you want to be prompted for the password, and what comes next on the command line will be interested as something else)), however this is **NOT** recomended, as the password is visible to any users (since it is the process you are now running).

You will have a `mysql>` prompt; enter `exit` or `quit` to exit.

### Creating a database ###

This can only be done as root.  To create a database, enter (from the `mysql>` prompt):

```
create database mst3k;
```

Note that the database that is being created is the same as your userid.  The reason for this is that this will be the database name on the course server, so keeping it the same will make it easier for you when you are deploying your code.

At this point, the database is created, but only the root user can access it.  Thus, we want to:

### Create other users ###

MySQL doesn't have users, per se, but instead has valid username/password pairs that can log in.  To create one:

```
grant all on mst3k.* to 'mst3k' identified by 'foobar';
grant all on mst3k.* to 'mst3k'@'localhost' identified by 'foobar';
```

There are a number of things to note:
- There are TWO separate commands; the only difference is the `@'localhost'` on the second command.  We aren't going into why this is necessary here.
- `mst3k` is your userid (that is listed twice on each line), and `foobar` is your password
-- Tou will likely want to use the password found in the Post'Em tool; again, this will save you the hassle of having to reconfigure your application when you deploy it to the server
- The first occurrence of 'mst3k' on the line is the database name; the second is the userid who can access it.  For our purposes, they just happen to be the same.
- The syntax is rather tricky, and it does not accept any variations.  The database specification is `mst3k.*`, for example

At this point, you can log out of mysql via `exit` or `quit` (or Control-D)

### Starting MySQL as your userid ###

To start MySQL as your userid, enter:

```
mysql -u mst3k -p mst3k
```

You'll notice this has a slightly different syntax - in addition to specifying the 'mst3k' user instead of the 'root' user, there is an additional 'mst3k' at the end.  That last 'mst3k' is specifying the database that you want to connect to.

Once logged in (i.e., you have the `mysql>` prompt), you can enter `show tables;` to see what tables are in your database.  There are likely none at this point, so it will state `Empty set`.

### Creating tables for Ruby ###

One nice aspect of Ruby is that you don't have to create the tables yourself.  When you enter `rake db:migrate`, the tables will be created for you automatically.

### Creating tables for CakePHP ###

This is a bit more work, but still pretty easy.  The tutorial will specify some MySQL commands to create the tables.  For example, the blog tutorial includes the following MySQL commands:

```
/* First, create our posts table: */
CREATE TABLE posts (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(50),
    body TEXT,
    created DATETIME DEFAULT NULL,
    modified DATETIME DEFAULT NULL
);

/* Then insert some posts for testing: */
INSERT INTO posts (title,body,created)
    VALUES ('The title', 'This is the post body.', NOW());
INSERT INTO posts (title,body,created)
    VALUES ('A title once again', 'And the post body follows.', NOW());
INSERT INTO posts (title,body,created)
    VALUES ('Title strikes back', 'This is really exciting! Not.', NOW());
```

We can ALMOST just do a cut-and-paste (and paste it into the `mysql>` prompt).  The only thing we have to change is the table name.  We want our CakePHP posts table to be called `cake_posts`, and our Ruby on Rails posts table to be called `rails_posts`, as we don't want them writing to the same table.  So we are going to change the table name (which occurs four times in those MySQL commands) from `posts` to `cake_posts`.  Once that is done, we can cut-and-paste the commands right into the `mysql>` prompt.

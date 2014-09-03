SLP: MySQL Reference 
====================

[Go up to the main SLP documents page](index.html) ([md](index.md))

### Starting up MySQL ###

This assumes that you have already installed MySQL.  On Ubuntu, this typically means installing the 'mysql-server' package.

To start up MySQL, enter `mysql -u mst3k -p` from the command line.  The `-u mst3k` is the user you are logging in as.  The `-p` tells MySQL to prompt you for a password.  If the name of your MySQL user is the same as your Unix user, then you can eliminate the `-u mst3k` part entirely.  You may also want to specify a starting database: `mysql -p foobar` - if you do not, you will have to execute a `use foobar;` command (see below) once logged into mysql.

If you would rather not have to enter your MySQL password each time, you can create a `.my.cnf` (yes, that's the correct file name) in your Unix user's home directory.  It should have two lines:

```
[client]
password=abcdefg
```

Be sure the permissions are set properly (`chmod 600 .my.cnf`), and you can just run `mysql` or `mysql foobar` to start the client; the password is read from the .my.cnf file, and thus you are not prompted for it.


### Database management ###

**To create a database:** `create database foobar;`.  Note that this must be run as the administrative user (typically root).

**To see what databases are avaialble:** `show databases;`.  This will show all the databases that you have access to.  If you are the root user, then it will show all the databases.

**To use a database once logged in:** `use foobar;`

**To give a user permission to use a database:** `grant all on foobar.* to 'userid' identified by 'password';`.  Note the foobar.* part as well as the quotes around the userid and password.  Some platforms also require that you specify `'userid'@'localhost'` as well as just 'userid', so I usually enter both commands.  'password' is the password you use when you start up mysql.  If you already have a password set, you can ignore the `identified by 'password'` part.


### Table management ###

A sample table creation command looks like the following.  This can all be one line or on multiple lines.  And the MySQL keywords are capitalized to differentiate them in the examples below, but they can all be lower case as well (MySQL doesn't care about the case of the keywords, but does with user identifiers):

```
CREATE TABLE example_autoincrement (
       id INT,
       data VARCHAR(100)
       thedate DATETIME,
       value DOUBLE
);
```

Note that there is no comma after the last field in the table.

If you are creating tables for CakePHP, then there are a few additional things you want to put in all your tables:
- an `id INT` field that automatically increments (each new record has a successively higher value)
  - a primary key on that `id` field.  A primary key means that this is the primary way to differentiate one record from another.  You can create primary keys out of almost anything, but making them out of an auto-incrementing int field is most common.
- a `created DATETIME` field that CakePHP will update for you
- a `modified DATETIME` field that CakePHP will update for you

Thus, a CakePHP table might look like the following:

```
CREATE TABLE semesters (
       id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
       title TINYTEXT,
       year INT UNSIGNED,
       start_month TINYINT UNSIGNED,
       end_month TINYINT UNSIGNED,
       created DATETIME DEFAULT NULL,
       modified DATETIME DEFAULT NULL
) ENGINE=innodb;
```

The title, year, start\_month, and end\_month fields would be replaced with the data that you want to hold in your table.  Note the 'ENGINE=innodb' part at the end - this is necessary if we want to use foreign keys, which this tutorial is not going over.  But putting that in doesn't hurt, and allows the inclusion of foreign keys in the future.

MySQL has a [large number of data types](http://dev.mysql.com/doc/refman/5.0/en/data-types.html)

**To see the tables that are available:** `show tables;`.  This shows the tables in the current database.

**To delete a table:** ` drop table semesters;`.  This will delete all the data from that table!

**To see the information about a table:** `describe semesters;`.  This shows the default column values, which are used below.

**To remove all elements from a talbe:** `truncate semesters;`.  You can do a delete command (see below), but truncate is much faster, as it does not remove each element one-by-one, which is what delete does.


### Retrieving data from the database ###

We are assuming the semesters table from above is the table we are pulling data from.

A typical command is: `select * from semesters;`.  You can specify which data you want to retrieve via a where clause: `select * from semesters where year=1989;`.  The `*` means all columns; you can select individual ones as well: `select id, title, year from semesters where year=1989;`.

There are many other options to the select command: http://dev.mysql.com/doc/refman/5.0/en/select.html


### Inserting and updating data in the database ###

You can insert data into the database in two forms:

```
insert into semesters set year=2012, start_month=3, end_month=12, created=now(), modified=now();
insert into semesters values (null, "Fall 2012", 2012, 8, 12, now(), now()), (null, "Fall 2011", 2011, 8, 12, now(), now());
```

A bunch of things are happening here:
- in the first one, note that only some of the fields are being specified; the others have the default values (which the describe table command displays)
- `now()` is a function call, and returns the current datetime
- the second command inserts multiple rows at once, but all columns in the row must be specified
- note that we specify `null` for the id, as MySQL will assign an auto-incremented id value for us

To update a row: `update semesters set start_month=9 where year=2012;`.  MAKE SURE that you specify your `where` clause, else it will update EVERY row in your table!

### Reomving data ###

`delete from semesters where year=2012;`  MAKE SURE that you specify your `where` clause, else it will delete EVERY row in your table!

You can also use the truncate command, described above

### Backing up and restoring your database ###

From the command line (NOT from the MySQL prompt), we will use the mysqldump command:

```
mysqldump -u mst3k -p foobar > foobar.sql
```

Note that the options to mysqldump are the same as when you call mysql from the command prompt:

- `-u mst3k`: specifies the user, but is not necessary if it is the same as your Unix user
- `-p`: specifies to prompt for a password, but is not necessary if you set up your .my.cnf as described above
- `foobar`: specifies the database to back up; unlike with the mysql client, this is required

This command also redirects the dump to a file (called "foobar.sql").

To restore the database:

```
cat foobar.sql | mysql -u mst3k -p foobar
```

Note that this will erase ALL values that currently exist in the tables.

### Advanced topics ###

There are a number of more advanced topics that we are not going over in this tutorial; we are just listing them here:

- foreign keys
- join operations
- details of the types
- mysql functions (we only saw 'now()')
- views
- etc.

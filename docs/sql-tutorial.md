SLP: SQL Tutorial
=======================

[Go up to the main SLP documents page](index.html) ([md](index.md))

SQL is a Structured Query Language that lets you create, access, and manipulate databases. It can be used to:

- create new databases

- create new tables in a database

- insert records in a table

- retrieve data from a table

- update records in a table

- delete records from a table

- execute queries to retreive (sub)records from the database

### SQL Commands ###

This sections lists the most important SQL commands:

- __SELECT__ - extracts data from a database
	
- __UPDATE__ - updates data in a database
	
- __DELETE__ - deletes data from a database
	
- __INSERT INTO__ - inserts new data into a database
	
- __CREATE DATABASE__ - creates a new database
	
- __ALTER DATABASE__ - modifies a database
	
- __CREATE TABLE__ - creates a new table
	
- __ALTER TABLE__ - modifies a table
	
- __DROP TABLE__ - deletes a table
	
- __CREATE INDEX__ - creates an index (search key)
	
- __DROP INDEX__ - deletes an index
	
	
### CREATE DATABASE ###

To create a database, type `CREATE DATABASE dbname;`. 

Now, create a database called `uva`.

### CREATE TABLE ###

Before you can create tables, you must choose a database. You can type `USE uva` to use the `uva` database you just created.

We want to create a table named `students` which has the following fields:

- comp_ID (PRIMARY KEY)

- First_Name

- Last_Name

- Major

- Year

You can create such table using the following command:

```
create table students( comp_ID varchar(6) PRIMARY KEY,

First_Name varchar(20), Last_Name varchar(20), 

Major varchar(5) NULL, Year int(1) );
```

### INSERT INTO ###

To populate the newly created table with some entries, use the following commands:

```
INSERT INTO students VALUES ("adf7kd", "Anna", "Kournikova", "CpE", 3);

INSERT INTO students VALUES ("dc7ed", "David", "Cooperfield", "CS", 4);

INSERT INTO students VALUES ("ca9s", "Criss", "Angel", "BME", 2);

INSERT INTO students VALUES ("mc3tg", "Mariah", "Carey", "CS", 3);

INSERT INTO students VALUES ("slj3r", "Samuel", "Jackson", "CpE", 3);

INSERT INTO students VALUES ("ct4u", "Charlize", "Theron", "BME", 4);
```

### SELECT Example ###

To view all the tuples in the `students` table, run the following command
`SELECT * from students;`

You should get the following table:


comp_ID | First_Name | Last_Name   | Major | Year |
---------|------------|-------------|-------|------|
adf7kd  | Anna       | Kournikova  | CpE   |    3 |
ca9s    | Criss      | Angel       | BME   |    2 |
ct4u    | Charlize   | Theron      | BME   |    4 |
dc7ed   | David      | Cooperfield | CS    |    4 |
mc3tg   | Mariah     | Carey       | CS    |    3 |
slj3r   | Samuel     | Jackson     | CpE   |    3 |



What happens if you try to insert the following:

`INSERT INTO students VALUES ("mc3tg", "Mariah", "Carey", "CS", 3);`


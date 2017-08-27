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
	
### SELECT Example ###

Assume you have the following `students` table:

comp_ID | First_Name | Last_Name | Major | Year 
------------ | ------------- | ------------- | ------------- | -------------
adf7kd | Anna | Kournikova | CpE | 3 
dc7ed | David | Copperfield | CS | 4 
ca9s | Criss | Angel | BME | 2 
mc3tg | Mariah | Cary | CS | 3 
slj3r | Samuel | Jackson | CpE | 3 
cc4u | Charlize | Charlize | BME | 4 

Running the following command
`SELECT * from students;`
will result in the following:
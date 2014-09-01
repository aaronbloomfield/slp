SLP: CakePHP: Getting Started
=============================

[Go up to the main SLP documents page](index.html) ([md](index.md))

To get a CakePHP website up and running:

- Make sure your webserver is configured for CakePHP; you need to have URL rewriting turned on.  See the [VirtualBox image details](virtualbox-image-details.html) ([md](virtualbox-image-details.md)) page for how to do this
- You should have your MySQL datbase and login information ready, as you will need that soon.
- download the latest stable version from http://cakephp.org/ - as of the writing of this tutorial (Aug 31, 2014), the latest stable version is 2.5.3 (however, version 3.0.0 is just about to be released...)
- unzip it somewhere in your web directory (likely somewhere in `~mst3k/html/cakephp/`)
    - you probably want to rename the root zip directory (which will be something like cakephp-cakephp-ac41b42/) to something relevant (such as the previuosly mentioned cakephp/)
-- take note of the non-domain part of the URL, as we will need this later.  If the CakePHP files are at http://server/~mst3k/cakephp/ (where "server" is the server used for the course), then we will need to remember the `/~mst3k/cakephp/` part
- configure the CakePHP installation
    - there are three .htaccess files (in the root CakePHP directory, in app/, and in app/webroot/).  Each one will need to be edited and a line like `RewriteBase /~mst3k/cakephp/` put in.  Put this in as the third line (after the `<IfModule mod_rewrite.c>` and `RewriteEngine on` lines).  This URL part should be what you remembered from above.
    - in app/Config/, copy database.php.default to database.php, and edit the first array (the `public $default = array(` one) changing three lines: login (your MySQL userid), password, and database name (likely your userid if you are doing this on a departmental server).  Note that the other values (datasource and host) should be left to their defaults.
    - edit app/Config/core.php, and change two values: Security.salt and Security.cipherSeed.  Change them to some other random value of the same length.  Note that the first is alphanumeric, whereas the second is just numeric.
- the app/tmp directory must be writable.  From the root CakePHP directory, enter `chmod -R 777 app/tmp/`
- At this point, you should be able to view the page.  It should look **exactly** like the image below
  - Note that all the boxes are green!  If **ANY** of them are yellow or red, then something is wrong, and it is not configured properly
  - Also note that there are **NOT** a bunch of lines of text at the top indicating that it cannot write to the cache directory; this means that you did not execute the chmod command properly

### Troubleshooting ###

If you are getting "internal server error" messages when viewing your CakePHP site, there are two things to try:
- make sure your .htaccess files (all three!) have the RewriteBase line added
- view the Apache error logs - see the [Linux Administration] page for how to do this

If you are receiving an error such as:

```Warning (512): SplFileInfo::openFile(/home/slp/mst3k/html/blog/app/tmp/cache/models/myapp_cake_model_default_pal4ka_list): failed to open stream: Permission denied [CORE/Cake/Cache/Engine/FileEngine.php, line 313]
```

... then your app/tmp directory is not writable.  Run `chmod -R 777 ~/html/cakephp/app/tmp` (where `~/html/cakephp/app/tmp` is the path to CakePHP's app/tmp directory).

### That image ###

![](images/cakephp-initial.png)

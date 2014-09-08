SLP: CakePHP: Getting Started
=============================

[Go up to the main SLP documents page](index.html) ([md](index.md))

To get a CakePHP website up and running:

- Make sure your webserver is configured for CakePHP; you need to have URL rewriting turned on.  See the [VirtualBox image details](virtualbox-image-details.html) ([md](virtualbox-image-details.md)) page for how to do this
    - There are some updates, below, that will need to be performed to your VirtualBox image to run CakePHP
	- However, these things (the rewriting and the steps below) are all done on the course server already
- You should have your MySQL datbase and login information ready, as you will need that shortly
- Download the latest 3.0 version version from the [CakePHP website](http://cakephp.org/) - as of the writing of this tutorial (Aug 31, 2014), the latest stable version is 3.0.0-beta1
    - Even though it's a beta version, we will be using CakePHP 3.0 this year for those projects that decide to use it, and the API is somewhat different from the 2.5 API; so we'll use the latest 3.0 version for this assignment
	- Once you know the URL, you can download it directly via wget: `wget http://server/file.zip`
- Unzip it somewhere in your web directory (likely somewhere in '~mst3k/html/cakephp/')
    - Be warned that it creates all the files in the *current* directory
    - Take note of the non-domain part of the URL, as we will need this later.  If you store the files in `~mst3k/html/cakephp`, then the CakePHP website URL is `http://server/~mst3k/cakephp/` (where "server" is the server used for the course), then we will need to remember the `/~mst3k/cakephp/` part
    - If you view the page now, you will get a "404: Not Found" error, which is expected at this point
- Configure the CakePHP installation
    - There are two .htaccess files (in the root CakePHP directory (~mst3k/html/cakephp/), and in webroot (~mst3k/html/cakephp/webroot/).  Both will need to be edited and a line like `RewriteBase /~mst3k/cakephp/` put in (edited for your particular directory/URL).  Put this in as the second to last line, just above the `</IfModule>` line.  This URL part should be what you remembered from above.
        - At this point, you should be able to view your URL, although at least one of the bars will not be green (specifically the one about connecting to the database)
    - In config/app.php, edit app.php, and edit the Datasources =&gt; default array: you will need to change the values for login, password, and database to match your MySQL credentials
- At this point, you should be able to view the page.  It should look **exactly** like the image below (although, if you used a newer version than 3.0.0-beta1, the version number will obviously be different) (and the add at the top may differ, too)
  - Note that all the boxes are green!  If **ANY** of them are yellow or red, then something is wrong, and it is not configured properly

### Image update ###

There are three commands that you will need to run on the VirtualBox image in order to use CakePHP locally, as they were not run when the image was created:

```
sudo apt-get install php5-mcrypt php5-intl
sudo php5enmod intl
sudo php5enmod mcrypt
sudo service apache2 restart
```


### Troubleshooting ###

If you are getting "internal server error" or "404: File Not Found" messages when viewing your CakePHP site, there are two things to try:

- make sure your .htaccess files (both of them!) have the RewriteBase line added
- view the Apache error logs - see the [Linux Administration Howtos](linux-admin-howtos.html) ([md](linux-admin-howtos.md)) page for how to do this

### That image ###

![](images/cakephp-initial.png)

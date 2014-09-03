Linux Administration Howtos
===========================

[Go up to the main SLP documents page](index.html) ([md](index.md))

Great!  You've gotten your VirtualBox all set up and running, but now you need to administer it.  Here are a bunch of commands to help you do that.

Other problems you are running into?  Just ask!  We'll update this page as we find out what Linux administration issues people are running into.

**Adding a user:** it may be the case that you want to log in as somebody else, such as if you want the user (and thus http URL) to be the same on your Linux box as on the server you are deploying to.  To add a user, you enter: 1sudo adduser mst3k`, where mst3k is the username.  You will be prompted to enter a bunch of other information.

**To allow a user to use sudo:** the first user on a system ('student' on the VirtualBox image) is allowed to use sudo, and nobody else is.  While there are a number of ways to enable other users to use sudo (such as the 'visudo' command), the easiest is just to add that user to the 'adm group - the sudo configuration file (/etc/sudoers) states that all members of this group can use sudo (as they are considered administraotrs).  To add a user to a group, enter: `sudo usermod -a -G adm mst3k`, where mst3k is the user you are modifying.  Note that this must be done using an account that *already* can use sudo.  Furthermore, if the 'mst3k' user is logged in, s/he will have to log out and log in again before the change takes effect.

**Viewing the Apache error log:** `tail -f /var/log/apache2/error.log`.  Permission denied?  Then the user is not in the 'adm' group (see the 'to allow a user to use sudo' section, above).  This command will display the last 10 lines of the apache2 error log, and any additional lines will display as they are added to that file.

Github Webhook Configuration
============================

[Go up to the main SLP documents page](index.html) ([md](index.md))

One way that github allows for updating a website is through a web hook -- this is a URL that github will call every time a push occurs.  For repositories in the uva-slp github organization, there are two steps that must be done by the course instructors (steps 2 and 6).

1. First, you need to clone the repository, and be able to update that clone, without always entering a password.  This is typically done through the user of a [deploy key](https://developer.github.com/guides/managing-deploy-keys/#deploy-keys) -- a SSH public key that is allowed to do that for a single repository.  Under the team account on the course server, run `ssh-keygen`.  This will create two files: `~/.ssh/id_rsa.pub` and `~/.ssh/id_rsa`.  These are the RSA public and private keys (respectively).

2. You will need to configurre the public key as your deploy key. In the uva-slp github organization, this can only be done by admins for the organization -- which means the course instructors.  Email them the public key (and ***ONLY*** the public key: `id_rsa.pub`), and they will configure it for you.  If you own the repository, then you can do it through the settings tab for that repo.

    - Note that these are tyipcally configured as read-only.  One would typically *not* be committing changes directly to the repo from a shared account.

	- The course instructor will also need to set up your user account's PHP permissions (when using your website, the web server should be running as the user of the account, and not as the default web server user).

3. Clone the repo in the account using the ssh version: `git clone git@github.com:username/repo`.  The auto-login that was done by the previous steps will not work through an https github clone URL.  Make sure the cloned repo is the right directory (`~/html`, for example).  The website should be viewable.

4. You then need to set up a *webhook* -- this will have github force an update of the repo on every push.  Consider the following PHP code:
```
<?php
$logfile = "/home/slp/team/html/autodeploy.log";
$fp = fopen($logfile,"a");
fprintf ($fp, "Auto-deploy at " . date("r") . " from " . (isset($_SERVER['REMOTE_ADDR'])?$_SERVER['REMOTE_ADDR']:"(no ip)") . "\n");
fprintf ($fp, "Commit info: " . (isset($_POST['payload'])?$_POST['payload']:"(no payload)") . "\n\n");
fclose($fp);
echo `cd /home/slp/team/html/; git pull`;
?>
```
A few notes about this code:

    - Note that in the script above, only the last line does the update (via the `git pull`); the rest is logging.
	- The logging should *not* be in your your cloned repo on the course server.
    - You will need to change the paths -- replace `team` with your team's account name (which is also your project tag).
    - The path above (`/home/slp/team/html`) is probably different for Django projects; those will likely have a path similar to `/home/slp/team/team/`.
    - Django projects will also have to run `python manage.py collectstatic` from the appropriate directory (see [here](http://stackoverflow.com/questions/8705305/automated-django-receive-hook-on-server-respond-to-collectstatic-with-yes) for how to prevent it from prompting you each time); the command might look like:
```
echo `cd /home/slp/team/team; python manage.py collectstatic --noinput`
```
    - Django projects will also have to reload the web server (or, more easily, touching wsgi.py); the command might look like the following (but make sure the path to wsgi.py is correct!):
```
touch /home/slp/team/team/wsgi.py
```
5. Try calling it: the URL will be something like `http://server/~team/github-hook.php` -- it should work.  If not, then you will need to figure out what went wrong -- and the details for that are in the apache2 error log.  To view this on the course server, enter `view-apache2-log`.  Keep this command running (perhaps by running with `view-apache2-log &`), as it will print out new entries to the error log as they appear.  If you are on your own machine, or you have the appropriate access, you can run `tail -f /var/log/apache2/error.log` instead (possibly with a ` &` at the end).

6. You then enter a webhook (under the repository's settings, select "webhooks & services").  For repositories that are part of the organization (such as the https://github.com/uva-slp repos), only the course administrator (i.e., the course instructor) can enable web hooks, so send him/her the URL of your web hook script.  Enter the URL, in the previous paragraph, of your webhook.  Now, every time that github receives a push from anybody, it will call that webhook, which will update the website.

7. Do a test commit to make sure it works, and then push that commit.

8. ***IMPORTANT NOTE:*** If a file is *locally* modified (meaning somebody updates a file on the course server), then git will ***NOT*** update that file from the repo!  This is why you should not modify code on the server -- instead, modify it on your local machine, push the result, and let the webhook modify the server.

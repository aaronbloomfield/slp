Running a Development Environment
=================================

[Go up to the main SLP documents page](index.html) ([md](index.md))

There are a number of ways to run a development environment that will allow you to do the work in this course.  They are all listed below; choose the one that you feel will work best for you.  There is no one particular solution that is better than the others -- they all have pros and cons.

With both VirtualBox and Docker, you have two operating systems -- the 'host' machine (what booted your computer) and the 'guest' machine (what is running in VirtualBox or Docker).

VirtualBox image
----------------

We have a VirtualBox image all created that has the necessary software installed.  This is very similar to the image used in CS 2150 in the past.  Due to updated versions of the software, however, you should *not* use your old CS 2150 image.  The image will require a XXX Gb download; once uncompressed, it will take up about XXX Gb on your machine.

Pros:

- This is a "one stop shop" where all the required software (editors, servers, etc.) are all on one machine.
- It requires the least amount of setup of the various options.

Cons:

- VirtualBox is a bit wonky, and often runs into various bugs and what-not.
- It takes up a lot of memory -- you should allocate 2 Gb to the image.  If your computer is low on memory, then it will run very slowly.
- Transfering files between VirtualBox and your host computer can be a pain -- fortunately, we are using git, so that won't be necessary all that much.
- When the wireless network connection drops, this will disable network access in the client.  This can happen when your computer transitions from one wireless access point to another, or when you put your computer to sleep, or if you go out of range.  You can either restart the network subsystem or reboot the guest operating system.
- Some of the meta keys (control, alt, and the Mac command key) will not always work the same as you would expect, which can frustrate some people.


Docker image
------------

We have a docker image ready for you to use.  To download it, do XXX.  You download a docker image, and when run, it's called a container (you can run many containers from the same image).  Note that once a container is destroyed, *ALL* files on that container are removed; if you start up another container from an image, then you get the default files.

Pros

- This takes up less memory than Virtual Box and will run faster.
- This takes up *far* less hard drive space (only 2 Gb rather than XXX Gb)
- Your editing will be in your host operating system using your favorite editor.
- Much easier to upgrade (once the docker image is updated, just do a `docker pull` and that's it)

Cons

- You will have to configure a mapping of a Docker container's directory to your home directory (we provide directions).
- You can't use MySQL for the DB access, so you will have to use SQLite in your container, and MySQL on the servers.  This isn't difficult, but it will be something to remember.  (There are ways to run MySQL on Docker, but it's a pain to manage).
- You will have to attach to the container to kick off the development servers.


Install your own
----------------

You can install everything on your own machine.  While we can help if you run into problems, we may not be able to debug everything if you run into an oddball problem.

- Ubuntu Linux: If you are running a Ubuntu version of Linux, then the install is easy -- just follow the directions listed on the [VirtualBox image details](virtualbox-image-details.html) ([md](virtualbox-image-details.md)) page.
- Ohter flavors of Linux: While we don't have much familiarity with them, it should be (relatively) easy to install on them, as many of hte packages and what-not will be similar.
- Windows: There are many tutorials online about installing your development framework and git on Windows.  You will have to be aware of a number of things:
    - If you do not use Apache2 as your webserver (IIS is Microsoft's web server), then you will have to ensure that all your configurations work on Apache2 and Linux, which is what the server is running
	- For that matter, you will *always* have to make sure things work on Apache2 and Linux
	- Windows handles text files differently than Linux; this will be an issue when you edit files.  See [here](https://en.wikipedia.org/wiki/Text_file#Formats) for details.  You will have to convert things to Linux format (there are tools for that) to keep things sane.
- Mac OS X: there are also many tutorials online about installing your development framework and git on a Mac.  You will have to be aware of a number of things:
	- You will *always* have to make sure things work on Apache2 and Linux
	- Macs handles text files differently than Linux -- not as bad as Windows, but not perfect.  This will be an issue when you edit files.  See [here](https://en.wikipedia.org/wiki/Text_file#Formats) for details.  You will have to convert things to Linux format (there are tools for that) to keep things sane.

Run on the course server
------------------------

You are certainly welcome to run your code on the course server, which is all set up for the software needed in this course.  You can configure a copy of the project in your own user directory, and test it there.  You can *NOT* edit live code as part of your development.

Pros:

- Nothing to install!

Cons:

- You have to work through a text prompt, or map a drive (see [sshfs](https://en.wikipedia.org/wiki/SSHFS)) to edit it directly
- You will have to configure your account to run a second version of the web application
- For these reasons, this is the least popular option

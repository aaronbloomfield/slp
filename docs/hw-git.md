SLP: git/github Homework
========================

[Go up to the main SLP documents page](index.html) ([md](index.md))

### Introduction

This assignment is meant to get everybody up to speed on using git and github.  You should read the [Introduction to git and github](git-intro.html) ([md](git-intro.md)) first.

### github.com

First, create an account on [github.com](https://github.com) if you do not already have one.  You should then [request an educational discount](https://education.github.com/) -- this will allow you to have a number of private repositories (5, in fact).  Approval of the student discount may take some time.  If you wait until the last minute, and do not get the discount in time, then you will have to purchase private repositories.  Thus, please start this part early.

For this assignment, you will need to create a **private** repository named `cs4970-git-hw`.  It **MUST** be named this, or else we will not be able to find it, and we will assume that you did not complete the assignment.  There will be only one private repository needed in this assignment.  The actual location of your repository will be: http://github.com/username/cs4970-git-hw (where "username" is your github username).  As mentioned in the course syllabus, if you do not have any private repositories available, you will have to purchase more.

Within that repo, you will need to complete each of the following sections for this assignment.  Note that you may need to enable the wiki and issue sections: from the main repo page, click on Settings (in the right-hand column), and under Features you can enable wikis and issues.

### Allow us to view the repo

The course instructor(s) and any graduate TA(s) need to be able to view the private repos that you create.  From the main repo page, click on Settings (in the right-hand column), click on Collaborators (in the left-hand column), and enter our github usernames.  You can find the appropriate github usernames in the [daily announcements slide](../uva/daily-announcements.html#/) set.

However, please ***ADD US AFTER YOU HAVE FINISHED THE HOMEWORK***.  This prevents us from getting notifications every time you make any change to your repo (edit a wiki page, add an issue, etc.).  We are going to get really cranky -- and take points off! -- if we get a dozen emails from every person's repo as they are working through this homework.  Since there are many students in the class, that's well over 1,000 github emails that we would rather not have to deal with.

### Update your profile picture

The default profile pictures are just patterns.  Make yours interesting!  You can change your profile picture pretty easily.  From your main profile page (https://github.com/username), click on "Edit Profile", then "Upload new picture".  If you have already changed your picture away from the default, then ignore this step.  And you don't **have** to do this.  But all the *cool* people are doing it...

### Wiki

Create a bunch of wiki pages.  Some of them must link to other pages.  And use a reasonable amount of Markdown to format your pages.  We don't care about the content -- we care that you know how to use the wiki, you know how to create and link to pages, and you know how to use Markdown.  You should have both internal links (to other wiki pages in that repo) as well as external links (to other URLs).

You should create at least one table.  So that we can easily find your markdown'ed code, we are just going to look on the main wiki page to see your prowess at markdown -- we will only check the other pages to ensure you know how to link from one page to another.

### Issues

- Create a bunch of issues; we don't care what their content is.  Entertain us, but be appropriate, please.  Use Markdown in them for formatting.  Because we don't want to look at *all* of your issues to find the one that shows that you can use Markdown, you should use Markdown in *all* of them.  You can create some Markdown text and cut-and-paste that into the other issues for this.
- Create some new labels.   Label a bunch of issues with different labels.
- Create a milestone.  Make some (but not all!) of your issues part of that milestone.
- [Resolve an issue via a commit message](https://help.github.com/articles/closing-issues-using-keywords/).  So that we can find the issue that you have resolved this way, let's say you will resolve issues #10 and #12 this way.  This means we will be looking at https://github.com/username/cs4970-git-hw/issues/10 and https://github.com/username/cs4970-git-hw/issues/12 to find these issues.

### Forks and pull requests

You should do a fork of the `uva-slp/git-hw-to-fork` repository that was set up for this assignment ([https://github.com/uva-slp/git-hw-to-fork](https://github.com/uva-slp/git-hw-to-fork)). You can do this by clicking on the "Fork" button on the upper-right when viewing that page.  Clone your forked repo, and make some change.  Make sure that this change involves just some addition -- if you everybody edits an existing file, then we have to deal with merging 100+ different edits to the same file, and we have better things to do.  So your change can be anything, but please keep it appropriate, as this is a public repo (and your forked repo can also be a public repo).  Entertain us!

Once you have made your change, commit and push.  You should then submit that change as a pull request; see [here](https://help.github.com/articles/creating-a-pull-request) for details. 

This is the only section that uses the `git-hw-to-fork` repo; all other sections use the `cs4970-git-hw` repo.

### Branches and merging

Git provides a visual graph of the branches of a repo via the network tool -- it's found off of the graphs link (which is on the right side of the main github repo page).  Alternatively, you can find it at https://github.com/username/repo/network.

Do the branching and merging required to make the network graph look like the last image on the [git branching and merging page](http://git-scm.com/book/en/Git-Branching-Basic-Branching-and-Merging) that you read in the [git intro](git-intro.html) ([md](git-intro.md)).  That image can be found [here](http://git-scm.com/figures/18333fig0317-tn.png).  Hint: that page describes the commands (and their order) necessary to make the graph look like that.  You are welcome to have other commits, of course -- we just want to see that particular structure in there somewhere.  You should then remove all the extra branches (other than master, of course) once you have created those branches and merges.

### Complicated merging

In this part, you will need to perform a complicated merge -- meaning one that git cannot automatically perform for you.

- Pick some file.  While it doesn't matter what it is, it will have to have a number of lines of text (or source code, or whatever) to modify.
- Create a separate branch, and call it whatever you want
- Edit the **same** part of the file in both branches, committing after each edit.  However, the changes must be *different* (if you make the *same* edit in both branches, there is nothing to merge!)
- Merge the branches.  git will create a conflict-resolution file, which you **MUST** copy (and commit) as conflict.merge.txt so that we can see it.  Fix the conflicts (however you would like), and finish the merge of those branches.
- Delete any extra branches other than master.

We will be looking at https://github.com/username/cs4970-git-hw/blob/master/conflict.merge.txt to see the file; if that URL doesn't work (replacing 'username' with your github username), then you will not receive credit for this part.

### Finishing up

Now that the homework is done, please don't forget to add us as collaborators!  See above for details.

### Grading

First, the easy ways to get zero credit:

- Did you create the required `cs4970-git-hw` repo?  If not, then zero credit
- Is the repo private?  If not, then zero credit.
- Can both the instructor and TA access the repo?  If not, then zero credit.

Still with us?  Great!  We are going to check that you have completed the various parts described above.  Be sure that the conflict.merge.txt file, from the "complicated merging" section is included.

In particular, we are going to check that those *specific* issues (#10 and #12) were resolved via a commit message.

### Releasing the private repository

The `cs4970-git-hw` repository is private.  You will need to keep this repository around until the assignment is graded.  If you want to request a regrade, then keep it around until the regrading process has completed.  Once you have received the grade, and are either satisfied with it or have gone through the regrade process, feel free to remove that repo.  Similar rules apply with the `git-hw-to-fork` repo, although since that is public, it does not count against your quota of private repositories.

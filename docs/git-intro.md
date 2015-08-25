SLP: Introduction to git and github
===================================

[Go up to the main SLP documents page](index.html) ([md](index.md))

People familiar with git and github can skip some parts, depending on your level of familiarity.  However, it would be wise to skim the parts you are skipping.  And everybody must read the section on tags and branches, as most people who think they know about branching really don't.

### Introduction

First, read through the [Wikipedia page on git](http://en.wikipedia.org/wiki/Git_%28software%29).

Git keeps the files, as well as the entire the version history, in a *repository*.  A repository is just a directory structure created via the `git init` command (which is not being gone over here).  However, most people who use git have the repository kept on a server.  One can set up their own server to do so, although the user then has to deal with authentication and authorization (who can access which repositories, etc.).  Many people use [github](https://github.com), which is what we will be using here.

The repository itself needs to be *cloned* in order for the files to be worked on; this is analogous to svn's checkout command.  This is done via the `git clone` command.  There are a few ways to clone a repository:

```
git clone git@github.com:aaronbloomfield/slp
```

Or:

```
git clone https://github.com/aaronbloomfield/slp.git
```

For both of these commands, the user who owns the repository is `aaronbloomfield`, and the repository is `slp`.  Either of the two commands will work, although the first one is considered more secure (see [here](https://help.github.com/articles/which-remote-url-should-i-use) for details).  And both will require you to enter your password any time you clone a private repo, and any time you *push* (more on pushing, below) to any repository.

Once the repository is cloned, you can work on the files within the created directory.  The git commands will work anywhere in that cloned repository -- you don't have to be in the root of that directory structure (which was the case with subversion).  As you create files that you want to add to the repository, you use `git add <file>`.  This stages them for (i.e., gets them ready for) adding to the repository, but does not add them just yet.  When you have finished making your changes (file additions, edits, or deletions), you then *commit* them.

Git is a distributed system, which means that you can make as many commits as you would like, but they are not sent back to the git server (in our case, github.com) until you instruct git to do so via a `git push`.

### git command summary

- `git clone git@github.com:username/repo`: clones a repository; a directory named the same name as the repository is created, and the files are placed there.
- `git add <file>`: stages a file to be added to the repository on the next commit.  You can call this on a directory, and git will stage the entire directory structure.
- `git commit`: commits the current changes (additions, deletions, and edits).  A few useful flags:
    - `-m "message"` will use that as a message as the commit message (otherwise git will prompt you to enter a message)
    - `-a`: by default, an edited file is not included in the list of files to be committed unless you add it via `git add`; this flag adds all edited files (but not all new files; they still have to be manually added via `git add`.
- `git status`: this displays a number of things:
    - the files that are staged for commit
    - the files that have been edited sine the last commit
    - the number of commits that you are "ahead" of the server (meaning how many times you have called 'git commit' without calling 'git push')
    - note that if you edit a file that is already in the repo, add it via `git add`, then edit it again, it will appear twice: as a file staged for commit (the version of the file after the first edit) and in the unstaged files list (the version after the second edit); calling `git add` on that file again, or committing with `-a` will cause the most recent edited version to be committed.
- `git push`: this pushes all the local commits to the server's version of the repository.
- `git pull`: this obtains the latest version of the repository from the server, as other people may have pushed changes to the repo since you last did a `git clone` or `git pull`.  Note that if one of the files modified in the server's repository is the same as a file you have modified locally, then git will attempt to merge those files; more on this below.
- `git mv <source> <dest>`: moves a file (or directory), or renames a file; this tells git about the move or rename.
- `git rm <file>`: stages a file (or directory) for removal; use the `-f` flag if that file has been edited since the last commit; use the `-r` flag to remove a directory.
- `git checkout`: this has two main functions
    - `git checkout <file>`: this reverts the (presumably edited) file to the version as of the last commit.
    - `git checkout <branch>`: this will switch the *entire* repo over to the branch specified.
- `git reset`: this allows you to "undo" various operations
    - If you want to unstage a file for commit, then enter `git reset <file>`
	- If you want to undo the last commit, you can enter `git reset --soft HEAD~1`.
- `git branch`: creates (or edits) branches; see below for details
- `git merge`: merges branches; see below for details
- `git tag`: creates (or edits) tags; see below for details
- `git update-index --assume-unchanged  <file>`: if there is a file that you want to modify from the version in the repo, but you do NOT want those changes to be committed, you use this command.  It's useful if you are updating passwords in a file, but you don't want those passwords stored in the repo.

There are many other git commands; type `man git` for a list, then type `man git-clone` for the details about the `git clone` command (likewise for the other git commands).

### Exploring github

There are many services that provide git repositories.  [Github](https://github.com/) is one of the most popular.  They provide free repositories for education, and have a powerful set of tools available through their web interface for managing your git repositories.

Github defines two types of repositories: public and private.  Anybody can create as many public repositories as they want, but note that anybody else can see the public repositories.  This course (and this document) is available in a public repository.  In contrast, private repositories can only be viewed by those who are authorized to view them.  Private repositories [cost money](https://github.com/pricing), although github provides free repositories for educational use.

When viewing any repository, it will display, underneath the list of files, the contents of a file called README.md (or readme.md, or readme.markdown, etc.).  [Markdown](http://daringfireball.net/projects/markdown/syntax) is a plain text formatting system that aims to make the document readable in both text format as well as easily allowing it to be converted into HTML or similar.  In fact, any directory that has a readme.md (or similar) will display the formatted contents underneath the list of files.

Note on the right-hand side that github provides issues and a wiki (both of which can be formatted with Markdown).  Take a moment to explore these, as they will be used extensively in this course.  For the assignment below, you will need to created formatted wiki pages, and create issues (along with labels and milestones).

Also on the right-hand side is the link for settings.  From there, you can select Collaborators, and select other people who can access the repository.  If the repo is public, then anybody can clone it. But only collaborators can push to a public repo, or have any access (clone and push) to a private repo.  You will need to add me ('aaronbloomfield') as a collaborator to your repo for this homework.

### Forks and Pull requests

Github easily allows one to [fork](http://en.wikipedia.org/wiki/Fork_%28software_development%29) a repository that they have access to.  This is a github feature; there is no git command named "fork".  Basically, this is a means to create a new repo that is a copy of an old repo, and keep track of which repo is the old repo.  As you have created the forked repo, you can make any changes to it.  This is typically done in two different situations.  One is if you want to suggest a change to be sent back to the source repo; this is called a pull request, and is described shortly.  The other reason is if you want to work on a project that is based off of another project.  For example, the [aaronbloomfield/github-api-tools](https://github.com/aaronbloomfield/github-api-tools) is a fork of the [KnpLabs/php-github-api](https://github.com/KnpLabs/php-github-api) repo, as the former uses the library that was created in the latter.  However, the fork adds features that are not meant to be in the original (i.e., the fork is an application that uses the original).

There are two ways that people can change a repository.  The first is if the user has permissions to push to a repo.  This is the case for any user's own repositories, and will be the case for the group repositories in this course.

However, it may be that one wants anybody to be able to suggest changes to a repository.  In this case, the user creates a fork, makes changes to the fork, and then suggests those changes back to the maintainer of the original repository.  This is called a *pull request* (again, this is a github name, and is not a git command, per se).  This will send a notification -- or request -- to the maintainer of the original to "pull" the changes into the original repository (hence "pull request").  To submit a pull request, click on the "pull requests" button on the right side of the github repository page.

When you create a fork, it copies the original repo at that point in time.  To update the original repo, you will want to follow the directions [here](https://help.github.com/articles/syncing-a-fork).  Note that you will likely have to [configure the upstream repo](https://help.github.com/articles/configuring-a-remote-for-a-fork) the first time you do this.

### Tagging

A tag is a way to denote a particular commit or a particular date.  For example, version releases are typically tags.  This differs from a branch, which is when development is going to go in multiple different directions.  A tag is just a means to demarcate a point in time.  Consider the main [linux kernel repo](https://github.com/torvalds/linux) -- there are a *lot* of tags there.

In the examples below, we use "v1.4" as the tag name, but it can be any string.

- To list all the tags in a repo, enter `git tag`.
- To create tag, enter `git tag -a v1.4 -m 'my version 1.4'`
- To find out information about a tag, enter `git show v1.4`.  Note that this shows the final commit that was tagged (and thus the diffs from that commit to the previous).
- If you create a tag in a cloned repo, that tag only exists on your local machine.  To send those tags to the git server (i.e., github), enter `git push origin --tags`.
- To delete a tag, enter `git tag -d v1.4`; you will also have to push this back to the git server via `git push origin :refs/tags/v1.4`
- To switch to a tagged version, enter `git checkout tags/v1.4`.

More information about tagging can be found [here](http://git-scm.com/book/en/Git-Basics-Tagging); the examples in this section were taken from that page.

### Branching and Merging

First, a comment about what branching is for.  Branching is for when you want a *separate* development path to occur in *conjunction* with the original development path.  It is **NOT** to indicate a particular commit or a particular date (that's a tag).  Branching is a wonderful and powerful tool -- but, like all tools, it should be used wisely and for it's proper purpose.  Linus Torvalds, the creator of git and the Linux kernel, has only one branch in his [linux kernel github repo](https://github.com/torvalds/linux), albeit a very large number of tags.  (Granted, he doesn't use github as his primary workflow git server, but still...)

Branching is merging is complicated, and one should read [this page](http://git-scm.com/book/en/Git-Branching-Basic-Branching-and-Merging) in it's entirety.

A few notes from that page (this is NOT a summary):

- The main branch is called "master"
- `git checkout -b foo` creates a "foo" branch from the current branch/commit the repo is currently in.
- `git checkout master` changes to the "master" branch, re-arranging the files as needed.
- Entering `git merge foo` will merge the "foo" branch INTO the current branch
- If you have updated a branch, such as "master", and want to "pull" those changes into another branch, such as "foo", then you enter (while in the "foo" branch): `git rebase master`
- `git branch -D foo` will delete the "foo" branch, but only locally; to do so remotely, use `git push origin --delete foo`

The rest of branching and merging should have been covered in that document

### git for those coming from subversion

While there are many differences between git and subversion, there are also similarities.  Here are the equivalent git commands, as much as is possible, to the SVN commands.  Note that the *flags* to these commands are not listed below.

- `svn checkout` is `git clone`.  Note that how URLs are specified for the two will be different.
- `svn commit` is `git commit` followed by a `git push`.  When you do a commit in SVN, it sends the changes back to the SVN server.  A git commit only does a local commit, but does not send the changes back to the git server until you do a push.  Under git, you can have as many commits as you want between pushes (although having too many somewhat defeats the purpose of the git server that you are using).
- `svn revert` is `git checkout`.  This is to restore a file to the last checked-in version.
- `svn update` is `git pull`, and largely operates the same way (although merging of files is much easier under git).


A bunch of SVN commands operate the same (more or less) under git:

- `svn status` is `git status`
- `svn add` is `git add`
- `svn move` is `git mv`
- `svn diff` is `git diff`.  Note that, under git, files that you have added will not have their diffs displayed via this command.

And some SVN commands have no equivalent under git:

- `svn mkdir` does not have an equivalent under git.  git does not keep track of directories, and any empty directories are automatically removed.  So if you want to create an otherwise empty directory, create an file called "empty" inside (`touch empty`), and add that via `git add`.  This will automatically add the directory as well.
- `svn copy` doesn't have a git equivalent.  Just copy the file normally and add the copied file.

SLP: Frameworks Expansion Homework
==================================

[Go up to the main SLP documents page](index.html) ([md](index.md))

In this homework, you will expand upon the frameworks that you created in the main [frameworks homework](hw-frameworks.html) ([md](hw-frameworks.md)).  For each framework, there is a list of features to add to the site.

***IMPORTANT:*** As your original frameworks homeworks have likely *NOT* been graded yet, you must preserve all of the original functionality in those homeworks, while adding in the required functionality.

### Django / Python

You will need to add these features

- On the landing page (http://server/django/mst3k), put in some CSS.  We aren't looking for much, but it should be a reasonably formatted page.  We want to see that you are using a .css file from your static directory.  And change enough around so that it's obvious that you used CSS.  If you don't know CSS, you are welcome to download and existing .css file and use that to format your page -- this is about getting a CSS file to work, not writing CSS code.
- Add users with permissions.  This is separate from the admin console -- your system should support users as well.  There are two user types:
    - Regular users: can create polls and remove polls (but only if they created them!)
    - Admin users: can do what regular users can do, plus they can create other users of both types (passwords are set when the user is created; you don't have to program in password changing)
    - To test this, we will log in as an admin (kermit/frog1234), try creating users, try creating (and using and deleting) polls, ensure that regular user 1 cannot delete the poll of regular user 2
- Note that all polls are public, and all will display on the landing page (there are no per-user pages)
- Also note that the original functionality (from the main [frameworks homework](hw-frameworks.html) ([md](hw-frameworks.md)) should still be present -- it's fine if there is more (CSS, more polls), but the original one should stay there.

A couple of notes:

- You can use the built-in user model for this, but the admin needs to be able to create a user without going into the built-in admin panel
- Passwords can be set via the built-in `user.set_password()` function
- It's fine if the link to create users just doesn't appear for regular users (this is not particularly secure, and not viable for the project, but okay for this homework)

### Ruby / Rails

- This will be essentially identical to the Django version (create users with permissions, etc.), except that the users will be able to manage blogs instead of polls. All blog posts are to appear on the main landing page.  We will use the same admin login (kermit/frog1234) to test this.

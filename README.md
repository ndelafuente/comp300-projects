# COMP 300 Projects
## Group Alpha | *Summer 2021*

This is a repository for the group projects that were completed in Principles of Digital Hardware with Chuck Pateros.

### Working on the Ubuntu RaspberryPi (uPi)
1. Power on the uPi to the uPi and open the command line (directly from the desktop or through SSH).

2. Clone this repository onto the uPi into a new folder `comp300-projects`.

        git clone https://github.com/ndelafuente/comp300-projects.git

3. Change directory to `comp300-projects`.

        cd comp300-projects

4. To set your account's default identity. Run:

        git config user.email "you@sandiego.edu"
    <!-- separate commands -->
        git config user.name "Name"

Now you should be able to push and pull from this repository!
<br>
<br>
Some useful commands:

Updating your local version of the repository (always try to do this before pushing)

    git pull --rebase

Checking what files have changed

    git status

Adding changes or new files to the repository

    git add <filename(s)>

Committing changes to the repository

    git commit -m "<Description of changes>"

Pushing changes to the repository

    git push
REM $Id$

rsync -v -c -r -e ssh --delete --force --exclude stats/ /cygdrive/h/web/ apache@home.epeios.org:/var/www/www

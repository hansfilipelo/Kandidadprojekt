#!/bin/sh

#  LaTeXLoop.sh
#  
#
#  Created by Hans-Filip Elo on 2014-02-24.
#

# cd to script dir
DIR=$( cd "$( dirname "$0" )" && pwd )
cd $DIR

while [ true ]
do
    /usr/bin/git pull --commit | grep "Already up-to-date."

    if [ $? -ne 0 ]
    then
        /usr/texbin/pdflatex -halt-on-error *.tex
    fi

    sleep 5
done

#!/bin/sh

#  LaTeXLoop.sh
#  
#
#  Created by Hans-Filip Elo on 2014-02-24.
#

# cd to script dir
DIR=$( cd "$( dirname "$0" )" && pwd )
cd $DIR

FILE=$1

while [ true ]
do
    /usr/bin/git pull --commit
    /usr/texbin/pdflatex $FILE
    sleep 15
fi

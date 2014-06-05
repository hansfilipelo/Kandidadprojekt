#!/bin/bash

# Variables
# cd to script dir
DIR=$( cd "$( dirname "$0" )" && pwd )
cd $DIR
# OUT
OUTFILE=latexout.txt

touch $OUTFILE


cd ../Kommunikation
pdflatex -interaction=nonstopmode kom.tex > $DIR/$OUTFILE 2>&1
cd ../Reglering
pdflatex -interaction=nonstopmode regler.tex >> $DIR/$OUTFILE 2>&1
cd ../Sensorer
pdflatex -interaction=nonstopmode Sensorer.tex >> $DIR/$OUTFILE 2>&1
cd ../Teknisk_Dokumentation
pdflatex -interaction=nonstopmode tekdok.tex >> $DIR/$OUTFILE 2>&1
cd ../../Designspec
pdflatex -interaction=nonstopmode Designspec.tex >> $DIR/$OUTFILE 2>&1
cd $DIR
pdflatex -interaction=nonstopmode kappa.tex >> $OUTFILE 2>&1 
grep -w ".*Warning:" $OUTFILE
grep -w ".*Error:" $OUTFILE

if [ $? != 0 ]
then
	rm $OUTFILE
fi


#!/bin/bash
# cd to script dir
DIR=$( cd "$( dirname "$0" )" && pwd )
cd $DIR
touch latexout.txt

cd ../Kommunikation
pdflatex kom.tex > $DIR/latexout.txt 2>&1
cd ../Reglering
pdflatex regler.tex >> $DIR/latexout.txt 2>&1
cd ../Sensorer
pdflatex Sensorer.tex >> $DIR/latexout.txt 2>&1
cd ../Teknisk_Dokumentation
pdflatex tekdok.tex >> $DIR/latexout.txt 2>&1
cd ../../Designspec
pdflatex Designspec.tex >> $DIR/latexout.txt 2>&1
cd $DIR
pdflatex kappa.tex >> latexout.txt 2>&1 
grep -w "Warning:" latexout.txt


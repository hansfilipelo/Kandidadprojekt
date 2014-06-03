#!/bin/bash
# cd to script dir
DIR=$( cd "$( dirname "$0" )" && pwd )
cd $DIR

cd ../Kommunikation
pdflatex kom.tex
cd ../Reglering
pdflatex regler.tex
cd ../Sensorer
pdflatex Sensorer.tex
cd ../Teknisk_Dokumentation
pdflatex tekdok.tex
cd ../../Designspec
pdflatex Designspec.tex 
cd $DIR
pdflatex kappa.tex
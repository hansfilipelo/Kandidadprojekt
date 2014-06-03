#!/bin/bash
# cd to script dir
DIR=$( cd "$( dirname "$0" )" && pwd )
cd $DIR
touch latexout.txt


cd ../Kommunikation
pdflatex -interaction=nonstopmode kom.tex > $DIR/latexout.txt 2>&1
cd ../Reglering
pdflatex -interaction=nonstopmode regler.tex >> $DIR/latexout.txt 2>&1
cd ../Sensorer
pdflatex -interaction=nonstopmode Sensorer.tex >> $DIR/latexout.txt 2>&1
cd ../Teknisk_Dokumentation
pdflatex -interaction=nonstopmode tekdok.tex >> $DIR/latexout.txt 2>&1
cd ../../Designspec
pdflatex -interaction=nonstopmode Designspec.tex >> $DIR/latexout.txt 2>&1
cd $DIR
pdflatex -interaction=nonstopmode kappa.tex >> latexout.txt 2>&1 
grep -w ".*Warning:" latexout.txt
grep -w ".*Error:" latexout.txt


#!/bin/bash

files=$(find ../../ -name *.hpp)

for f in $files
do
	echo $f
	
	f1=$f"1"
		
	perl -pe "s/$1/$2/" $f > $f1
	
	rm -f $f
	
	cp $f1 $f

	rm -f $f1
done



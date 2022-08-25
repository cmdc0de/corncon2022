ls *.c | while read line
do
	#echo $line
	echo "#include \"gimp_type.h\"" > tmp
	cat $line >> tmp
	mv tmp $line
done

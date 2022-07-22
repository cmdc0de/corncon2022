ls *.c | while read line
do
	mv $line ${line}pp
done

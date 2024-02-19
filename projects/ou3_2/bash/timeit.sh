rm ttiming.txt mtiming.txt atiming.txt Smtiming.txt Satiming.txt
touch ttiming.txt mtiming.txt atiming.txt Smtiming.txt Satiming.txt

for i in 10000 13000 16000 19000 22000;
do
    echo
    echo Running sample size $i...
    for j in 1 2 3 4 5 6;
    do
	echo Running tabletest $j/6...
	./table0test -t -n $i >> ttiming.txt
    done

    for j in 1 2 3 4 5 6;
    do
	echo Running mtftabletest $j/6...
	./table1test -t -n $i >> mtiming.txt
    done

    for j in 1 2 3 4 5 6;
    do
	echo Running arraytabletest $j/6...
	./table2test -t -n $i >> atiming.txt
    done

	for j in 1 2 3 4 5 6;
    do
	echo Running Smtftabletest $j/6...
	./table3test -t -n $i >> Smtiming.txt
    done

	 for j in 1 2 3 4 5 6;
    do
	echo Running Sarraytabletest $j/6...
	./table4test -t -n $i >> Satiming.txt
    done
done

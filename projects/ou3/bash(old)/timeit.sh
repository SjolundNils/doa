rm ttiming.txt mtiming.txt atiming.txt
touch ttiming.txt mtiming.txt atiming.txt

for i in 10000 13000 16000 19000 22000;
do
    echo
    echo Running sample size $i...
    for j in 1 2 3 4 5 6;
    do
	echo Running tabletest $j/6...
	./tabletest -t -n $i >> ttiming.txt
    done

    for j in 1 2 3 4 5 6;
    do
	echo Running mtftabletest $j/6...
	./mtftabletest -t -n $i >> mtiming.txt
    done

    for j in 1 2 3 4 5 6;
    do
	echo Running arraytabletest $j/6...
	./arraytabletest -t -n $i >> atiming.txt
    done
done

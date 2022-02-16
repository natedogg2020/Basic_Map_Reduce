.PHONY: run

run:
	chmod +x mapreduce
	chmod +x mapper
	chmod +x reducer
	./mapreduce 5 2 test/T1/F1.txt

t1f:
	chmod +x mapreduce
	chmod +x mapper
	chmod +x reducer
	./mapreduce 5 2 test/T2/empty

t1mltr:
	chmod +x mapreduce
	chmod +x mapper
	chmod +x reducer
	./mapreduce 0 2 test/T2/empty.txt
t1m:
	chmod +x mapreduce
	chmod +x mapper
	chmod +x reducer
	./mapreduce 0 -1 test/T2/empty.txt

t1r:
	chmod +x mapreduce
	chmod +x mapper
	chmod +x reducer
	./mapreduce 5 0 test/T2/empty.txt

t2:
	chmod +x mapreduce
	chmod +x mapper
	chmod +x reducer
	./mapreduce 5 2 test/T2/empty.txt
t3:
	chmod +x mapreduce
	chmod +x mapper
	chmod +x reducer
	./mapreduce 1 1 test/T1/F1.txt
t4:
	chmod +x mapreduce
	chmod +x mapper
	chmod +x reducer
	./mapreduce 32 32 test/T3/F3.txt

clean:
	rm -rf output

valt1:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./mapreduce 5 2 test/T1/F1.txt
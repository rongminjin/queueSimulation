default: build

build:
	clang++ -c main.cpp
	clang++ -o simulator main.o
	#to run: ./simulator customerArrivalRate maxServiceTime randomSeed

data:
	python dataGenerator.py

analysis:
	rm -f bankData.txt
	rm -f supermarketData.txt
	python script.py
	python surfacePlotGenerator.py

clean:
	rm -f main.o
	rm -f simulator

all: car-rental-sim

car-rental-sim: car-rental-sim.o simlib.o
	gcc -o car-rental-sim car-rental-sim.o simlib.o -lm
	./car-rental-sim
	make clean

clean:
	rm car-rental-sim.o
	rm simlib.o
	rm car-rental-sim

car-rental-sim.o: car-rental-sim.c
	gcc -c car-rental-sim.c -o car-rental-sim.o -I.

simlib.o: simlib.c
	gcc -c simlib.c -o simlib.o -I.
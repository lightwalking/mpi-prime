mpiPrimes: mpiPrimes.o Wheel30.o MillerRabin.o
	mpic++ -o mpiPrimes mpiPrimes.o Wheel30.o MillerRabin.o

mpiPrimes.o: mpiPrimes.cpp Wheel30.h MillerRabin.h
	mpic++ -c mpiPrimes.cpp

Wheel30.o: Wheel30.cpp Wheel30.h
	mpic++ -c Wheel30.cpp

MillerRabin.o: MillerRabin.cpp MillerRabin.h Wheel30.h
	mpic++ -c MillerRabin.cpp

copy: mpiPrimes
	rsync -a /home/pi/mpi-prime/mpiPrimes swarm2:/home/pi/mpi-prime/ \
	&& rsync -a /home/pi/mpi-prime/mpiPrimes swarm3:/home/pi/mpi-prime/ \
	&& rsync -a /home/pi/mpi-prime/mpiPrimes swarm4:/home/pi/mpi-prime/

run: 
	mpirun -n 16 -hosts swarm1,swarm2,swarm3,swarm4 ./mpiPrimes > primes.txt

clean:
	rm mpiPrimes mpiPrimes.o Wheel30.o MillerRabin.o primes.txt


mpiPrimes: mpiPrimes.o
	mpic++ -o mpiPrimes mpiPrimes.o

mpiPrimes.o:
	mpic++ -c mpiPrimes.c++

copy: mpiPrimes
	rsync -a /home/pi/mpi-prime/mpiPrimes swarm2:/home/pi/mpi-prime \
	&& rsync -a /home/pi/mpi-prime/mpiPrimes swarm3:/home/pi/mpi-primes \
	&& rsync -a /home/pi/mpi-prime/mpiPrimes swarm4:/home/pi/mpi-prime

run: 
	mpirun -n 16 -hosts swarm1,swarm2,swarm3,swarm4 ./mpiPrimes > primes.txt

clean:
	rm mpiPrimes mpiPrimes.o primes.txt


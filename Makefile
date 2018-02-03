mpiPrimes: mpiPrimes.o
	mpic++ -o mpiPrimes mpiPrimes.o

mpiPrimes.o:
	mpic++ -c mpiPrimes.c++

copy: mpiPrimes
	rsync -a /home/pi/mpi-prime swarm2:/home/pi \
	&& rsync -a /home/pi/mpi-prime swarm3:/home/pi \
	&& rsync -a /home/pi/mpi-prime swarm4:/home/pi

run: 
	mpirun -n 16 -hosts swarm1,swarm2,swarm3,swarm4 ./mpiPrimes > primes.txt

clean:
	rm mpiPrimes mpiPrimes.o primes.txt


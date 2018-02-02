mpiPrimes: mpiPrime.o
	mpic++ -o mpiPrimes mpiPrimes.o

mpiPrimes.o:
	mpic++ -c mpiPrimes.c++

clean:
	rm mpiPrimes mpiPrimes.o


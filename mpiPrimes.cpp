#include <iostream>
#include <cmath>
#include <mpi.h>
#include "Wheel30.h"
#include "MillerRabin.h"

#define PRINT_PERIODIC_STATUS_ONLY

#ifdef PRINT_PERIODIC_STATUS_ONLY
#include <time>
#endif

#ifndef NUMBER
#define NUMBER unsigned long long
#endif

#ifndef MPI_NUMBER
#define MPI_NUMBER MPI_UNSIGNED_LONG_LONG
#endif

#define RANGE_SIZE 1000
#define PRIMES_ARRAY_SIZE 333

#define SET 50

#if SET==250
#define NUM_PRIMES 250000000
#define FIRST_RANGE_START 4222245001ULL
#define FIRST_RANGE_END   4222246000ULL
#define INITIAL_PRIME_COUNT 200000441
#endif

#if SET==200
#define NUM_PRIMES 200000000
#define FIRST_RANGE_START 3121245001ULL
#define FIRST_RANGE_END   3121246000ULL
#define INITIAL_PRIME_COUNT 150000285
#endif

#if SET==150
#define NUM_PRIMES 150000000
#define FIRST_RANGE_START 2038080001
#define FIRST_RANGE_END   2038081000
#define INITIAL_PRIME_COUNT 100000237
#endif

#if SET==100
#define NUM_PRIMES 100000000
#define FIRST_RANGE_START 982455001
#define FIRST_RANGE_END   982456000
#define INITIAL_PRIME_COUNT 50000154
#endif

#if SET==50
#define NUM_PRIMES 50000000
#define FIRST_RANGE_START 9
#define FIRST_RANGE_END   RANGE_SIZE
#define INITIAL_PRIME_COUNT 4
#endif

#ifndef FIRST_RANGE_START
#define NUM_PRIMES 10000000
#define FIRST_RANGE_START 9
#define FIRST_RANGE_END   RANGE_SIZE
#define INITIAL_PRIME_COUNT 4
#endif


using namespace std;

Wheel30 wheel30;
MillerRabin millerRabin;

bool testPrime(NUMBER n)
{
	if (n < 75000) return wheel30.isPrime(n);
  return millerRabin.isPrime(n);
/*
	if (n % 2L == 0) return false;
	if (n % 3L == 0) return false;
	unsigned long sqrtN = floor(sqrt(n));
	unsigned long m = 6;
	for (unsigned long k = 1; m <= sqrtN; ++k, m=6*k)
	{
		if (n % (m+1L) == 0 || n % (m-1L) == 0) return false;
	}
	return true;
*/
}

int main()
{
	MPI_Init(NULL, NULL);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	
	NUMBER primes[PRIMES_ARRAY_SIZE];
	NUMBER range[2];
	range[0] = FIRST_RANGE_START;
	
	if (world_rank == 0) 
	{
#ifndef PRINT_PERIODIC_STATUS_ONLY					
#if INITIAL_PRIME_COUNT==4
		cout << "1:\t2" << endl;
		cout << "2:\t3" << endl;
		cout << "3:\t5" << endl;
		cout << "4:\t7" << endl;
#endif
#endif

		range[1] = FIRST_RANGE_END;
		NUMBER totalPrimeCount=INITIAL_PRIME_COUNT;
		int node;
		for (node=1; node<world_size && range[0] < range[1]; ++node)
		{
			MPI_Send(&range, 2, MPI_NUMBER, node, 0, MPI_COMM_WORLD);
			range[0] = range[1]+1;
			range[1] += RANGE_SIZE;
		}
		while (totalPrimeCount<NUM_PRIMES)
		{
			for (node=1; node<world_size && range[0] < range[1]; ++node)
			{
				int localCount = 0;

				MPI_Recv(&primes, PRIMES_ARRAY_SIZE, MPI_NUMBER, node, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				localCount = primes[0];
				for (int primeIndex=1; primeIndex<=localCount; ++primeIndex)
				{
					totalPrimeCount++;
#ifdef PRINT_PERIODIC_STATUS_ONLY					
					if (totalPrimeCount % 1000000 == 0)
					{
						time_t rawtime;
						struct tm * timeinfo;
						time(&rawtime);
						timeinfo = localtime ( &rawtime );
						cout << asctime(timeinfo) << '\t' 
						<< totalPrimeCount << ":\t" 
						<< primes[primeIndex] << endl;
					}
#else
					cout << totalPrimeCount << ":\t" << primes[primeIndex] << endl;
#endif
				}

				MPI_Send(&range, 2, MPI_NUMBER, node, 0, MPI_COMM_WORLD);
				range[0] = range[1]+1;
				range[1] += RANGE_SIZE;
			}
		}
		range[0]=0;
		range[1]=0;
		for (node=1; node<world_size; ++node)
		{
			MPI_Send(&range, 2, MPI_NUMBER, node, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		while (range[0] != 0)
		{
			int localCount = 0;

			MPI_Recv(&range, 2, MPI_NUMBER, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (range[0] != 0)
			{
				for (NUMBER n = range[0]; n < range[1]; n+=2)
				{
					if (testPrime(n))
					{
						primes[++localCount]=n;
					}
				}

				primes[0]=localCount;
				MPI_Send(&primes, localCount+1, MPI_NUMBER, 0, 0, MPI_COMM_WORLD);
			}
		}
	}

	MPI_Finalize();
	return 0;
}

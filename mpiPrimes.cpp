#include <iostream>
#include <cmath>
#include <mpi.h>
#include "Wheel30.h"
#include "MillerRabin.h"

#define NUM_PRIMES 2000000
#define RANGE_SIZE 1000
#define PRIMES_ARRAY_SIZE 333

using namespace std;

Wheel30 wheel30;
MillerRabin millerRabin;

bool testPrime(unsigned long n)
{
	if (n < 75000L) return wheel30.isPrime(n);
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
	
	
	unsigned long primes[PRIMES_ARRAY_SIZE];
	unsigned long range[2];
	range[0] = 8;
	
	if (world_rank == 0) 
	{
		cout << "1:\t2" << endl;
		cout << "2:\t3" << endl;
		cout << "3:\t5" << endl;
		cout << "4:\t7" << endl;
		range[1] = RANGE_SIZE;
		int totalPrimeCount=4;
		int node;
		for (node=1; node<world_size; ++node)
		{
			MPI_Send(&range, 2, MPI_UNSIGNED_LONG, node, 0, MPI_COMM_WORLD);
			range[0] = range[1]+1;
			range[1] += RANGE_SIZE;
		}
		while (totalPrimeCount<NUM_PRIMES)
		{
			for (node=1; node<world_size; ++node)
			{
				int localCount = 0;
				
				MPI_Recv(&primes, PRIMES_ARRAY_SIZE, MPI_UNSIGNED_LONG, node, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				localCount = primes[0];
				for (int primeIndex=1; primeIndex<=localCount; ++primeIndex)
				{
					totalPrimeCount++;
					cout << totalPrimeCount << ":\t" << primes[primeIndex] << endl;
				}
				
				MPI_Send(&range, 2, MPI_UNSIGNED_LONG, node, 0, MPI_COMM_WORLD);
				range[0] = range[1]+1;
				range[1] += RANGE_SIZE;				
			}
		}
		range[0]=0;
		range[1]=0;
		for (node=1; node<world_size; ++node)
		{
			MPI_Send(&range, 2, MPI_UNSIGNED_LONG, node, 0, MPI_COMM_WORLD);
		}
		
	}
	else 
	{
		while (range[0] != 0)
		{
			int localCount = 0;
			
			MPI_Recv(&range, 2, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (range[0] != 0)
			{
				for (unsigned long n = range[0]; n < range[1]; ++n)
				{
					if (testPrime(n))
					{
						primes[++localCount]=n;
					}
				}
				
				primes[0]=localCount;
				MPI_Send(&primes, localCount+1, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD);
			}
		}
	}
	MPI_Finalize();
	return 0;
}

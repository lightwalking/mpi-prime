#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;

#include "MillerRabin.h"
#include "Wheel30.h"


// Utility function to do modular exponentiation.
// It returns (a^b) % mod
// Overflows for cases where p > 32 bits 
NUMBER power32(NUMBER x, NUMBER y, NUMBER p)
{
	NUMBER res = 1;      // Initialize result
	x = x % p;  // Update x if it is more than or equal to p
	
	while (y > 0ULL)
	{
		// If y is odd, multiply x with result
		if (y & 1)
		{
			res = (res*x) % p;
		}
		
		// y must be even now
		y = y>>1; // y/2
		x = (x * x) % p;
	}
	return res;
}

NUMBER modmult(NUMBER a, NUMBER b, NUMBER mod)
{
	if (a == 0 || b < mod / a)
		return (a*b)%mod;
	NUMBER sum;
	sum = 0;
	while(b>0)
	{
		if(b&1)
			sum = (sum + a) % mod;
		a = (2*a) % mod;
		b>>=1;
	}
	return sum;
}

// Utility function to do modular exponentiation.
// It returns (a^b) % mod
// Protects against the overflow issue but is slower
// so it calls the faster power32 for function
// for mod <= 32 bits
NUMBER power(NUMBER a, NUMBER b, NUMBER mod)
{
	if (mod <= 0xFFFFFFFF)
		return power32(a, b, mod);
	
	NUMBER product,pseq;
	product=1;
	pseq=a%mod;
	while(b>0)
	{
		if(b&1)
			product=modmult(product,pseq,mod);
		pseq=modmult(pseq,pseq,mod);
		b>>=1;
	}
	return product;
}

// This function returns false if n is composite 
// and returns false if n is probably prime.
// d is an odd number such that  d*2<sup>r</sup> = n-1
// for some r >= 1
bool millerTest(NUMBER d, NUMBER n, NUMBER a)
{
	// Compute a^d % n
	NUMBER x = power(a, d, n);
	
	if (x == 1 || x == n-1)
		return true;
	
	// Keep squaring x while one of the following doesn't
	// happen
	// (i)   d does not reach n-1
	// (ii)  (x^2) % n is not 1
	// (iii) (x^2) % n is not n-1
	while (d != n-1)
	{
		x = (x * x) % n;
		d *= 2;
		
		if (x == 1)	return false;
		if (x == n-1)	return true;
	}
	
	// Return composite
	return false;
}

NUMBER a11[] = { 2 }; // if n < 2,047, it is enough to test a = 2;
NUMBER a21[] = { 2, 3 }; // if n < 1,373,653, it is enough to test a = 2 and 3;
NUMBER a24[] = { 31, 73 }; // if n < 9,080,191, it is enough to test a = 31 and 73;
NUMBER a25[] = { 2, 3, 5 }; // if n < 25,326,001, it is enough to test a = 2, 3, and 5;
NUMBER a32[] = { 2, 3, 5, 7 }; // if n < 3,215,031,751, it is enough to test a = 2, 3, 5, and 7;
NUMBER a33[] = { 2, 7, 61 }; // if n < 4,759,123,141, it is enough to test a = 2, 7, and 61;
NUMBER a41a[] = { 2, 13, 23, 1662803 }; // if n < 1,122,004,669,633, it is enough to test a = 2, 13, 23, and 1662803;
NUMBER a41b[] = { 2, 3, 5, 7, 11 }; // if n < 2,152,302,898,747, it is enough to test a = 2, 3, 5, 7, and 11;
NUMBER a42[] = { 2, 3, 5, 7, 11, 13 }; // if n < 3,474,749,660,383, it is enough to test a = 2, 3, 5, 7, 11, and 13;
NUMBER a49[] = { 2, 3, 5, 7, 11, 13, 17 }; // if n < 341,550,071,728,321, it is enough to test a = 2, 3, 5, 7, 11, 13, and 17;
NUMBER a62[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23 }; // if n < 3,825,123,056,546,413,051, it is enough to test a = 2, 3, 5, 7, 11, 13, 17, 19, and 23;
NUMBER a64[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 }; // if n < 18,446,744,073,709,551,616 = 264, it is enough to test a = 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, and 37;

Wheel30 wheel;

// It returns false if n is composite and returns true if n
// is prime. Wheel30 is used to validate prime vs probable prime.  
bool MillerRabin::isPrime(NUMBER n)
{
	// Corner cases
	if (n <= 1 || n == 4)  return false;
	if (n <= 5) return true;
	
	NUMBER* aSet = NULL;
	int aSize = 0;
//	if (n < 2047L) { aSet = a11; aSize = 1; }
//	else 
	if (n < 1373653L) { aSet = a21; aSize = 2; }
	else if (n < 9080191L) { aSet = a24; aSize = 2; }
//	else if (n < 25326001L) { aSet = a25; aSize = 3; }
//	else if (n < 3215031751L) { aSet = a32; aSize = 4; }
	else if (n < 4759123141L) { aSet = a33; aSize = 3; }
	else if (n < 1122004669633L) { aSet = a41a; aSize = 4; }
	else if (n < 2152302898747L) { aSet = a41b; aSize = 5; }
	else if (n < 3474749660383L) { aSet = a42; aSize = 6; }
	else if (n < 341550071728321L) { aSet = a49; aSize = 7; }
	else if (n < 3825123056546413051L) { aSet = a62; aSize = 9; }
	else { aSet = a64; aSize = 12; }
	
	// Find r such that n = 2^d * r + 1 for some r >= 1
	NUMBER d = n - 1;
	while (d % 2 == 0)
		d /= 2;
	
	int aIndex = 0;
	NUMBER a = aSet[aIndex++];
	while (a < n && aIndex < aSize)
	{
		if (millerTest(d, n, a) == false)
			return false;
		
		a = aSet[aIndex++];
	}
	if (!wheel.isPrime(n)) { cerr << "mismatch on n: " << n << endl; return false; }
	return true;
}

#include <cmath>
#include "Wheel30.h"

bool Wheel30::isPrime(NUMBER n)
{
	if (n == 2L || n == 3L || n == 5L || n == 7L || n == 11L
			|| n == 13L || n == 17L || n == 19L || n == 23L || n == 29L) return true;
	if (n <= 30L) return false;
	if (n % 2L == 0 || n % 3L == 0 || n % 5L == 0 || n % 7L == 0 || n % 11L == 0
			|| n % 13L == 0 || n % 17L == 0 || n % 19L == 0 || n % 23L == 0 || n % 29L == 0) return false;
	
	NUMBER sqrtN = floor(sqrt(n)) + 1;
	NUMBER m = 30;
	
  while (m <= sqrtN)
  {
    if (n % (m+1L) == 0
				|| n % (m+7L) == 0
				|| n % (m+11L) == 0
				|| n % (m+13L) == 0
				|| n % (m+17L) == 0
				|| n % (m+19L) == 0
				|| n % (m+23L) == 0
				|| n % (m+29L) == 0)
		{
			return false;
		}
		
		m+=30;
  }
  return true;
}

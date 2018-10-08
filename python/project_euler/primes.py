# # # Prime numbers generators

# Iterative generator
__primes = []

def __is_prime(number):
	for p in __primes:
		if number % p == 0:
			return False
	return True

def iterative():
	N = 2
	while True:
		if __is_prime(N):
			__primes.append(N)
			yield N
		N = N + 1

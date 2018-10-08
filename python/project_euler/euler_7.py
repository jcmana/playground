import primes

gen = primes.iterative()

for n in range(0, 10001):
	prime = gen.next()

print prime
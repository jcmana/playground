import sys
import primes

N = 600851475143

while N != 1:
	sys.stdout.write(str(N))
	gen = primes.generator()
	while True:
		div = gen.next()

		if N < div:
			break

		if N % div == 0:
			N = N / div
			sys.stdout.write(" div " + str(div) +" = " + str(N))
			sys.stdout.write("\n")
			break
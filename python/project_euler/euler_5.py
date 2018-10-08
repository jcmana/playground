N = 2520

while True:
	divisible = True
	for d in range(2, 20):
		if N % d:
			divisible = False
	if divisible:
		break
	else:
		N = N + 10

print N
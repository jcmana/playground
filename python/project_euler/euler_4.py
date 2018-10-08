A = 999
B = 999
palindromes = []

def is_palindrome(number):
	string = str(number)
	l = string[0:3]
	r = string[3:7]
	r = r[::-1]

	return l == r

while True:

	if is_palindrome(A * B):
		palindromes.append(A * B)

	if A < 100:
		A = 999
		B = B - 1
	else:
		A = A - 1

	if A < 100 and B < 100:
		break

print max(palindromes)

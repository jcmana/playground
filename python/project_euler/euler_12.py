# Project Euler - Problem 12 - Highly divisible triangular number

import time

def triangle_numbers():
    sum = 0
    num = 1
    while True:
        sum = sum + num
        yield sum
        num = num + 1

def factorize_counting(num):
    count = 1
    for n in range(2, num):
        if num % n == 0:
            count = count + 1
    return count

def factorize_full(num):
    factors = []
    rng = range(1, num)
    for n in rng:
        if num % n == 0:
            factors.append(n)
    factors.append(num)
    return factors

gen = triangle_numbers()

for t in gen:
    factors = factorize_full(t)
    print(str(t) + ": " + str(factors) + "(" + str(len(factors)) + ")")
    if len(factors) > 500:
        break
    input()

"""
for t in gen:
    print(t)
"""

"""
time_counting = []
time_full = []

for n in range(0, 10):
    #start = time.time()
    #factors = factorize_counting(24215412)
    #end = time.time()

    #time_counting.append(end - start)

    start = time.time()
    factors = factorize_full(24215412)
    end = time.time()

    time_full.append(end - start)

#print("Avarage time factorize_counting:")
#print(time_counting)
#print()
print("Avarage time factorize_full:")
print(time_full)
"""

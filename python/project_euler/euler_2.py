N = 4 * 10**6
a = 1
b = 2
s = 0

while True:
    k = a + b
    a = b
    b = k
    
    if k > N:
        break
    
    if k % 2 == 0:
        s += k

s = s + 2
print s
# basic tuple functionality
x = (1, 2, 3 * 4)
print(x)
try:
    x[0] = 4
except TypeError:
    print("TypeError")
print(x)
try:
    x.append(5)
except AttributeError:
    print("AttributeError")

print(x[1:])
print(x[:-1])
print(x[2:3])

print(x + (10, 100, 10000))

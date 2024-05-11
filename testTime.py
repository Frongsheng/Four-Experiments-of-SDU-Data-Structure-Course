import matplotlib.pyplot as plt

input_txt = 'time1.txt'
x = []
y = []
z = []

def new_func1(input_txt):
    f = open(input_txt)
    return f

f = new_func1(input_txt)
def new_func(x, line):
    x.append(int(line[0]))

for line in f:
    line = line.strip('\n')
    line = line.split(' ')

    new_func(x, line)
    y.append(float(line[1]))

f.close

input_txt = 'time2.txt'
f = open(input_txt)
for line in f:
    line = line.strip('\n')
    line = line.split(' ')

    z.append(float(line[1]))

f.close

plt.plot(x, y, marker='o', label='backtrack')
plt.plot(x, z, marker='*', label='branch_bound',linestyle="--")


plt.xticks(x[0:len(x):2], x[0:len(x):2], rotation=45)
plt.margins(0)
plt.xlabel("data")
plt.ylabel("cost(ms)")
plt.title("compare time")
plt.tick_params(axis="both")
plt.legend()
plt.show()
import subprocess
import random
import matplotlib.pyplot as plt
from statistics import mean

input_sizes = [1024, 128, 256, 341, 512, 1023, 2048]
colors = ["-r", "-g", "-b", "-y", "--r", "--g", "--b"]

random.seed()

def sample_sizes():
    for i in range(1):
        complete = subprocess.run(["./strassen", "2", str(input_sizes[i]), "yuh.txt"], capture_output=True)
        times = [float(x) for x in complete.stdout.decode('utf-8').rstrip("\n").split("\n")]
        n0s = [x for x in range(2, input_sizes[i] + 1)]
        plt.plot(n0s, times, colors[i])
        plots.append((input_sizes[i], n0s, times))

def min_n0():
    sizes = []
    n0s = []
    i = 256
    n0_ranks = {i: [] for i in range(32, 257)}
    while i <= 523:
        complete = subprocess.run(["./strassen", "2", str(i), "yuh.txt"], capture_output=True)
        time_list = complete.stdout.decode('utf-8').rstrip(",\n").split(",")
        times = [float(x) for x in time_list]
        n0_time_pairs = [(i + 32, times[i]) for i in range(len(times))]
        ranked = sorted(n0_time_pairs, key=take_second)
        for y in range(len(ranked)):
            elem = ranked[y]
            n0_ranks[elem[0]].append(y+1) #add rank of n0 to that n0's list of ranks
        i += random.randint(10, 20)
        print("completed dim " + str(i))
    xs =[]
    ys=[]
    for n0 in n0_ranks:
        xs.append(n0)
        ys.append(n0_ranks[n0])
        print(str(n0) + ", " + str(mean(n0_ranks[n0])))
    plt.plot(xs, ys, "-b")

def take_second(elem):
    return elem[1]

def read_res():
    ranks = []
    n0s = []
    f = open("res.txt", "r")
    for r in f:
        vals = r.split(",")
        n0s.append(int(vals[0]))
        ranks.append(float(vals[1]))
    plt.plot(n0s, ranks, "ro")
    
read_res()
plt.ylabel("average rank")
plt.xlabel("n0")
plt.title("Average ranks of reasonable n0")
plt.show()

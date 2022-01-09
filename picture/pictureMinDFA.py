from graphviz import Digraph

all_status = []
f = open('./DFAmin.txt', 'r')
lines = f.readlines()

stats = []
for line in lines:
    split = line.split(' ')
    all_status.append(split)

for i in range(1,len(all_status) - 2):
	if all_status[i][0] not in stats:
		stats.append(all_status[i][0])

print(stats)
dot = Digraph('minDFA')
for s in stats:
    dot.node(s, s)

for n in all_status[len(all_status) - 1]:
    dot.node(n,n, shape='doublecircle')

for i in range(1,len(all_status) - 2):
	for j in range(1,len(all_status[i]) - 1):
		if all_status[i][j] != '#':
			dot.edge(all_status[i][0],all_status[i][j],all_status[0][j - 1])
dot.view()

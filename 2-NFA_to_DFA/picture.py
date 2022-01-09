from os import stat
from graphviz import Digraph

all_status = []
f = open('./DFA.txt', 'r')
lines = f.readlines()

stats = []
for line in lines:
    split = line.split(' ')
    all_status.append(split)

for i in range(len(all_status) - 2):
	if all_status[i][0] not in stats:
		stats.append(all_status[i][0])

print(stats)
dot = Digraph('DFA')
for s in stats:
    dot.node(s, s)

for i in range(len(all_status[len(all_status) - 1]) - 1):
    dot.node(all_status[len(all_status) - 1][i], all_status[len(all_status) - 1][i], shape='doublecircle',)

for i in range(len(all_status) - 2):
    dot.edge(all_status[i][0], all_status[i][2], all_status[i][1])
dot.view()

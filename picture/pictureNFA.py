from graphviz import Digraph

all_status = []
f = open('./NFA.txt', 'r')
lines = f.readlines()

stats = []
for line in lines:
    split = line.split(' ')
    all_status.append(split)

for i in range(len(all_status) - 2):
	if all_status[i][0] not in stats:
		stats.append(all_status[i][0])

print(all_status)
dot = Digraph('DFA')
for s in stats:
    dot.node(s, s)


dot.node(all_status[len(all_status) - 1][1], all_status[len(all_status) - 1][1], shape='doublecircle')

for i in range(len(all_status) - 2):
    dot.edge(all_status[i][0], all_status[i][2], all_status[i][1])
dot.view()

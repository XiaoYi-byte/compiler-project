from graphviz import Digraph

all_status = []
f = open('./DFA.txt', 'r')
lines = f.readlines()

for line in lines:
    split = line[0:len(line) - 1].split('  ')
    all_status.append(split)

dot = Digraph('DFA')
for i in range(1, len(all_status) - 1):
    dot.node(all_status[i][0], all_status[i][0])

for i in range(len(all_status[len(all_status) - 1])):
    dot.node(all_status[len(all_status) - 1][i], all_status[len(all_status) - 1][i], shape='doublecircle')

for i in range(1, len(all_status) - 1):
    for j in range(1, len(all_status[i]) - 1):
        dot.edge(all_status[i][0], all_status[i][j], all_status[0][j])
dot.view()

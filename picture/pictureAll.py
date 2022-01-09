import os
from graphviz import Digraph


def pictureDFA():
	all_status = []

	f = open(os.path.abspath(os.path.dirname(os.path.dirname(__file__)))+'\\DFA.txt', 'r')
	lines = f.readlines()

	stats = []
	for line in lines:
		split = line.split(' ')
		all_status.append(split)

	for i in range(len(all_status) - 2):
		if all_status[i][0] not in stats:
			stats.append(all_status[i][0])

	print(stats)
	dot = Digraph(name='DFA',format='png')
	for s in stats:
		dot.node(s, s)

	for i in range(len(all_status[len(all_status) - 1]) - 1):
		dot.node(all_status[len(all_status) - 1][i], all_status[len(all_status) - 1][i], shape='doublecircle')

	for i in range(len(all_status) - 2):
		dot.edge(all_status[i][0], all_status[i][2], all_status[i][1])
	dot.render('./picture/DFA.gv', view=True)

def pictureNFA():
	all_status = []
	f = open(os.path.abspath(os.path.dirname(os.path.dirname(__file__)))+'\\NFA.txt', 'r')
	lines = f.readlines()

	stats = []
	for line in lines:
		split = line.split(' ')
		all_status.append(split)

	for i in range(len(all_status) - 2):
		if all_status[i][0] not in stats:
			stats.append(all_status[i][0])

	dot = Digraph(name='NFA',format='png')
	for s in stats:
		dot.node(s, s)


	dot.node(all_status[len(all_status) - 1][1], all_status[len(all_status) - 1][1], shape='doublecircle')

	for i in range(len(all_status) - 2):
		dot.edge(all_status[i][0], all_status[i][2], all_status[i][1])
	dot.render('./picture/NFA.gv', view=True)

def pictureMinDFA():
	all_status = []
	f = open(os.path.abspath(os.path.dirname(os.path.dirname(__file__)))+'\\DFAmin.txt', 'r')
	lines = f.readlines()

	stats = []
	for line in lines:
		split = line.split(' ')
		all_status.append(split)

	for i in range(1,len(all_status) - 2):
		if all_status[i][0] not in stats:
			stats.append(all_status[i][0])

	dot = Digraph(name='minDFA',format='png')
	for s in stats:
		dot.node(s, s)

	for n in all_status[len(all_status) - 1]:
		dot.node(n,n, shape='doublecircle')

	for i in range(1,len(all_status) - 2):
		for j in range(1,len(all_status[i]) - 1):
			if all_status[i][j] != '#':
				dot.edge(all_status[i][0],all_status[i][j],all_status[0][j - 1])
	dot.render('./picture/minDFA.gv', view=True)

def draw():
	pictureNFA()
	pictureDFA()
	pictureMinDFA()

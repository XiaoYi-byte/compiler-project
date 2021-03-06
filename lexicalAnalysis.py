import time
from tkinter import *
from tkinter import ttk, messagebox
from tkinter.font import Font
from graphviz import Digraph
from PIL import Image, ImageTk as itk
import subprocess
import os

raw1 = None
raw2 = None
raw3 = None
NFA = None
DFA = None
mDFA = None
text = ""

def pictureDFA():
    all_status = []

    f = open('./file/DFA.txt', 'r')
    lines = f.readlines()

    stats = []
    for line in lines:
        split = line.split(' ')
        all_status.append(split)

    for i in range(len(all_status) - 2):
        if all_status[i][0] not in stats:
            stats.append(all_status[i][0])
    dot = Digraph(name='DFA', format='png')
    for s in stats:
        dot.node(s, s)

    for i in range(len(all_status[len(all_status) - 1]) - 1):
        dot.node(all_status[len(all_status) - 1][i], all_status[len(all_status) - 1][i], shape='doublecircle')

    for i in range(len(all_status) - 2):
        dot.edge(all_status[i][0], all_status[i][2], all_status[i][1])
    dot.render('./picture/DFA.gv')


def pictureNFA():
    all_status = []
    f = open('./file/NFA.txt', 'r')
    lines = f.readlines()

    stats = []
    for line in lines:
        split = line.split(' ')
        all_status.append(split)

    for i in range(len(all_status) - 2):
        if all_status[i][0] not in stats:
            stats.append(all_status[i][0])

    dot = Digraph(name='NFA', format='png')
    for s in stats:
        dot.node(s, s)

    dot.node(all_status[len(all_status) - 1][1], all_status[len(all_status) - 1][1], shape='doublecircle')

    for i in range(len(all_status) - 2):
        dot.edge(all_status[i][0], all_status[i][2], all_status[i][1])
    dot.render('./picture/NFA.gv')


def pictureMinDFA():
    all_status = []
    f = open('./file/DFAmin.txt', 'r')
    lines = f.readlines()

    stats = []
    for line in lines:
        split = line.split(' ')
        all_status.append(split)

    for i in range(1, len(all_status) - 2):
        if all_status[i][0] not in stats:
            stats.append(all_status[i][0])

    dot = Digraph(name='minDFA', format='png')
    for s in stats:
        dot.node(s, s)

    for n in all_status[len(all_status) - 1]:
        dot.node(n, n, shape='doublecircle')

    for i in range(1, len(all_status) - 2):
        for j in range(1, len(all_status[i]) - 1):
            if all_status[i][j] != '#':
                dot.edge(all_status[i][0], all_status[i][j], all_status[0][j - 1])
    dot.render('./picture/minDFA.gv')


def draw():
    pictureNFA()
    pictureDFA()
    pictureMinDFA()
    global raw1
    global raw2
    global raw3
    global NFA
    global DFA
    global mDFA
    raw1 = Image.open("./picture/NFA.gv.png")
    raw2 = Image.open("./picture/DFA.gv.png")
    raw3 = Image.open("./picture/minDFA.gv.png")
    NFA = itk.PhotoImage(raw1.resize((200, 500)))
    DFA = itk.PhotoImage(raw2.resize((200, 500)))
    mDFA = itk.PhotoImage(raw3.resize((200, 500)))
    pic1 = Canvas(frmR, bg='#87CEEB', width=200, height=500)
    pic1.create_image(2, 2, anchor=NW, image=NFA)
    pic1.grid(row=2, column=0, padx=10)

    pic2 = Canvas(frmR, bg='#87CEEB', width=200, height=500)
    pic2.create_image(2, 2, anchor=NW, image=DFA)
    pic2.grid(row=2, column=1, padx=10)

    pic3 = Canvas(frmR, bg='#87CEEB', width=200, height=500)
    pic3.create_image(2, 2, anchor=NW, image=mDFA)
    pic3.grid(row=2, column=2, padx=10)


def save():
    # save as txt file
    sentence = grammar.get(0.0, 'end')
    sentences = sentence.split('\n')
    output1 = ""
    output2 = ""
    for line in sentences:
        if line == "":
            continue
        if line[0] != 'V':
            output1 = output1 + line + ' #' + '\n'
        else:
            line = ''.join(line.split())
            line = line.strip('V->')
            output2 = output2 + line + '\n'

    o1 = open("./file/DK.txt", 'w', encoding='utf-8')
    o1.write(output1)
    o1.close()

    o2 = open("./file/input.txt", 'w', encoding='utf-8')
    o2.write(output2)
    o2.close()
    messagebox.showinfo(title=None, message="Success Generated!")


def test():
    code = testCode.get()+'\n'
    o = open("./file/in.txt", 'w', encoding='utf-8')
    o.write(code)
    o.close()
    time.sleep(1)
    f = open("./file/out.txt", 'r', encoding='utf-8')
    global text
    text = f.read()
    print(text)
    res.insert('end', text)
    res.grid(row=5, column=0, columnspan=2)
    f.close()


if __name__ == '__main__':
    window = Tk()
    window.title('lexicalAnalysis')
    # window.geometry('960x600')
    window.resizable(0, 0)
    window.configure(bg='#D9FCFF')

    frmL = Frame(window, bg='#E6E6FA')
    frmL.pack(side=LEFT, padx=10)
    frmR = Frame(window, bg='#D9FCFF')
    frmR.pack(side=RIGHT, padx=10)

    # ??????????????????2???????????????1??????????????????2?????????
    Label(frmL, bg='#E6E6FA', font=('????????????', 20, 'bold'), text='????????????????????????') \
        .grid(row=0, column=0)
    grammar = Text(frmL, width=40, height=20)
    grammar.grid(row=1, column=0, padx=10, pady=5)
    lexiBtn = Button(frmL, text='?????????????????????', font='??????', bg='#BDD6E5', borderwidth=2,
                     relief='groove', width=20, height=3, command=save) \
        .grid(row=2, column=0, columnspan=2, padx=5, pady=5)

    Label(frmL, bg='#E6E6FA', font=('????????????', 20, 'bold'), text='????????????') \
        .grid(row=3, column=0)
    message = StringVar()
    message.set("Result:")
    testCode = Entry(frmL,width=40)
    testCode.grid(row=4, column=0, columnspan=2, padx=5, pady=5)
    res = Listbox(frmL, bg="#FFF0F5", listvariable=message,width=50)
    res.grid(row=5, column=0, columnspan=2)
    testBtn = Button(frmL, text='????????????', font='??????', bg='#BDD6E5', borderwidth=2,
                     relief='groove', width=20, height=3, command=test).grid(row=6, column=0, columnspan=2, padx=5,
                                                                             pady=5)
    # ?????????????????????
    # myFont = Font(family="Times New Roman", size=15)
    # grammar.configure(font=myFont)
    # testCode.configure(font=myFont)

    # ????????????????????????????????????
    drawBtn = Button(frmR, text='Visible', font='??????', bg='#FFF0F5', borderwidth=2,
                     relief='groove', width=8, height=2, command=draw)
    drawBtn.grid(row=0, column=0, columnspan=3, padx=5, pady=5)
    Label(frmR, bg='#D9FCFF', font=('????????????', 20, 'bold'), text='RE->NFA->DFA->minDFA') \
        .grid(row=1, column=0, columnspan=3)

    cv1 = Canvas(frmR, bg='white', width=200, height=500)
    cv1.grid(row=2, column=0, padx=10)
    Label(frmR, bg='#D9FCFF', font=('??????', 20, 'bold'), text='NFA').grid(row=3, column=0, pady=5)

    cv2 = Canvas(frmR, bg='white', width=200, height=500)
    cv2.grid(row=2, column=1, padx=10)
    Label(frmR, bg='#D9FCFF', font=('??????', 20, 'bold'), text='DFA').grid(row=3, column=1, pady=5)

    cv3 = Canvas(frmR, bg='white', width=200, height=500)
    cv3.grid(row=2, column=2, padx=10)
    Label(frmR, bg='#D9FCFF', font=('??????', 20, 'bold'), text='minDFA').grid(row=3, column=2, pady=5)

    window.mainloop()

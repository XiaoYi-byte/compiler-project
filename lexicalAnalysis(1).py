import time
from tkinter import *
from tkinter import ttk
from tkinter.font import Font
from PIL import Image, ImageTk as itk
import os


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

    o1 = open("DK.txt", 'w', encoding='utf-8')
    o1.write(output1)
    o1.close()

    o2 = open("input.txt", 'w', encoding='utf-8')
    o2.write(output2)
    o2.close()


def test():
    code = testCode.get()+'\n'
    o = open("in.txt", 'w', encoding='utf-8')
    o.write(code)
    o.close()
    time.sleep(5)
    f = open("out.txt", 'r', encoding='utf-8')
    text = f.read()
    print(text)
    res.insert('end', text)
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

    # 左侧框架放置2个输入框，1个输出框以及2个按钮
    Label(frmL, bg='#E6E6FA', font=('微软雅黑', 20, 'bold'), text='词法分析程序生成') \
        .grid(row=0, column=0)
    grammar = Text(frmL, width=40, height=20)
    grammar.grid(row=1, column=0, padx=10, pady=5)
    lexiBtn = Button(frmL, text='生成词法分析器', font='楷体', bg='#BDD6E5', borderwidth=2,
                     relief='groove', width=20, height=3, command=save) \
        .grid(row=2, column=0, columnspan=2, padx=5, pady=5)

    Label(frmL, bg='#E6E6FA', font=('微软雅黑', 20, 'bold'), text='测试程序') \
        .grid(row=3, column=0)
    testCode = Entry(frmL, textvariable='please enter code here')
    testCode.grid(row=4, column=0, columnspan=2, padx=5, pady=5)
    res = Listbox(frmL)
    res.grid(row=5, column=0, columnspan=2, padx=10, pady=5)
    testBtn = Button(frmL, text='测试代码', font='楷体', bg='#BDD6E5', borderwidth=2,
                     relief='groove', width=20, height=3, command=test).grid(row=6, column=0, columnspan=2, padx=5, pady=5)
    # 设置文本框样式
    # myFont = Font(family="Times New Roman", size=15)
    # grammar.configure(font=myFont)
    # testCode.configure(font=myFont)
    res.config(state=DISABLED, cursor='arrow')

    # 右侧画布显示各过程的结果
    Label(frmR, bg='#D9FCFF', font=('微软雅黑', 20, 'bold'), text='RE->NFA->DFA->minDFA') \
        .grid(row=0, column=0, columnspan=3)

    # while True:
    #     if os.path.exists("NFA.gv.png") and os.path.exists("DFA.gv.png") and os.path.exists("minDFA.gv.png"):
    #         break

    raw1 = Image.open("img.png")
    raw2 = Image.open("img.png")
    raw3 = Image.open("img.png")

    NFA = itk.PhotoImage(raw1.resize((200, 500)))
    DFA = itk.PhotoImage(raw2.resize((200, 500)))
    mDFA = itk.PhotoImage(raw3.resize((200, 500)))

    cv1 = Canvas(frmR, bg='white', width=200, height=500)
    cv1.create_image(2, 2, anchor=NW, image=NFA)
    cv1.grid(row=1, column=0, padx=10)
    Label(frmR, bg='#D9FCFF', font=('楷体', 20, 'bold'), text='NFA').grid(row=2, column=0, pady=5)

    cv2 = Canvas(frmR, bg='white', width=200, height=500)
    cv2.create_image(2, 2, anchor=NW, image=DFA)
    cv2.grid(row=1, column=1, padx=10)
    Label(frmR, bg='#D9FCFF', font=('楷体', 20, 'bold'), text='DFA').grid(row=2, column=1, pady=5)

    cv3 = Canvas(frmR, bg='white', width=200, height=500)
    cv3.create_image(2, 2, anchor=NW, image=mDFA)
    cv3.grid(row=1, column=2, padx=10)
    Label(frmR, bg='#D9FCFF', font=('楷体', 20, 'bold'), text='minDFA').grid(row=2, column=2, pady=5)

    window.mainloop()

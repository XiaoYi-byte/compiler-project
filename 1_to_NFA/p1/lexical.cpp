#include"lexical.h"

int StateNum=0;

void putin(string& sentence)
{
    cout << "������������ʽ��";
    cin >> sentence;
    while (!islegal(sentence))
    {
        cout << "����ı��ʽ�Ƿ������������룺";
        cin >> sentence;
    }

}

bool islegal(string sentence)
{
    int length = sentence.size();
    //��֤ÿ���ַ��ǺϷ��ģ�ͬʱ����Ҫƥ��  
    if ((!legal_char(sentence)) || (!legal_match(sentence)))
    {
        return false;
    }
    return true;
}

bool is_letter(char c)
{
    if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) return true;
    else  return false;
}

bool is_operator(char c)
{
    if ((c == '(') || (c == ')') || (c == '|') || (c == '*')) return true;
    else return false;
}


bool legal_char(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        char c = s[i];
        if ( !is_letter(c)&& !is_operator(c) )  return false;
    }
    return true;
}

bool legal_match(string s)
{
    stack<char> STACK;
    for (int i = 0; i < s.size(); i++)
    {
        char c = s[i];
        if (c == '(') STACK.push(c);
        if (c == ')')
        {
            if (!STACK.empty()&& STACK.top() == '(') STACK.pop();
            else
            {
                cout << "��ƥ���������(" << endl;
                return false;
            }
        }
    }
    if (!STACK.empty())
    {
        cout << "��ƥ���������)" << endl;
        return false;
    }

    return true;

}



string addplus(string s)
{
    string final;
    for (int i = 0, j = 1; j < s.size(); i++, j++)  //���������ָ��
    {
        if (is_letter(s[i]) && is_letter(s[j])) final = final + s[i] + '+';  //������ĸ�����м��+
        else if (s[i] == '*' && is_letter(s[j]))  final = final + s[i] + '+';
        else if (s[i] == ')' && is_letter(s[j]))  final = final + s[i] + '+';
        else if (s[i]==')' && s[j] == '(')  final = final + s[i] + '+';   //��ĸ��(������м��+
        else if (s[i] == '*' && s[j] == '(')  final = final + s[i] + '+';
        else if (is_letter(s[i]) && s[j] == '(')  final = final + s[i] + '+';

        else  final = final + s[i];
    }
    final = final + s[s.size() - 1];  //���һ��������
    
    return final;
}

int isp(char c)  //ջ�����ȼ�
{
    switch(c)
    {
        case '#': return 0;
        case '(': return 1;
        case '*':return 7;
        case '|':return 5;
        case '+':return 3;
        case ')':return 8;  //ջ�ڲ�����������ţ�
        default:
        cout<<"error"<<endl;  //�����ϲ�����������Ƿ��ַ�
        return false;
    }
}

int osp(char c)
{
    switch(c)
    {
        case '#': return 0;
        case '(': return 8;
        case '*':return 6; //��ͬ������Բ���ջ
        case '|':return 4;
        case '+':return 2;
        case ')':return 1;  //ջ�ڲ�����������ţ�
        default:
        cout<<"error"<<endl;  //�����ϲ�����������Ƿ��ַ�
        return false;
    }
}

string convert(string s)
{
    string final;
    char temp;
    stack<char> sign;
    sign.push('#');
    int length=s.size();
    for(int i=0;i<length;i++)
    {
        if(is_letter(s[i])) final+=s[i];  //����Ƿ������ֱ�Ӽ��������ַ���
        else
        {
            while(osp(s[i])<isp(sign.top()))  //������������һֱpop
            {
                temp=sign.top();
                sign.pop();
                final+=temp;
            }
            if(s[i]!=')') sign.push(s[i]);
            else sign.pop();  //����������ŵĻ���ջ��һ���������ţ�Ҳ������
        }
    }
    while(sign.top()!='#')   //�ѷ���ջ������ܵ��������pop�����ӵ�final����
    {
        temp=sign.top();
        sign.pop();
        final+=temp;
    }
    cout << final << endl;
    return final;
}


Cell toNFA(string s)
{
    int length=s.size();
    stack<Cell> cstack;
    Cell temp,cell1,cell2; 
    for (int i=0;i<length;i++)
    {
        if(is_letter(s[i])) temp=op_letter(s[i]); 
        else if(s[i]=='+')
        {
            cell1=cstack.top();
            cstack.pop();
            cell2=cstack.top();
            cstack.pop();
            temp=op_plus(cell2,cell1);
        }
        else if(s[i]=='|') 
        {
            cell1=cstack.top();
            cstack.pop();
            cell2=cstack.top();
            cstack.pop();
            temp=op_or(cell2,cell1);
        }
        else if(s[i]=='*')
        {
            cell1=cstack.top();
            cstack.pop();
            temp=op_close(cell1);
        }
        else 
        {
            cout<<"error"<<endl;
      //      return false;
        }
        cstack.push(temp);
    }
    temp=cstack.top();
    cstack.pop();
    return temp;
}

Node AddNode()
{
    Node newnode;
    newnode.name=StateNum+65;  //ת���ɴ�д��ĸ
    StateNum++; 
    return newnode;
}

Cell op_letter(char c)
{
    Cell cell;
    cell.edgenumber=0;
    Edge edge;

    //�������ɽڵ�
    Node st=AddNode();
    Node en=AddNode();

    //��
    edge.start=st;
    edge.end=en;
    edge.symbol=c;

    //ģ��
    cell.edgeset[cell.edgenumber++]=edge;
    cell.startnode=cell.edgeset[0].start;
    cell.endnode=cell.edgeset[0].end;

    cout << "���ַ�ģ����׽���ǣ�" << cell.startnode.name << endl;
    cout << "���ַ�ģ���β����ǣ�" << cell.endnode.name << endl;

    return cell;
}

Cell op_plus(Cell c1,Cell c2)
{
    Cell cell;
    cell.edgenumber=0;
    Edge edge;

    //�ߵĴ���
    copyedge(cell,c1);
    copyedge(cell,c2);

    edge.start=c1.endnode;   //��������ֱ�����c1.endnode�ȼ���
    edge.end=c2.startnode;
    edge.symbol='#';

    //cell�Ĵ���
    cell.startnode=c1.startnode;
    cell.endnode=c2.endnode;

    cell.edgeset[cell.edgenumber++]=edge;

    cout << "�ӷ�������׽���ǣ�" << cell.startnode.name << endl;
    cout << "�ӷ������β����ǣ�" << cell.endnode.name << endl;

    return cell;

}

Cell op_or(Cell c1,Cell c2)
{
    Cell cell;
    cell.edgenumber=0;
    Edge edge1,edge2,edge3,edge4;

    //����������β�ڵ�
    Node st=AddNode();
    Node en=AddNode();

    //�ߵĴ���
    copyedge(cell,c1);
    copyedge(cell,c2);

    edge1.start=st;
    edge1.end=c1.edgeset[0].start;
    edge1.symbol='#';

    edge2.start=st;
    edge2.end=c2.edgeset[0].start;
    edge2.symbol='#';
    
    edge3.start=c1.edgeset[c1.edgenumber-1].end;
    edge3.end=en;
    edge3.symbol='#';

    edge4.start=c2.edgeset[c2.edgenumber-1].end;
    edge4.end=en;
    edge4.symbol='#';

    //cell�Ĵ���
    cell.startnode=st;
    cell.endnode=en;
    
    cell.edgeset[cell.edgenumber++]=edge1;
    cell.edgeset[cell.edgenumber++]=edge2;
    cell.edgeset[cell.edgenumber++]=edge3;
    cell.edgeset[cell.edgenumber++]=edge4;

    cout << "��������׽���ǣ�" << cell.startnode.name << endl;
    cout << "�������β����ǣ�" << cell.endnode.name << endl;

    return cell;

}

Cell op_close(Cell c)
{
    Cell cell;
    cell.edgenumber=0;
    Edge edge1,edge2,edge3,edge4;

    //����������β�ڵ�
    Node st=AddNode();
    Node en=AddNode();

    //�ߵĴ���
    edge1.start=st;
    edge1.end=c.startnode;
    edge1.symbol='#';

    edge2.start=st;
    edge2.end=en;
    edge2.symbol='#';
    
    edge3.start=c.endnode;
    edge3.end=en;
    edge3.symbol='#';

    edge4.start=c.endnode;
    edge4.end=c.startnode;
    edge4.symbol='#';

    copyedge(cell,c);

    //cell�Ĵ���
    cell.startnode=st;
    cell.endnode=en;
    
    cell.edgeset[cell.edgenumber++]=edge1;
    cell.edgeset[cell.edgenumber++]=edge2;
    cell.edgeset[cell.edgenumber++]=edge3;
    cell.edgeset[cell.edgenumber++]=edge4;

    cout << "�հ�������׽���ǣ�" << cell.startnode.name << endl;
    cout<< "�հ������β����ǣ�" << cell.endnode.name << endl;

    return cell;
}


void copyedge(Cell& final,Cell origin)
{
    int fnumber=final.edgenumber;
    int onumber=origin.edgenumber;

    for(int i=0;i<onumber;i++)
    {
        final.edgeset[fnumber+i]=origin.edgeset[i];
    }
    final.edgenumber=fnumber+onumber;
}


void show(Cell c)
{
    cout << "NFA �ı�����" << c.edgenumber << endl;
    cout << "NFA ����ʼ״̬��" << c.startnode.name << endl;
    cout << "NFA �Ľ���״̬��" << c.endnode.name << endl;
    for (int i = 0; i < c.edgenumber; i++)
    {
        cout << "��" << i + 1 << "���ߵ���ʼ״̬��" << c.edgeset[i].start.name
            << "  ����״̬��" << c.edgeset[i].end.name
            << "  ת������" << c.edgeset[i].symbol << endl;
    }
}
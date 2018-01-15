// 2017.12.23

#include <iostream>
#include <cstdio>
#include <conio.h>
#include <cstdlib>
using namespace std;

#define MaxSize 100

// 定义数据栈，储存转化为后缀表达式的字符串算式
typedef struct Data_SNode {
    int Top;
    int Ptr;
    char Elem[MaxSize][MaxSize];
} *D_Stack;

// 定义操作符栈，用于执行中缀转后缀表达式
typedef struct Operation_SNode {
    int Top;
    char Elem[MaxSize];
} *Oper_Stack;

// 定义算式栈，用于执行后缀表达式的计算
typedef struct Counter_SNode {
    int Top;
    double Elem[MaxSize];
} *C_Stack;

D_Stack initData();     // 初始化数据栈
Oper_Stack initOper();  // 初始化操作符栈
C_Stack initCounter();  // 初始化算式栈
void getInput(D_Stack Data, Oper_Stack Oper);               // 接受输入算式并储存在数据栈中
void PreToSuf(char ch, Oper_Stack Oper, D_Stack Data);      // 操作符中缀转后缀表达式算法，即逆波兰核心算法
void Count(D_Stack Data, C_Stack Counter);                  // 计算后缀表达式


int main()
{
    D_Stack Data = initData();
    Oper_Stack Oper = initOper();
    C_Stack Counter = initCounter();
    getInput(Data, Oper);
    Count(Data, Counter);

    return 0;
}

D_Stack initData()
{
    int i, j;
    D_Stack Data;

    Data = (struct Data_SNode *)malloc(sizeof(struct Data_SNode));
    for (i = 0; i < MaxSize; i++)
    {
        for (j = 0; j < MaxSize; j++)
        {
            Data->Elem[i][j] = '\0';
        }
    }
    Data->Top = -1;
    Data->Ptr = -1;

    return Data;
}

Oper_Stack initOper()
{
    int i;
    Oper_Stack Oper;
    Oper = (struct Operation_SNode *)malloc(sizeof(struct Operation_SNode));
    for (i = 0; i < MaxSize; i++)
    {
        Oper->Elem[i] = '\0';
    }
    Oper->Top = -1;

    return Oper;
}

C_Stack initCounter()
{
    int i;
    C_Stack Counter;
    Counter = (struct Counter_SNode *)malloc(sizeof(struct Counter_SNode));
    for (i = 0; i < MaxSize; i++)
    {
        Counter->Elem[i] = 65535;
    }
    Counter->Top = -1;

    return Counter;
}

void getInput(D_Stack Data, Oper_Stack Oper)
{
    int flag;           // 标记之前的输入是什么
    char ch;            // 接收输入字符

    flag = 0;           // flag：0是初始状态，1代表上一次输入数字，2代表上一次输入符号
    ch = getche();
    while (ch != 61)    // 在输入‘=’之前
    {
        if (ch >= 48 && ch <= 57 || ch == 46)   // 如果输入字符是数字或小数点，直接存入
        {
            if( flag == 0)      // 初始字符
            {
                Data->Elem[++Data->Top][++Data->Ptr] = ch;
                flag = 1;
            }
            else if (flag == 1) // 前一个输入为数字，继续向后排
            {
                Data->Elem[Data->Top][++Data->Ptr] = ch;
            }
            else  // flag == 2  // 前一个输入为操作符，换一行排
            {
                Data->Ptr = -1;
                Data->Elem[++Data->Top][++Data->Ptr] = ch;
                flag = 1;
            }
        }
        else if (ch == 42 || ch == 43 || ch == 45 || ch == 47 || ch == 40 || ch == 41)
        {                                           // 如果接受字符是符号，执行中缀转后缀算法
            PreToSuf(ch, Oper, Data);
            flag = 2;
        }
        ch = _getche();
    }
    while (Oper->Top >= 0)      // 输入‘=’后，将操作符栈中的字符全部取出来
    {
        Data->Ptr = -1;
        Data->Elem[++Data->Top][++Data->Ptr] = Oper->Elem[Oper->Top--];
    }
}

void PreToSuf(char ch, Oper_Stack Oper, D_Stack Data)
{
    if (Oper->Top == -1)                // 如果栈空
    {
        Oper->Elem[++Oper->Top] = ch;
    }
    else
    {
        if (ch == 42 || ch == 47)       // 如果接受字符为*或/，则将栈顶的*或/全部排出再存入
        {
            while (Oper->Elem[Oper->Top] == 42 || Oper->Elem[Oper->Top] == 47)
            {
                Data->Ptr = -1;
                Data->Elem[++Data->Top][++Data->Ptr] = Oper->Elem[Oper->Top--];
            }
            Oper->Elem[++Oper->Top] = ch;
        }
        else if (ch == 43 || ch == 45)  // 如果接受字符为+或-
        {
            if (Oper->Elem[Oper->Top] == 40)    // 栈顶是（，则直接存入
                Oper->Elem[++Oper->Top] = ch;
            else                                // 栈顶为其他，则取出至栈顶为（或栈空，再存入
            {
                while (Oper->Top >= 0 && Oper->Elem[Oper->Top] != 40)
                {
                    Data->Ptr = -1;
                    Data->Elem[++Data->Top][++Data->Ptr] = Oper->Elem[Oper->Top--];
                }
                Oper->Elem[++Oper->Top] = ch;
            }
        }
        else if (ch == 40)              // 接受字符为（，则直接存入
        {
            Oper->Elem[++Oper->Top] = ch;
        }
        else if (ch == 41)              // 接受字符为）,则将栈内（前字符全部排出,并舍弃（
        {
            while (Oper->Elem[Oper->Top] != 40)
            {
                Data->Ptr = -1;
                Data->Elem[++Data->Top][++Data->Ptr] = Oper->Elem[Oper->Top--];
            }
            Oper->Top--;
        }
    }
}

void Count(D_Stack Data, C_Stack Counter)
{
    int i;
    double Pre, Later, RST;     // Pre为前数，Later为后数，RST为结果值
    char Operation;             // 符号

    for (i = 0; i <= Data->Top; i++)
    {
        if (Data->Elem[i][0] >= 48)
        {
            Counter->Elem[++Counter->Top] = atof(Data->Elem[i]);  // atof()将字符类型转化为double类型
        }
        else
        {
            Operation = Data->Elem[i][0];
            Later = Counter->Elem[Counter->Top--];
            Pre = Counter->Elem[Counter->Top--];
            switch (Operation)
            {
            case 43: RST = Pre + Later; break;      // 加
            case 45: RST = Pre - Later; break;      // 减
            case 42: RST = Pre * Later; break;      // 乘
            case 47: RST = Pre / Later; break;      // 除
            }
            Counter->Elem[++Counter->Top] = RST;
        }
    }

    cout << Counter->Elem[Counter->Top];
}

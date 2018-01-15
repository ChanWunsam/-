// 2017.12.23

#include <iostream>
#include <cstdio>
#include <conio.h>
#include <cstdlib>
using namespace std;

#define MaxSize 100

// ��������ջ������ת��Ϊ��׺���ʽ���ַ�����ʽ
typedef struct Data_SNode {
    int Top;
    int Ptr;
    char Elem[MaxSize][MaxSize];
} *D_Stack;

// ���������ջ������ִ����׺ת��׺���ʽ
typedef struct Operation_SNode {
    int Top;
    char Elem[MaxSize];
} *Oper_Stack;

// ������ʽջ������ִ�к�׺���ʽ�ļ���
typedef struct Counter_SNode {
    int Top;
    double Elem[MaxSize];
} *C_Stack;

D_Stack initData();     // ��ʼ������ջ
Oper_Stack initOper();  // ��ʼ��������ջ
C_Stack initCounter();  // ��ʼ����ʽջ
void getInput(D_Stack Data, Oper_Stack Oper);               // ����������ʽ������������ջ��
void PreToSuf(char ch, Oper_Stack Oper, D_Stack Data);      // ��������׺ת��׺���ʽ�㷨�����沨�������㷨
void Count(D_Stack Data, C_Stack Counter);                  // �����׺���ʽ


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
    int flag;           // ���֮ǰ��������ʲô
    char ch;            // ���������ַ�

    flag = 0;           // flag��0�ǳ�ʼ״̬��1������һ���������֣�2������һ���������
    ch = getche();
    while (ch != 61)    // �����롮=��֮ǰ
    {
        if (ch >= 48 && ch <= 57 || ch == 46)   // ��������ַ������ֻ�С���㣬ֱ�Ӵ���
        {
            if( flag == 0)      // ��ʼ�ַ�
            {
                Data->Elem[++Data->Top][++Data->Ptr] = ch;
                flag = 1;
            }
            else if (flag == 1) // ǰһ������Ϊ���֣����������
            {
                Data->Elem[Data->Top][++Data->Ptr] = ch;
            }
            else  // flag == 2  // ǰһ������Ϊ����������һ����
            {
                Data->Ptr = -1;
                Data->Elem[++Data->Top][++Data->Ptr] = ch;
                flag = 1;
            }
        }
        else if (ch == 42 || ch == 43 || ch == 45 || ch == 47 || ch == 40 || ch == 41)
        {                                           // ��������ַ��Ƿ��ţ�ִ����׺ת��׺�㷨
            PreToSuf(ch, Oper, Data);
            flag = 2;
        }
        ch = _getche();
    }
    while (Oper->Top >= 0)      // ���롮=���󣬽�������ջ�е��ַ�ȫ��ȡ����
    {
        Data->Ptr = -1;
        Data->Elem[++Data->Top][++Data->Ptr] = Oper->Elem[Oper->Top--];
    }
}

void PreToSuf(char ch, Oper_Stack Oper, D_Stack Data)
{
    if (Oper->Top == -1)                // ���ջ��
    {
        Oper->Elem[++Oper->Top] = ch;
    }
    else
    {
        if (ch == 42 || ch == 47)       // ��������ַ�Ϊ*��/����ջ����*��/ȫ���ų��ٴ���
        {
            while (Oper->Elem[Oper->Top] == 42 || Oper->Elem[Oper->Top] == 47)
            {
                Data->Ptr = -1;
                Data->Elem[++Data->Top][++Data->Ptr] = Oper->Elem[Oper->Top--];
            }
            Oper->Elem[++Oper->Top] = ch;
        }
        else if (ch == 43 || ch == 45)  // ��������ַ�Ϊ+��-
        {
            if (Oper->Elem[Oper->Top] == 40)    // ջ���ǣ�����ֱ�Ӵ���
                Oper->Elem[++Oper->Top] = ch;
            else                                // ջ��Ϊ��������ȡ����ջ��Ϊ����ջ�գ��ٴ���
            {
                while (Oper->Top >= 0 && Oper->Elem[Oper->Top] != 40)
                {
                    Data->Ptr = -1;
                    Data->Elem[++Data->Top][++Data->Ptr] = Oper->Elem[Oper->Top--];
                }
                Oper->Elem[++Oper->Top] = ch;
            }
        }
        else if (ch == 40)              // �����ַ�Ϊ������ֱ�Ӵ���
        {
            Oper->Elem[++Oper->Top] = ch;
        }
        else if (ch == 41)              // �����ַ�Ϊ��,��ջ�ڣ�ǰ�ַ�ȫ���ų�,��������
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
    double Pre, Later, RST;     // PreΪǰ����LaterΪ������RSTΪ���ֵ
    char Operation;             // ����

    for (i = 0; i <= Data->Top; i++)
    {
        if (Data->Elem[i][0] >= 48)
        {
            Counter->Elem[++Counter->Top] = atof(Data->Elem[i]);  // atof()���ַ�����ת��Ϊdouble����
        }
        else
        {
            Operation = Data->Elem[i][0];
            Later = Counter->Elem[Counter->Top--];
            Pre = Counter->Elem[Counter->Top--];
            switch (Operation)
            {
            case 43: RST = Pre + Later; break;      // ��
            case 45: RST = Pre - Later; break;      // ��
            case 42: RST = Pre * Later; break;      // ��
            case 47: RST = Pre / Later; break;      // ��
            }
            Counter->Elem[++Counter->Top] = RST;
        }
    }

    cout << Counter->Elem[Counter->Top];
}

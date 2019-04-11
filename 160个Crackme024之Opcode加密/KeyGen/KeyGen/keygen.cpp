#include <iostream>
#include <windows.h>
using namespace std;

int main(void)
{
	CHAR szName[20] = { 0 };
	cout << "Name:";
	cin >> szName;
	DWORD dwNum = 0x58455443;//程序实现填入的全局变量
	for (int i = 0; i < strlen(szName); i++)//用户名循环相加DWORD
	{
		void *p = &szName[i];
		_asm
		{
			mov edi, p;
			mov eax, dword ptr[edi];
			add dwNum, eax;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	DWORD dwPass = 0;
	while (1)
	{
		DWORD temp = (dwNum + dwPass) ^ 0x584554;
		temp -= (WORD)((dwNum + dwPass) >> 0x10);
		if (dwPass % 0x100000 == 0)  //这里if语句可以不要只是用来看看它在跑密码还是死机了
			cout << dwPass << endl;
		if (temp == 0x585426EB)//这就是上一个算法得到的正确的opcode
			break;
		dwPass++;
	}
	cout << "Pass:" << dwPass << endl;
	system("pause");
}
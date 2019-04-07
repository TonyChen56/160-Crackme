#include "pch.h"
#include <windows.h>
#include<iostream>
using namespace std;


int CalcKey()
{
	char* name;
	unsigned long serial = 0xF6EEDB88;
	unsigned long *p;
	name = new char[20]{0};

	cout << "请输入用户名:";
	gets_s(name, strlen(name) - 1);
	for (int i= 0x10 - 1; i >= 0; i--)
	{
		p = (unsigned long *)&name[i];
		serial ^= *p;
		serial--;
	}
	cout <<serial << endl;
	return 0;
}




int main()
{
	CalcKey();
	system("pause");
	return 0;
}
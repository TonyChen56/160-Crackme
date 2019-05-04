#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>


void ROR(DWORD * myd, unsigned char n)
{
	_asm
	{
		MOV EBX, myd;
		MOV eax, [EBX];
		MOV cl, n;
		ROR EAX, cl;
		MOV[EBX], eax;
	}
}


int main()
{
	char *username;
	unsigned long serial;
	serial = 0x44414D4E;
	unsigned long nTemp = 0;
	username = new char[260];
	memset(username, 0, 260);
	printf("请输入用户名：");
	scanf_s("%s", username, 260);
	for (int i = 0; i < strlen(username); i++)
	{
		nTemp += username[i];
		ROR(&serial, 1);
		ROR(&serial, i);
		serial ^= nTemp;
	}

	serial |= 0x10101010;
	delete[] username;

	printf("%x\n", serial);
	system("pause");
	return 0;
}

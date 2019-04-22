#include "pch.h"
#include <windows.h>
#include<iostream>
using namespace std;


int CalcKey()
{
	char username[20] = { 0 };
	int result = 0;
	char serial[20] = { 0 };
	printf("请输入用户名:");
	scanf_s("%s", username, 20);
	
	//计算用户名的结果
	for (int i = 0; i < strlen(username); i++)
	{
		if (username[i] < 0x41 || username[i]>0x5A)
		{
			username[i] -= 0x20;
		}
		result += username[i];
	}
	result ^= 0x5678;

	result ^= 0x1234;
	printf("%d\n", result);
	return 0;
}




int main()
{
	CalcKey();
	system("pause");
	return 0;
}
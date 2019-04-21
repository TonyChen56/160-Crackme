#include "pch.h"
#include <windows.h>
#include<iostream>
using namespace std;


int CalcKey()
{
	char username[20] = { 0 };
	char result[20] = { 0 };
	char serial[5] = { 0 };
	printf("请输入用户名:");
	scanf_s("%s", username, 20);
	if (strlen(username)<5)
	{
		printf("用户名长度不能小于5\n");
		return 0;
	}

	for (int i = 0; i < strlen(username); i++)
	{
		result[i] = username[i] / 0xA;
		if (result[i]>9)
		{
			result[i] /= 0xA;
		}
	}

	serial[0] = result[0];
	serial[1] = result[2];
	serial[2] = result[3];
	serial[3] = result[4];
	serial[4] = 0;

	for (int i = 0; i < 4; i++)
	{
		printf("%d", serial[i]);
	}
	printf("\n");
	return 0;
}




int main()
{
	
	CalcKey();
	system("pause");
	return 0;
}
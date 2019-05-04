#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>


int main()
{
	char username[20] = { 0 };
	char serial[20] = { 0 };
	printf("请输入用户名：");
	scanf_s("%s", username, 20);
	for (int i = 0; i < strlen(username); i++)
	{
		serial[i] = username[i] + 5;
	}
	printf("%s\n", serial);
	system("pause");
	return 0;
}

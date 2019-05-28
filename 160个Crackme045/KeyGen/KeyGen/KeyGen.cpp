#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>


int main()
{
	int result = 0x37;
	char username[20] = { 0 };
	char serial[20] = { 0 };
	printf("请输入用户名：");
	scanf_s("%s", username, 20);
	int usernameLen = strlen(username);
	if (usernameLen>=4)
	{
		for (int i = 0; i < usernameLen; i++)
		{
			result += username[i] << 9;
		}
	}
	printf("%d\n", result);
	system("pause");
	return 0;
}

#include "pch.h"
#include <iostream>
#include <windows.h>


int CalcKey()
{
	int result = 0x81276345;
	char username[20] = { 0 };
	printf("请输入用户名:");
	scanf_s("%s", username, 20);

	int UserNameLen = strlen(username);

	if (UserNameLen <5)
	{
		printf("用户名长度最少为五位");
		return 0;
	}
	
	for (int i = 0; i < UserNameLen; i++)
	{
		result += username[i];
		result ^= i << 8;
		result *= ~(i * UserNameLen) * (i + 1);
	}
	printf("%d\n", result);
	return 0;
}




int main()
{
	CalcKey();
	system("pause");
	return 0;
}
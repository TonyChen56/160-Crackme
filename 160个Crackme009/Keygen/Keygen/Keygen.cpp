#include "pch.h"
#include <iostream>
#include <windows.h>

using namespace std;


int CalcKey()
{
	char szUserName[20] = { 0 };	
	__int64 result = 0;
	char key[50] = { 0 };
	printf("请输入用户名:");
	scanf_s("%s", szUserName, 20);

	int iUserNameLen = strlen(szUserName);
	//求出每一位用户名的和
	for (int i = 0; i < iUserNameLen; i++)
	{
		result += szUserName[i];
	}

	result *= 1234567890;
	//转为字符串
	sprintf(key, "%I64d", result);
	//替换第4位和第9位
	key[3] = '-';
	key[8] = '-';

	printf("%s\n", key);
	return 0;
}




int main()
{
	CalcKey();
	system("pause");
	return 0;
}
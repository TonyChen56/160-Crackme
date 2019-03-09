#include "pch.h"
#include <iostream>
using namespace std;


int CalcKey1()
{
	int nKey = 0x1C48;
	char szName[10] = { 0 };
	int nCode = 0;
	int nTemp = 0;

	printf("请输入用户名:");
	scanf_s("%s", szName, 10);
	if (strlen(szName)!=5)
	{
		printf("请输入五位用户名\n");
		return 0;
	}

	//根据name字符串计算
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 5; j >= 1; j--)
		{
			nTemp += szName[i - 1] * szName[j - 1] * nKey;
		}
	}

	//取模
	nTemp = nTemp % 0xA2C2A;
	//反推code
	nCode = (0x50 - ((nTemp - 1) * 0x59 % 0x50)) + (nTemp - 1) * 0x59;
	printf("%d\n", nCode);
	return 0;
}


//计算第一个按钮OK所需要的密码
int CalcKey2()
{
	return 0;
}

int main()
{
	CalcKey1();
	system("pause");
	return 0;
}
#include "pch.h"

#include <iostream>
using namespace std;

//计算第二个按钮Cancella所需要的密码
int CalcKey1()
{
	int key = 0;
	int nTemp = 0;
	int num = 0;		
	char username[20] = { 0 };
	printf("请输入用户名,长度必须在六位以上:");
	scanf_s("%s", username, 20);

	//检查长度
	if (strlen(username)<=5)
	{
		printf("用户名长度不满足 请重新输入");
		return 0;
	}

	//求阶乘
	num = ((int)username[4]) % 7 + 2;

	nTemp = num;
	for (int i=1;i< nTemp;i++)
	{
		num *= i;
	}
	nTemp = num;
	int result = 0;
	//求用户名的ASCII和乘以阶乘
	for (int i=0;i<strlen(username);i++)
	{
		result += nTemp * username[i];
	}
	//取出正确的密码
	key = result - 0x7A69;
	printf("%d\n", key);

}


//计算第一个按钮OK所需要的密码
int CalcKey2()
{
	char key[20] = { 0 };
	char username[20] = { 0 };
	printf("请输入注册码,长度必须在六位以上:");

	//输入密码
	scanf_s("%s", key, 20);

	//检查长度
	if (strlen(key) <= 5)
	{
		printf("密码长度不满足 请重新输入");
		return 0;
	}
	for (int i= strlen(key)-1;i!=-1;i--)
	{
		username[i] = (key[i] * key[i] * (i + 1)) % 0x19 + 0x41;

	}
	printf("%s\n", username);
}

int main()
{
	CalcKey2();
	system("pause");
	return 0;
}
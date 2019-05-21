#include <iostream>
#include <windows.h>


int main()
{
	char username[20] = { 0 };
	printf("请输入用户名:");
	scanf_s("%s", username, 20);
	int usernameLen = strlen(username);
	if (usernameLen<6)
	{
		printf("用户名长度必须大于等于6");
	}
	//大写转小写
	for (int i=0;i<usernameLen;i++)
	{
		if (username[i]>='A'&&username[i]<='Z')
		{
			username[i] += 32;
		}
	}

	int v13 = 0;
	int TotalSum = 0;
	int i = 0;                                      // i初始化为零
	do
	{
		switch ((username[i- 1]))// 根据username[i]的值给v13赋值
		{
		case 0x61u:
			v13 = 0x18;
			break;
		case 0x62u:
			v13 = 0x25;
			break;
		case 0x63u:
			v13 = 0x42;
			break;
		case 0x64u:
			v13 = 0xC;
			break;
		case 0x65u:
			v13 = 0xD;
			break;
		case 0x66u:
			v13 = 6;
			break;
		case 0x67u:
			v13 = 0x36;
			break;
		case 0x68u:
			v13 = 0x2B;
			break;
		case 0x69u:
			v13 = 0x17;
			break;
		case 0x6Au:
			v13 = 0x2F;
			break;
		case 0x6Bu:
			v13 = 0x13;
			break;
		case 0x6Cu:
			v13 = 0x82u;
			break;
		case 0x6Du:
			v13 = 0x9Bu;
			break;
		case 0x6Eu:
			v13 = 0x92u;
			break;
		case 0x6Fu:
			v13 = 3;
			break;
		case 0x70u:
			v13 = 0x63;
			break;
		case 0x71u:
			v13 = 0x21;
			break;
		case 0x72u:
			v13 = 0x42;
			break;
		case 0x73u:
			v13 = 0x5C;
			break;
		case 0x74u:
			v13 = 0x29;
			break;
		case 0x75u:
			v13 = 0xC7u;
			break;
		case 0x76u:
			v13 = 0x66;
			break;
		case 0x77u:
			v13 = 0x58;
			break;
		case 0x78u:
			v13 = 0xA;
			break;
		case 0x79u:
			v13 = 0x28;
			break;
		case 0x7Au:
			v13 = 0x50;
			break;
		default:
			v13 = 0x5D;
			break;
		}
		TotalSum += v13;                          // 循环累加v13的值
		++i;
		//这里记得超出范围清掉高位
		if (TotalSum>0xFF)
		{
			TotalSum &= 0x00FF;
		}
	} while (i != 6);

	printf("%d-%d\n", TotalSum,0x4A7E*usernameLen);
	system("pause");
	return 0;
}

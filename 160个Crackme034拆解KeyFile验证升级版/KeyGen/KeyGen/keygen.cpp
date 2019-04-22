#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
	
	char username[15] = { 0 };
	char serial[5] = { 0 };
	int temp = 0x41;
	int result = 0;
	printf("请输入用户名，长度必须为14:");
	scanf_s("%s", username, 15);
	

	for (int i = 0; i < 14; i++)
	{
		username[i] ^= temp;
		result += username[i];
		temp++;
	}
	result ^= 0x12345678;


	char* p = (char*)&result;
	for (int i = 0; i < 4; i++)
	{
		printf("%c", p[i]);
	}
	printf("\n");
	system("pause");
	return 0;
}

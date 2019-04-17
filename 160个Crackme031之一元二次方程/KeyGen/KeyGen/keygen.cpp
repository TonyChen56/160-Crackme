#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
	char serial[16] = { 0 };
	char result[15] = { "VeiajeEjbavwij" };
	char key1[16] = { "200020002000200" };
	char key2 = '4';

	for (int i = 0; i < 14; i++)
	{
		serial[i] = result[i] ^ key2;
	}
	for (int i = 0; i < 14; i++)
	{
		serial[i] = serial[i] ^ key1[i];
	}

	printf("%s\n", serial);
	system("pause");
	return 0;
}
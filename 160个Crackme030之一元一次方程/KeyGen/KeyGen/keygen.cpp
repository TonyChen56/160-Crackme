#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
	char serial[13] = { 0 };
	char result[14] = { "qBQSYdXUe_B\\V" };
	char key[17] = { "2000200020002000" };

	for (int i = 0; i < 14; i++)
	{
		serial[i] = result[i] ^ key[i];
	}
	serial[13] = 0;
	printf("%s\n", serial);
	system("pause");
	return 0;
}
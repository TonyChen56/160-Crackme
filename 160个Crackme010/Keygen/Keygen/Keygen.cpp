#include "pch.h"
#include <iostream>
#include <windows.h>

using namespace std;


int CalcKey()
{
	char result[MAX_PATH] = { 0 };
	char key[MAX_PATH] = { "kXy^rO|*yXo*m\\kMuOn*+" };
	int keyLen = strlen(key);

	for (int i = 0; i < keyLen; i++)
	{
		result[i] = key[i] - 0xA;
	}

	printf("%s\n", result);

	return 0;
}




int main()
{
	CalcKey();
	system("pause");
	return 0;
}
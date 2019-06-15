#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <windows.h>
using namespace std;
int main()
{
	const char * path = "E:\\due-cm2.dat";
	FILE * keyfile;
	char * youkey = new char[40];         //输入用户名
	memset(youkey, 0, 40);
	unsigned char * writekey = new unsigned char[40];
	memset(writekey, 0, 40);             //存入文件的内容
	cout << "Enter your name:";
	scanf_s("%s",youkey,40);
	if (strlen(youkey) >= 8)
		cout << "用户名最多支持13位，其余部分将被截断!" << endl;
	int now = 0;

	writekey[0] = 0xEA;
	writekey[1] = 0xEB;
	writekey[2] = 0x01;
	for (unsigned int x = 0; x < 13 && x < strlen(youkey); x++)
	{
		writekey[x + 3] = youkey[x] ^ writekey[x];
	}
	if (strlen(youkey) >= 13)
		now = 16;
	else
		now = strlen(youkey) + 3;

	writekey[now] = 0x01;
	writekey[now + 1] = 0xD9;
	writekey[now + 2] = 0xD9;
	errno_t err = fopen_s(&keyfile, path, "w+b");
	if (err != 0)
	{
		cout << "file open or create failed!" << endl;
		system("pause");
		return -1;
	}
	rewind(keyfile);
	fwrite(writekey, sizeof(byte), 40, keyfile);
	fclose(keyfile);
	cout << "write file over!" << endl;
	delete[] youkey;
	delete[] writekey;
	system("pause");
	return 1;
}
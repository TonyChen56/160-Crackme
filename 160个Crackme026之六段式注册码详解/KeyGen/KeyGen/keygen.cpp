#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
	char username[20] = { 0 };
	printf("请输入用户名:");

	scanf_s("%s", username, 20);
	int usernameLength = strlen(username);
	if (usernameLength < 5)
	{
		printf("用户名长度必须大于5");
	}
	char key[50];
	char key1[10], key2[10], key3[10], key4[10], key5[10];
	sprintf(key1, "%d", (int)(username[usernameLength - 1] * 432.4*17.79 / 15 + username[0]));
	sprintf(key2, "%X", (int)(username[usernameLength - 1] * 432.4*17.79 / 15 - username[0] * 0x19));
	sprintf(key3, "%X", (int)(username[usernameLength - 1] * 432.4*17.79 / 15));
	sprintf(key4, "%d", (int)(username[0] * usernameLength - 0x1b));
	sprintf(key5, "%d", usernameLength);
	memset(key, 0, sizeof(key));
	strcat(key, key1);
	strcat(key, key2);
	strcat(key, key3);
	strcat(key, key4);
	strcat(key, key5);
	strcat(key, "-CM");
	printf("%s\n", key);
	system("pause");
	return 0;
}
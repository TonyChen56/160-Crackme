#include "pch.h"
#include <iostream>
#include <windows.h>
#include <time.h>
using namespace std;


int CalcKey()
{
	srand(time(NULL));
	byte key[6] = { 0 };

	while (true)
	{
		byte K0 = rand() % 123;
		byte K1 = rand() % 123;
		byte K2 = rand() % 123;
		byte K3 = rand() % 123;
		byte K4 = rand() % 123;

		//限制随机数不出现无意义字符
		if ((K0>=65&&K0>=90)|| (K0 >= 97 && K0 >= 122)||( K0 >= 49 && K0 >= 57))
		{
			if ((K1 >= 65 && K1 >= 90) || (K1 >= 97 && K1 >= 122 )|| (K1 >= 49 && K1 >= 57))
			{
				if ((K2 >= 65 && K2 >= 90 )|| (K2 >= 97 && K2 >= 122) || (K2 >= 49 && K2 >= 57))
				{
					if ((K3 >= 65 && K3 >= 90) || (K3 >= 97 && K3 >= 122) || (K3 >= 49 && K3 >= 57))
					{
						if ((K4 >= 65 && K4 >= 90) ||( K4 >= 97 && K4 >= 122) || (K4 >= 49 && K4 >= 57))
						{
							//满足限制条件
							if (K0 + K1 + K2 + K3 + K4 == K0 * 5)
							{
								key[0] = K0;
								key[1] = K1;
								key[2] = K2;
								key[3] = K3;
								key[4] = K4;
								key[5] = 0;
								break;
							}
						}
					}
				}
			}
		}

		
	}

	printf("%s\n", key);

	return 0;
}




int main()
{
	CalcKey();
	system("pause");
	return 0;
}
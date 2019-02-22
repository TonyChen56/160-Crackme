[TOC]

### 查壳

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqotnn9sj30ge086n08.jpg)
程序使用Virtual Basic写的 无壳

### 分析程序
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqpgok9aj308m07rtag.jpg)
程序是使用用户名和序列号的方式加密
载入OD 随便输入一个用户名和密码
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqptyzroj308b07f3zf.jpg)
来到第一处关键校验处 校验过程如下

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqv7qhtnj30pr093tal.jpg)
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqv7nujvj30qt0ayaca.jpg)
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqv7o02sj30q10c60ux.jpg)


1\. 求出了用户名的长度
2\. 将用户名长度乘以0x17CFB 得到结果 如果溢出则跳转
3\. 将结果再加上用户名的第一个字符的ASCII
4\. 将结果转为十进制
5\. 将结果和AKA进行拼接 得到最后的序列号

### 校验结果
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqsxxa6zj30oi0cvtew.jpg)
输入我们计算的一组序列号 注册成功

### 写出注册机
```C++
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <windows.h>
int main()
{
    //密码
    char key1[4] = "AKA";
    //输入用户名
    char username[10] = { 0 };
    printf("请输入用户名:");
    scanf_s("%s", username, 10);
    //1\. 取长度
    int iUsernameLen = strlen(username);
    //2\. 将用户名长度乘以0x17CFB 得到结果
    int result = iUsernameLen * 0x17CFB;
    //3\. 将结果再加上用户名的第一个字符的ASCII
    result = result + username[0];
    //4\. 将结果转为十进制 此步骤省略
    //5\. 拼接序列号
    char key[MAX_PATH] = { 0 };
    sprintf(key, "%s-%d", key1, result);
    //打印序列号
    printf("生成的序列号为:%s\n", key);
    system("pause");
    return 0;
}
```
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqsy1u2tj30nq07sgm7.jpg)
至此 002分析完成
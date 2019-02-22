[TOC]
## 查壳
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bq8eu6ryj30il0fgqe9.jpg)
程序是使用delphi编写的 无壳

## 分析前的准备
首先将目标程序拖入IDA 添加Delphi相关的签名
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bq9wuzjyj30q9091juh.jpg)
这可以帮助我们识别库函数 减少工作量

## 分析程序
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqatc7yaj30df09044k.jpg)
整个程序分为两部分 一个是序列号+用户名 一个是单纯的序列号 我们需要把两个部分都解出来
首先来解决序列号部分

### 分析Serial
随便输入一组错误的序列号 程序弹出一个失败的提示框

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqb3ovxjj30h60akk36.jpg)

那么首先可以从这个字符串入手 载入OD
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqbqefomj30qf0a6jy2.jpg)
找到字符串之后反汇编窗口跟随 可以看到在错误的提示上面有一个正确的提示 和一个关键跳转
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqc77rgtj30pu09t0xy.jpg)
再往上会看到一个比较函数 而关键跳转就是根据这个函数来执行的 所以我们在这个函数上下断点  输入一个错误的序列号 然后等待程序的断下
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqcl36d2j30qb095n3c.jpg)
根据传入的参数可知 函数将eax和edx进行比较 然后根据比较的结果进行跳转 所以第一个序列号是一个固定的硬编码`Hello Dude!`

### 验证结果
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqcyba66j30ky0c5whk.jpg)
输入`Hello Dude!` 程序弹出正确提示 序列号部分完成

### 分析Serial/Name
接下来分析用户名和序列号的部分 这个部分比单纯的序列号要有点难度
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqd6zvd2j30ew06z76c.jpg)
首先我们输入一个错误的用户名和密码，然后找到函数开始的地方下断点,分析整个函数的校验过程
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqdj5rlij30q90auwm0.jpg)
首先检测用户名长度是否大于等于4，如果不是则提示错误
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqduwbrwj30qp04rdgu.jpg)
接着是用户名的计算部分 也是唯一一处有用的计算部分 前面虽然也有对用户名进行计算 但是并没有采用计算出来的结果。计算过程如下
首先取出用户名的第一位，乘以0x29。然后再乘以2。保留结果。假设结果为16BE，也就是十进制的5822
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqe63fquj30qf0bjjye.jpg)
接着，将计算的结果转为字符串，然后和实现准备好的字符串进行拼接，结果为——`CW-5822-CRACKED`
这个就是序列号，也就是说序列号是根据用户名动态生成的。
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqf18uxlj30qe09on4e.jpg)
在关键的比较函数里也是可以直接看到结果的。

## 写出注册机
接下里对还原整个算法 算出注册机。代码如下
```C++
#define _CRT_SECURE_NO_WARNINGS        //去除安全函数的检查
#include<stdio.h>
#include <stdlib.h>
#include <windows.h>
int main()
{
    //密码
    char key1[3] = "CW";
    char key2[8] = "CRACKED";
    //输入用户名
    char username[10] = { 0 };
    printf("请输入用户名 长度必须大于等于4:");
    scanf_s("%s", username, 10);
    //判断长度
    if (strlen(username) < 4)
    {
        printf("长度必须大于等于4,请重新输入\n");
    }
    //根据用户名生成密码
    //计算用户名的第一位
    int username1 = username[0];
    int result1 = (username1 << 0x3) - username1;  //左移0x3位再减去原来的值
    //计算用户名第二位
    int username2 = username[1];
    result1 = (username1 << 0x4) + result1;        //左移4位再加上原来的结果
    //计算用户名的第四位
    int username4 = username[3];
    int result2 = username4 * 0xB;                //乘以0xB
    //计算用户名的第三位
    int username3 = username[2];
    result2 = (username3 * 0xE) + result2;        //乘以0xE在加上第四位的结果
    //再次计算第一位
    int result3 = username1 * 0x29 * 2;            //乘以0x29再乘以2
    //将result3转为ASCII 并拼接密钥
    char key[50] = { 0 };
    sprintf(key, "%s-%d-%s", key1, result3, key2);
    //打印key
    printf("密钥为:%s\n", key);
    system("pause");
    return 0;
}
```
### 验证结果
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqfc1g2uj30ox09jaa4.jpg)
程序根据输入的用户名算出了一组序列号 输入到目标程序里
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bqfc83oij30ep09q0ua.jpg)
结果正确。到此 整个程序的破解完成。
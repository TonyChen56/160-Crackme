[TOC]

## 查壳

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ify5ayu0j30h30fpwmg.jpg)

程序是使用delphi编写的 无壳

## 导出符号

既然是delphi编写的 那么我们首先把他放到IDA里，把所有的关于Delphi的签名全部加上

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ifz9beizj30uy0ejq47.jpg)

让会自动帮我们识别所有的Delphi的函数，可以看到 添加签名之后 未识别的函数就只有那么一丢丢，剩下的都是库函数。然后再导出为MAP文件，导入到OD里，可以极大的减轻负担。

## 寻找突破口

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ig1daa6uj30bk0bydhi.jpg)

来观察一下这个程序，两个编辑框 一个图片显示框，据说是注册之后会显示一个朱茵的图片。没有按钮，没有提示。这尼玛，完全没有思路啊怎么整？难道要下内存访问断点？

这里要借助一款Delphi的反编译工具Darkde4来帮助我们寻找突破口。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ig3tqhv9j30z00hy3zn.jpg)

打开窗体部分，我们看到 原来之前一直误以为的图片显示框是一个大按钮，而这个大按钮有两个对应的相应事件，分别是单击事件和双击事件。

接着再来来到过程部分。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ig7os8ozj30sg0amglp.jpg)

这里是整个程序的所有的响应事件 以及对应的RVA

- 第一个FormCreate是窗体的创建事件 这个不必关心，一般创建事件都是显示图形界面相关的操作
- 第二个事件名是chkcode，全称应该是checkcode，校验代码，至于校验的是什么代码？不知道
- 第三个KeyUp是响应的键盘的弹起
- 第四个DbClick是按钮的双击事件
- 第五个Click是按钮的单击事件

那么思路和突破口也就有了，对应单击和双击事件的RVA 直接去OD分析两个响应事件的具体实现部分。

## 单击事件

在单击事件的RVA 0x00457B8处下断点，随便输入一组用户名，从上往下分析所有的执行过程。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0igfuegwqj30sw0fmju7.jpg)

校验过程如下

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ihikf5r5j30uk0bkace.jpg)

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ihjse6umj30yx0j4wil.jpg)

1. 获取用户名
2. 获取用户名长度 将长度加上0x1E
3. 将长度转为字符串
4. 字符串拼接  拼接为长度+用户名+循环次数 
5. 整个算法循环18次 最后的结果如上图

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ihry4wlmj30vw0atmzn.jpg)

算法循环完了之后 比较[esi+0x30C]的值是否相等，如果不相等直接退出。如果相等，又是一轮重复的循环校验，接着提示注册成功。就是说前面的循环算法就是作者下的一个套，真正有用的部分就是这个比较，也就是[esi+0x30C]的值必须是0x85。

### 结论

[esi+0x30C]的值必须是0x85

OK 单击事件分析结束 接下来分析双击事件

## 双击事件

还是通过Delphi的反编译工具Darkde4的过程窗口找到双击事件的RVA。下断点，分析。注意，在这之前必须取消单击事件的断点，否则断不下来。

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0iieo00q8j30wa0bpjty.jpg)

还是同样的套路，一轮循环的验证之后，才来到真正有用的部分。

[esi+0x30C]和0x3E作比较，如果成立，就把[esi+0x30C]赋值为0x85，也就满足了单击事件的条件。

也就是说必定有一个地方，是把[esi+0x30C]赋值为0x3E。如果满足了这个条件，那么双击事件校验通过，单击事件也通过。这简直就是俄罗斯套娃啊。

### 结论

[esi+0x30C]的值必须为0x3E

## 再次寻找突破口

根据双击事件经验 我们能猜测，肯定有一个地方是把0x3E赋值给了[esi+0x30C]。

那么我们直接在OD中，右键->查找所有常量，输入3E，看看能不能找到mov [esi+0x30C],0x3E这样一条指令。如果能，那么这个就是真正校验的地方。

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0iiq2lrmhj30mh0a2wev.jpg)

虽然跟想象的不一样 但是大致还是差不多的 跟进去看看。然后，拉到函数最上面

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0iirs8d70j30x00em0v4.jpg)

之前导入的IDA的注释显示这个是chkcode的校验事件。就是之前我们在Darkde4过程窗口里看到的checkcode的响应事件，这个应该就是真正的校验函数了。OK ，下断点，开始分析

## chkcode事件

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0iivzn6baj30tu0fuabz.jpg)

这个在注册码输入的时候就断下来的，应该响应的是编辑框变换的响应事件。校验过程如下

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ikx4moxdj30v90ceacs.jpg)

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ikyf23ssj30ti0ct0vc.jpg)

首先获取用户名长度 然后将用户名长度+5，接着进行字符串拼接 拼接方式为黑头Sun Bird(用户名长度+5)dseloffc-012-OK(用户名)，拼接好的字符串就是密码。

## 校验结果

接着输入用户名和序列号，根据之前的分析过程 我们需要先双击，再单击才能够校验成功。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ig7os8ozj30sg0amglp.jpg)

但是这里需要注意一点，在过程窗口中有一个KeyUp键盘弹起事件，在你输入序列号的时候 他会检测是否有键盘弹起，如果没有 则不成功，这样做的目的是为了防止复制粘贴。破解的方法也很简单，在复制粘贴完序列号之后随便按一个键(我按的是方向键  因为不会影响到输入结果)，接着双击 再单击。可以看到，成功注册完成。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0il0fu5qxj30c10c276t.jpg)

## 写出注册机

编写注册机 代码如下

```C++
#include "pch.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

int main()
{
	char key1[50] = "黑头Sun Bird";
	char key2[50] = "dseloffc-012-OK";
	char username[20] = { 0 };
	printf("请输入用户名:");
	scanf_s("%s", username, 20);
	printf("序列号为:\n");
	printf("%s%d%s%s\n", key1, strlen(username)+5, key2, username);
	system("pause");
}
```

校验结果 随便输入一个用户名，复制到程序，然后随便按一个按键，双击 单击。OK校验成功！

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ilhh1c4rj30ux0fck1b.jpg)

最后附上我的Github链接：https://github.com/TonyChen56/160-Crackme

需要分析过程和udd等相关文件的可以去我的Github下载。
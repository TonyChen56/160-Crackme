[TOC]

之前在网上搜索了下关于这个Crackme的分析，好像并没有看到比较详细的分析文章，4C法的原理也没有解释的很清楚。所以就发出来了。

## 查壳

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bz34vve9j30jl0bdmzb.jpg)

程序使用VB5.0写的无壳

## 去Neg

程序打开后 会有大概7秒的弹窗

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bz4vposdj308u06ltbl.jpg)

在这7秒内 什么都不能干，所以第一件事就是把这个7秒的neg给去掉

### 方法一 Timer搜索法

首先将内存定位到程序开始处0x401000

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bz7qrx65j30oz0cejtj.jpg)

搜索Timer，Timer是VB程序默认的定时器变量，

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bz936hc9j30ra0g7ad1.jpg)

我们找到Timer之后 在上面的位置可以看到一个0x1B58，这个是计时器的秒数。也就是十进制的7000,7000毫秒就是7秒。所以第一种去Neg的方法就是将0x1B58改为0x0001

但是此种方法也有一定的局限性，如果程序的作者将计时器的默认名称改掉之后 根本无法在内存中搜索到Timer关键字 也就无法下手。下面介绍一种通用的解决方法

### 方法二 4C法

#### Virtual Basic可执行程序结构研究

对于Visual Basic5/6编译生成的程序，不管自然编译还是伪编译，其程序入口点处的结构都是一样的。来到OEP处：

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bzd0oybtj30qo0i6jul.jpg)

指针0x4067D4指向的结构就是Virtual Basic程序的VBHeader结构，由此伸展开，整个VB程序的框架就在这了。

##### Virtual Basic程序框架结构

可能的Virtual Basic程序框架结构如图

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0m2e1rvs0j30r70a2q4o.jpg)

##### TVBHeader结构定义

```C++
Signature		        array[1..4] of char;		//00H签名，必须为VB5！
Flagl			    	WORD；					   //04H.未知标志
LanguageDll	        	array[1..14] of char;		//06H语言链接库名，通常为”*"或者vb6chsdll
BackLanguageDll     	array[1..14] of char;   	//14H.后备语言链接库名
RuntimeDLLVersion 		WORD；					   //22H运行库版本
Languageld			    DWORD：				       //24H语言标识
BackupLanguageID 		DWORD；				       //28H，未知标志
aSubMain		        DWORD；				       //2CH，不为00000000则指向Sub MainO指针
aProjectlnfo		    DWORD					   //30H.指针，指向tProjeclnfo结构  
Plag2					WORD；					  //34H未知标志
Flag3					WORD；					  //36H.未知标志
Flag4					DWORD；					  //38H未知标志
ThreadSpace 			DWORD；					  //3CH.线程空间
Flag5					DWORD；					  //14H未知标志
ResCount				WORD；					  //44H.数量，表示form与cls文件个数
ImportCount				WORD；					  //46H.数量，引用的ocx、dll文件个数
Flag6					BYTE：					 //48H未知，可能代表运行时程序所占内存大小
Flag7				    BYTE；					 //49H未知，可能与程序启动时花费时间有关
Flag8					WORD；					 //4AH.未知
AGUTTable 				DWORD;					 //4CH.指针，指向Form GuI描述表
AExdCompTrble		    DWORD;				      //50H指针，指向“引入的ocx、dll文件描述表
Aproicclpescrinion 		DWORD;					 //54H，指针；指向tProjectDescriptionTable的指针
OProjectExename			DWORD					 //58H.偏移，Offset ProjectExename
OProjectTitle 		    DWORD；				    //5CH，偏移，Offset Projectitle
OHelpFile 				DWORD；					//60H.偏移，Ofset Helpile
OProjectName 			DWORD.					 //64H.偏移，Offset ProjectName
```

##### AGUTTable(4C的位置)结构定义

```c++
Signature				DWORD					//00H.必须是50000000
FomID					TGUID					//04，可能是以GUID方式命名的formID
Index					BYTE   					//24H 窗体的序号
Flag1					BYTE					//28H 第一个窗体的启动标志，可能是90 也可能是10
AGUIDescriptionTable    DWORD					//48H指针指向以“FFCC…“开始的FormGUI表
Flag3					Dword					//4CH.意义不明
```

以`AfKayAs.2.Exe`为例说明：

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0m3its8eoj30lb0eg40h.jpg)

其对应每个字节的含义如下表

| offset | 含义                                |
| ------ | ----------------------------------- |
| 00H    | VB5!                                |
| 04H    |                                     |
| 06H    | 语言链接库名                        |
| 14H    |                                     |
| 22H    | 04代表msvbvm50.dll版本              |
| 24H    | 语言标识                            |
| 28H    |                                     |
| 2CH    | 00000000表示非由sub_main启动        |
| 30H    | 指向ProjectInfo结构                 |
| 34H    |                                     |
| 36H    |                                     |
| 38H    |                                     |
| 3CH    |                                     |
| 40H    |                                     |
| 44H    | 只有2个FROM                         |
| 46H    | 引用的OCX DLL文件个数为0            |
| 48H    |                                     |
| 49H    |                                     |
| 4AH    |                                     |
| 4CH    | Ptr 0040126C                        |
| 50H    | 若没有引入的ocx,dll文件，此指针无效 |
| 54H    | 指向tProjectDescriptionTable的指针  |
| 58H    | ProjectExename                      |
| 5CH    | ProjectTitle                        |
| 60H    | HelpFile                            |
| 64H    | ProjectName                         |

#### 4C法实战

在程序载入到入口点之后的第一个push也就是VBHeader的位置，数据窗口跟随。

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bzd0oybtj30qo0i6jul.jpg)

然后在数据窗口输入刚才的地址+4C，找到form GUI描述表的指针

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bze6qv4nj30mn0a875u.jpg)

数据窗口跟随DWORD

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bzf4hy05j30ni0b2ta9.jpg)

接着会来到一块有规律的数据区域，每一个小块的数据是每一个窗体的信息，其中00和01是窗体的序号 而10是第一个窗体的启动标志。

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bzfstv7vj30r30g1n03.jpg)

所以 我们只要把第一个窗体数据块的00改成10 然后把第二个窗体的数据块的10改成00，或者将窗体的序号调换。即可去除neg。

## 破解序列号

这个程序和002那个程序一模一样 无非是换了套浮点算法而已 就算你看不懂浮点指令 也可以通过单步跟踪 观察FPU栈的数值猜测到序列号

首先输入一个错误的用户名和序列号

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0f0xozan0j308z06r3zr.jpg)

下面是验证步骤

#### 第一步

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0f0vrcb5cj30ru0cmq5l.jpg)

还是和002一样 获取用户名长度 然后根据得到的结果算出来一个值 我这里这个结果为622287

#### 第二步

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0f0zp5s8bj311p0ckq64.jpg)

第二步 首先将刚才计算的结果转为浮点数 放入到FPU栈 然后将结果加上2.0 得到622289。逆向这个程序你并不需要看懂每一条浮点指令，只需要单步跟踪 然后时时观察FPU栈的情况。

#### 第三步

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0f12kr9mlj310u0cdq5y.jpg)

还是将刚才的结果转为浮点数并入栈 然后将622289乘以一个值再减去一个值 最后得到结果ST0=1866865

#### 第四步

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0f14inoksj311o0ar76s.jpg)

将刚才的结果1866865减去一个值 得到结果1866880 到此校验就结束了

#### 验证结果

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0f16mpcrfj30lm0cr403.jpg)




## 编写注册机

接下来使用Keymake编写注册机，设置如下

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0m5gzuezgj30p00hiq4j.jpg)

接着运行注册机 出现了正确的序列号，至于为什么会出现两次，这个我也不知道，这个软件我用的并不熟。哈哈 就这样吧。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0m5ig12boj30m40b2qc5.jpg)

需要udd文件和分析文档的可以到我的Github下载：https://github.com/TonyChen56/160-Crackme


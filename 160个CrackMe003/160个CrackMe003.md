[TOC]

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

### 4C法

在程序载入到入口点之后的第一个push数据窗口跟随。

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bzd0oybtj30qo0i6jul.jpg)

然后在数据窗口输入刚才的地址+4C

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bze6qv4nj30mn0a875u.jpg)

然后在第一个地址处数据窗口跟随DWORD

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

接下来输入刚刚得到的结果1866800 OK 成功破解！由于我对浮点指令并不是很熟，也是为了破解这个Crackme才去学习的相关知识，所以这道题只是勉强破解出来了，不知道算不算取巧 注册机就不写了吧。实力不够啊 哈哈！


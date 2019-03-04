[TOC]

这个Crackme跟004一样是同一个作者，不过变态多了，真的是层层设防啊。

## 查壳

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pd732w7hj30gf09jab2.jpg)

程序是使用Delphi编写的，加了个UPX壳，随随便便就能脱掉。

### 关于脱壳

这个壳的话，有三种脱的方式，PEiD通用脱壳器 手动脱和专用的脱壳器，这里建议用专用的脱壳器 反汇编代码和对齐比较准确方便后面的分析，我用脱壳器脱的时候报错了，所以直接从网上找了一个脱好了的。

## 使用DarkDe分析程序

把脱壳后的程序拖如DarkDe

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0pegmz1zxj30v20hd75w.jpg)

首先来观察一下窗体，发现这里有一个隐藏的Edit框。

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0pehvi0w6j30ix0dat8z.jpg)

再来观察一下事件按钮，有如下几个响应事件：

- 表单创建
- Timer1定时器
- 按钮的鼠标按下事件
- 双击事件
- Edit2的双击事件
- 表单的鼠标移动事件
- Image1的鼠标按下事件
- Image2的鼠标按下事件
- Image3的鼠标按下事件
- Image4的鼠标按下事件
- Timer2定时器
- 按钮点击事件

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0petv6wtoj30jb0c93yi.jpg)

接着再来看一下控件的ID，这个会方便以后的分析，注意了，如果你脱壳用的是手动的方法或者是通用脱壳机，这个控件ID是显示不出来的。所以能用专用脱壳器脱尽量不要手脱。

## 分析程序

### 导出符号

首先为了方便我们分析，把程序拖到IDA里，添加Delphi的所有的签名，然后导出map文件，加载到OD中，方便后续的调试。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pnprx53qj30l4055aa9.jpg)

### 从字符串入手

整个程序整体的框架已经了解的差不多，那么接下来就开始分析这个程序。首先从字符串入手。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0poe10hjoj30rf0acjrq.jpg)

找到这个注册成功的字符串，跟进去。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pof9cemxj30ti05mgme.jpg)

拉到函数头的位置，我们看到右边有个IDA的注释，说明这个是Timer2的响应事件。然后这里有连续的五条跳转指令，都是跳到函数结束的位置。那么要想注册成功，就必须让这五个条件不成立。

### 分析第一层防护

右键->查找所有常量->查找0x3C4，找到0xC34被赋值的位置，我们的目的是不让这个地方被赋值。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pp644lgmj30iv05hq30.jpg)

接着找到函数头的位置，IDA的位置显示这个是表单的创建事件

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ppb02woaj30td09cmyb.jpg)

### 分析表单创建事件

接着，在函数头的位置下断点，分析整个表单创建事件。分析表单创建事件的目的是为了让ebx+0x304这个位置的值不被赋值为0xC34。整个事件的代码分析如下：

#### 初始化控件

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pq5q7uajj30tc09ujtj.jpg)

首先是初始化控件，这个不是重点。

#### 读取文件内容

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pq82hqiaj30sr09bmz2.jpg)

接着获取`C:\ajj.126.c0m\j\o\j\o\ok.txt`的文件句柄，读取文件，然后检索IO输入操作结果，如果检索失败，直接退出函数。我这个脱壳后的文件是从网上下载的，原来是X盘，好像是被人改成C盘。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pqbijsz4j311f0dzdk7.jpg)

如果文件存在，那么从文本中读取字符串，然后比较文本中的字符串是否是作者要求的，这个123456789是我自己写入txt文件中测试用的，如果要复制作者要求的那个字符串，必须把从0x20开始到00的所有十六进制复制下来。如果字符串比较通过，那么就会跳过`[ebx+0x304]`被赋值为`0xC34`的过程，至此整个表单创建函数就完成了。

#### 破解第一层防护

创建文件，路径为: `C:\ajj.126.c0m\j\o\j\o\ok.txt`，然后将`20 61 6A 6A D0 B4 B5 C4 43 4B 6D 65 D5 E6 C0 C3  21 FF FF  `用十六进制编辑器写入到文本。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pqtloiibj30880adaax.jpg)

成功破解第一层防护之后，隐藏的Edit编辑框就出来了。但是这个编辑框是被禁用的。

### 分析第二层防护

计时器的五个条件我们已经满足了第一个，接下来回到计时器的入口，从第二个条件入手。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0prlvkcwrj30sq0ceacd.jpg)

条件二是默认已经满足了，跳转不会实现。那么我们现在要满足条件三，让`[ebx+0x310]`赋值为`0xF94`。

同样，搜索`0xF94`这个常量，找到赋值的地方。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0provooplj30h604m748.jpg)

第一条，直接跟进去，拉到函数头部，发现这里是表单的鼠标移动事件。接下来开始分析这个事件的所有的代码。

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0psuhi8quj30t00a4jsj.jpg)

#### 分析表单的鼠标移动事件

接下来在函数头部下断点，然后从左上角移动到表单，触发断点，分析整个函数。

##### 获取坐标

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ptn1e8xsj30s709xwg2.jpg)

首先把参数一和参数二给edx和eax，我们必须知道这两个参数是什么。因为这是鼠标点击事件，初步猜测是鼠标的X和Y坐标。接下来做四次测试

```C++
左上角：edx=0         eax=0x14
右上角：edx=0         eax=0xE8
左下角：edx=0x13B     eax=0x3
右下角：edx=0x13B     eax=0xFD
```

当从不同的位置移入表单，对应的eax和edx的值都不相同，所以判断这两个参数是横纵坐标，而且坐标系的原点在左上角。

##### 校验图片

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ptr64yhhj30w50chwh6.jpg)

接着函数取出了一个图片对象，然后判断是否是这个图片对象，

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0petv6wtoj30jb0c93yi.jpg)

可以看到Image3的ID是0x2E0,所以只有当图片为Image3的时候才能满足条件，而Image3根据测试是"性相近"。

##### 校验坐标

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ptykd618j30t6047756.jpg)

接着将横坐标和0xE2做比较，将纵坐标和0x12C做比较，小于或等于跳转，那么就是说如果想让他不跳转，横纵坐标必须大于这两个值。整个表单的原点是左上角，那么我们必须从右下角移入才能满足条件。

##### 再次校验图片和坐标

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0puakv7s2j30sd09kgnl.jpg)

接着再次校验图片是否是Image2 Image2是性本善，然后校验横坐标和纵坐标，这一次必须从左下角移入才能满足条件。然后再判断`[ebx+0x30C]`这个地址的值是否是`0x9`，默认这个值是0x9，所以这里又是一层防护。只有满足了[ebx+0x30C]这个地址的值是9，才能把ebx+0x310赋值为0xF94，也就是满足我们的第二层防护。

### 分析第三层防护

接下来还是一样，要让[ebx+0x30C]这个地址的值不为9，查找所有常量。符合要求的只有第一条和第二条。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0purtv041j30od08b3yu.jpg)

而第一条跟进去发现这是之前分析过的表单创建的时候就被赋值为30C，所以目标不是这里，是第二条。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0puylpldvj30s90andhy.jpg)

第二条跟进去，应该就是这里了。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pv078lzaj30sj09rta9.jpg)

接下来，拉到函数头的位置，

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pv1nfo6aj30t908bdgk.jpg)

发现这里是Edit2的双击事件，而Edit2目前是禁用的，所以双击事件根本断不下来，这又是一层防护。

### 分析第四层防护

想要破解第二层防护必须先破解第三层防护，想要破解第三层防护，就必须先破解第四层防护，这是在玩俄罗斯套娃啊。那么我们这一步必须让Edit2控件变成启用状态。怎么做呢？利用刚才看到的控件ID。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0petv6wtoj30jb0c93yi.jpg)

Edit2的控件ID是0x2F0，接着搜索所有常量。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pvew6ggtj30f406d74i.jpg)

总共有这么多和0x2F0相关的，我们必须一个一个去看才能找到禁用Edit2的那一个。经过排查发现真正的开启Edit2控件的是第四个。

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0pvub2ohvj30s6052wf5.jpg)

这个函数并不长，是Panel1的双击事件，想要让Edit2控件被启用，就必须让eax+0x308这个地址被赋值为0x29D。

Panel是什么东西呢。打开一下DarkDe，中间的这个大按钮就是Panel1，注意不是图片是按钮。所以想要触发这个事件必须是双击空白处才能让程序断下。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pwp3i9wbj30r80fr0t6.jpg)

### 分析第五层防护

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pw3jw1ywj30iv072jrn.jpg)

查找常量，第三条，跟进去,这里是注册按钮的鼠标按下事件。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pwayntwlj30qm0d8di1.jpg)

首先比较edx=0x230D，这个是直接能通过的，没有给咱下套，我还以为又套了一层。接着比较cl是1，cl的话必须是右键单击，还是用猜，鼠标无非就量个参数，坐标和左右按键。eax+0x308的初始值是0x28E，右键单击一次增加3，所以只要将注册按钮右键单击5次，第五层防护就算过了。

#### 破解第五层防护

右键单击五次注册按钮

### 破解第四层防护

第五层防护已经过了，那么接下来来到没有解决的第四层。

鼠标右键单击五次注册按钮，然后等图片过了，双击大按钮，响应事件断下，

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pwu26091j30sm0botat.jpg)

这个时候可以看到eax+0x308的值已经是29D了。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pwvl99y1j307i0act91.jpg)

Edit2编辑框也能正常使用了。

### 破解第三层防护

Edit2编辑框的编辑框已经启用了，那么接下来双击编辑框

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0pwz5bnmgj30rk0ajdh1.jpg)

程序成功断下，接下来分析Edit2的双击事件。我们的目的要让[ebx+0x30C]被赋值为eax，达到破解第二层防护的条件。

#### 分析Ddit2双击事件

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0pxm03f02j30sn08wtal.jpg)

首先获取Edit2的内容，然后获取长度，长度必须等于8，然后比较第二个字符是否是0x5F，0x5F就是下划线。

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0pxniv91yj30v808wmyy.jpg)

接着获取Edit2的内容，比较第六个字符是否是0x2C，然后获取Edit1的内容和长度，然后将长度+3之后除以3，

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0pxpbzhkgj30ue09uwgc.jpg)

比较余数，余数必须为0。就是说Edit1的长度必须为3的倍数。至此，Ddit2双击事件分析完成。

**破解第三层防护:**第一个编辑框长度必须是3的倍数，第二个编辑框第二个字符必须是下划线，第六个字符必须的逗号

### 破解第二层防护

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0py8o1bmnj30vr0d3din.jpg)

当破解了第三层防护的时候，第二层防护的比较条件也就满足了。继续往下分析。

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0pyf3a16xj30ty09yjtn.jpg)

接着往下走，程序会获取第一个编辑框的内容然后和ajj做比较，也就是说第一个编辑框必须为ajj。然后会把数字转成字符串显示到控件。

![](http://ww1.sinaimg.cn/large/006Rs2Luly1g0pygc94lsj308r0ad0t1.jpg)

成功了之后，这里会出现一个数字。

### 破解第六层防护

接下来来到总验证的地方。也就是一开始的定时器，看看现在的校验情况如何。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0qkmjhhnej30tp0ahtah.jpg)

我们已经过掉了前三个验证，接下来要过掉第四个，也就是要让ebx+0x318等于ebx+0x314。那么要搞清楚0x318和0x314这两个位置是怎么被赋值的

#### 分析0x318

搜索所有常量->0x318，

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0qkpflel8j30ep07cweq.jpg)

除了第一个设置初始值，剩下的所有对这个地址的变量做加法运算。随便点进去第二个，增加二的位置，

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0qkrjwgbnj30tm0acmyw.jpg)

这里是图片1的鼠标按下的位置，而且显示的是注册不成功。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0qkwieritj30u007u75q.jpg)

首先判断鼠标按下的是左键还是右键，如果是左键+2，右键+0x11。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0qkpflel8j30ep07cweq.jpg)

再次回到所有常量的位置，那么规律也就出来了，这几个加法分别对应四张图片，根据点击的按键不同，加的值也不同。0x318这个位置搞明白了，接下来去分析0x314。

#### 分析0x314

搜索所有常量->0x314。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ql308exqj30gt061wem.jpg)

0x314被赋值的地方也有很多，分别对应五个值。这个几个地址相差的值比较小，说明是同一个函数，随便点进去一个。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ql63awkjj30u408pac0.jpg)

这里是之前分析过的表单的鼠标移动事件，这个地址的值取决于eax的值，有四种情况第一种情况赋值为0x41，第二种情况赋值为0x3D，第三种情况赋值为0x34，第三种情况赋值为0xDF。然后根据不同的情况，在界面上会显示一个数字。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0ql90d0p7j30th0b440l.jpg)

而eax的值来自于0x30C这个位置，又得继续往上找。

### 破解第七层防护

接下来还是查找所有常量->0x30C。

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0qlc7n8zxj30qq0abwga.jpg)

来到给0x30C赋值的位置，这里是编辑框的双击事件，之前已经分析过了，但是我没有分析上面两个call具体是做什么的。

第一个call是获取磁盘的剩余空间，然后将eax做一系列的处理，交给第二个call，第二个call是纯粹的算法了。

最后这两处结合，是根据显示的数字不同，然后对图片采用不同的点击方案。

## 校验步骤总结

1. 创建文件，路径为: `C:\ajj.126.c0m\j\o\j\o\ok.txt`，然后将`20 61 6A 6A D0 B4 B5 C4 43 4B 6D 65 D5 E6 C0 C3  21 FF FF  `用十六进制编辑器写入到文本。
2. 鼠标右键点击五次注册按钮
3. 鼠标左键双击图片框的空白处 
4. 用户名输入ajj
5. 密码输入1_345,78(第二位必须的下划线，第六位必须的逗号，长度必须8位，其余随便)，然后鼠标左键双击
6. 在图片是性相近的时候，鼠标从右下角移入软件框内
7. 在图片是性本善的时候，鼠标从左下角移入软件框内
8. 根据显示的数字不同，最终的解决方案也不同，具体如下：
   - 数字为0——在“习相远”图片时左键点击图片2次，在“人之初”图片时右键点击图片3次
   - 数字为1——在“习相远”图片时左键点击图片1次，在“习相远”图片时右键点击图片2次
   - 数字为2——在“性本善”图片时左键点击图片2次，在“性相近”图片时右键点击图片2次
   - 数字为3——在“习相远”图片时左键点击图片1次，在“习相远”图片时右键点击图片8次
   - 数字为4——在“习相远”图片时左键点击图片2次，在“人之初”图片时右键点击图片3次

所有的步骤都执行完毕之后，等待计时器执行完成之后，

![](http://ww1.sinaimg.cn/large/006Rs2Lugy1g0qmau0e7zj307f09oglm.jpg)

注册按钮会变成注册了，鼠标移上去会显示厉害厉害真厉害。

需要相关文件的可以到我的Github下载：https://github.com/TonyChen56/160-Crackme






















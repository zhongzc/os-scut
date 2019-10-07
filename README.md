# os-scut

## 实验一：进程和线程的创建

1. 在linux下编写一个应用程序，命名为an_ch2_1b。这个程序不断地输出如下行：
Those output come from child,[系统时间]
另外写一个应用程序，命名为an_ch2_1a。这个程序创建一个子进程，执行an_ch2_1b。然后这个程序（an_ch2_1a）不断地输出如下行：
Those output come from parent,[系统时间]
观察程序运行的结果，并对你看到的现象进行解释。

2. 在linux环境下编写一个控制台应用程序，程序中有一个共享的整型变量shared_var，初始值为0；创建一个线程并使其立即与主线程并发执行。新创建的线程与主线程均不断地循环，并输出shared_var 的值。主线程在循环中不断地对shared_var 进行加1操作，即每次循环shared_var 被加1；而新创建的线程则不断地对shared_var 进行减1 操作，即每次循环shared_var 被减1。观察程序运行的结果，并对你看到的现象进行解释。

3. 提交源代码以及实验报告。

## 实验二：进程和线程同步和互斥

1. 生产者消费者问题（信号量）

参考教材中的生产者消费者算法，创建5个进程，其中两个进程为生产者进程，3个进程为消费者进程。一个生产者进程试图不断地在一个缓冲中写入大写字母，另一个生产者进程试图不断地在缓冲中写入小写字母。3个消费者不断地从缓冲中读取一个字符并输出。为了使得程序的输出易于看到结果，仿照的实例程序，分别在生产者和消费者进程的合适的位置加入一些随机睡眠时间。

可选的实验：在上面实验的基础上实现部分消费者有选择地消费某些产品。例如一个消费者只消费小写字符，一个消费者只消费大写字母，而另一个消费者则无选择地消费任何产品。消费者要消费的产品没有时，消费者进程被阻塞。注意缓冲的管理。

2. 用线程实现睡觉的理发师问题，（同步互斥方式采用信号量或mutex方式均可）

理发师问题的描述：一个理发店接待室有n张椅子，工作室有1张椅子；没有顾客时，理发师睡觉；第一个顾客来到时，必须将理发师唤醒；顾客来时如果还有空座的话，他就坐在一个座位上等待；如果顾客来时没有空座位了，他就离开，不理发了；当理发师处理完所有顾客，而又没有新顾客来时，他又开始睡觉。

3. 读者写者问题

教材中对读者写者问题算法均有描述，但这个算法在不断地有读者流的情况下，写者会被阻塞。编写一个写者优先解决读者写者问题的程序，其中读者和写者均是多个进程，用信号量作为同步互斥机制。

4. 提交源代码以及实验报告。

## 实验三：实现一个文件查找程序

仿照unix操作系统中的find命令，在实现一个myfind命令。myfind命令从指定的目录下开始，递归地查找指定文件。其命令格式如下：
```
myfind PATH -option 参数 [-print] [-exec 命令 {} \;]
PATH：查找的起始目录。
-option 参数：用来控制搜索方式，具体如下（以下只给我常用的）：
    -name "文件"：指定要查找的文件名，可以有通配符?*等。
    -prune 目录：指出搜索时不搜索该目录。
    -mtime +n或-n：按时间搜索，+n：表示n天之前修改过的文件；-n：表示今天到n天前之间修改过的文件。
    -ctime +n或-n：也是按时间查找，+n：表示n天之前创建的文件；-n：表示今天到n天前之间创建的文件。
-print：将搜索结果输出到标准输出。
```

可选的实验1：实现-exe选项。
    -exec：对查找到的每一个结果实行指定的程序。格式如下：
```
-exec 程序名
```
提示：执行指定的程序可以创建一个进程执行

可选的实验2：
将你实现的myfind命令改为用多线程的方式实现。比较多线程实现的myfind与单线程实现的myfind的执行结果、效率等，并在实验报告中给出你的解释。

## 实验四：实现一个unix命令解释程序

1. 在linux中实现一个命令执行程序doit，它执行命令行参数中的命令，之后统计
    1）命令执行占用的CPU时间(包括用户态和系统态时间，以毫秒为单位)，
    2）命令执行的时间，
    3）进程被抢占的次数，
    4）进程主动放弃CPU的次数，
    5）进程执行过程中发生缺页的次数

2. 在linux中实现一个简单的命令解释程序，功能要求：
    1）同时支持内部命令和外部命令，内部命令支持两个（cd、exit）
    2）支持后台命令

提示：实验中可能用到的系统调用如下：
- fork() — 创建一个新进程
- getrusage() — 取得进程的资源使用情况
- gettimeofday() — 取当前的时间
- execve() — 装入一个程序并执行
- wait() — 等待子进程结束
- chdir() — 改变进程的工作目录
- strtok() — 字符串解析

3. 提交源代码以及实验报告。

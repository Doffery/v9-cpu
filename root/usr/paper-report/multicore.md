#第四次论文阅读
\\Dong 2015210938
\\2016.04.06

##mTCP: A Highly Scalable User-level TCP Stack for Multicore Systems
###论文内容简介
提出了以User-level的TCP栈，在多核系统中能够高效处理短TCP连接。
###论文内容
论文使用User-level TCP栈通过library与驱动直接交互，免去传统系统中的kernel部分，省去了syscall的时间，极大的提高了TCP处理速度。

通过PSIO library与驱动交互，对于每一个应用线程，配合一个mTCP线程不断查询网卡，返回包或者发送包。并且绑定到同一核，实现locality，使用lock-free的数据结构，给每个核都配备了一套资源flow pool, socket buffers等。

因为使用mTCP不断查询，能够很好的为程序提供批处理。

使用对TCP连接控制包优先处理，并且预先分配大量内存供存储包加快了系统对短TCP连接的处理。

设计了较好的API，只需应用程序作出较小更改即能兼容运行。
###论文实验
实现mTCP后，实现（更改了）lighttpd等TCP程序，并且实验验证了对短TCP连接的高效处理能力。比较了TCP处理能力，证明了自己的contribution。
###论文贡献点
实现的User-Level TCP栈解决了资源共享问题，并且单核处理有很好的locality
使用批处理处理package
设计的API能够较好的兼容现有应用
###论文缺点
没有讨论batch带来的Latency
全User-Level处理带来的安全性问题呢
不断查询NIC，CPU占用高
应用多thread能友好处理吗


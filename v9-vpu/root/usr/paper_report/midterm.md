
#Midterm Paper Presentation
\\Dong 2015210938
\\2016.04.09
##MegaPipe: A New Programming Interface for Scalable Network I/O

###文章研究的问题
和mTCP解决的相同的问题，解决现有操作系统对短连接，短消息和多核处理不够好的问题：
+	system call overhead
+	shared listenng socket
+	file abstraction？？？

###设计内容：MegaPipe
MegaPipe同时需要更改user-space library和linux内核，同时应用程序也需要根据api做调整。

####引入了channel
用于应用进程和core通信，每两个channel之间相互独立。一个channel可以对应于多个core。

channel是一个抽象的概念，表示core与user之间的通信与其他core与另外user通信的独立性。

每一个channel有自己的一份data structure，如listening socket等。

和轮训检查是否有package不同，这里采用completion notification model。Kernel采用notification的形式将command结果返回给应用程序。

####设计了Lightweight Socket
原有的socket为了兼容VFS，采用和全局文件共享的FD方式，但是TCP中socket有两个特点：
+ 很少共享
+ 生存时间短暂

因而为了兼容而使其配合全局使用得不偿失。

设计的lwsocket只在每个channel中本地可见，并且不采用最小FD id的方式，采用随机分配方式（万一碰撞呢？），避免了全局的socket使用共享，提高了效率。

并且，如果程序指定，MegaPipe可以将lwsocket转化成普通的socket。

####将System Call按batch方式处理
因此system call处理总需要进行mode之间的切换，这会造成很大的时间消耗。因此对system call进行batch处理，可以提升性能。

在MegaPipe中，这部分操作都是由user-level的library来做的，对application来讲是透明的。

###实现
主要有三个部分：Kernel， User-level library和application modification。

Kernel更改主要是对I/O操作的MegaPipe支持，主要是lwsocket，batch I/O command还有event queue。增加了一个module 1800行左右，并且更改了系统内核400行左右。

Library只是kernel module的一个包装，400行代码。

现在的设计只支持event-driven servers。并且需要针对completion notification model和shared listening socket部分进行对应的更改。

>in a "thread driven" runtimes, when request comes in, a new thread is being created and all the handling is being done in that thread.

>in "event driven" runtimes, when request comes in, the event is dispatched and handler will pick it up. When? In Node.js, there is an "event loop" which basically loops over all the pieces of code that need to be executed and executes them one by one. So the handler will handle the event once event loop invokes it. The important thing here is that all the handlers are being called in the same thread - event loop doesn't have a thread pool to use, it only has one thread.

###实验
首先实验分析了MegaPipe对多核、不同长度消息的可拓展性，而后针对memcached和nginx做了实验分析。

###论文优点
实现了partitioned listening sockets，改善了多核共享listening socket的问题。

实现lwsocket，脱离出VPS，直接指向TCB，避免了和VPS的全局同步。

对System Call进行Batch处理，提高了性能。
###论文缺点
不仅仅需要更改内核，还需要更改应用程序。

现在仅支持event-driven server，对thread-based性能提升有待验证。

###对比mTCP&Fastsocket
mTCP的优势是越过了kernel，还有batched的packet处理（？？？），因此快。

Fastsocket的优势：
更好的API接口兼容性，table-level的partition，还有更好的connection locality。

虽然这篇文章在前，但是和mTCP和FastSocket相比仍然具有的优势是：
lightweight socket虽然导致兼容性问题，但却有性能提升，并且能够tranfer到正常的socket。

###论文评价
在12年实现出这篇文章，并且同时考虑listening socket共享，batch还有lightweight socket
1. Shared resources
2. Broken locality
3. Per packet processing

这三个方面的优化都考虑到了比较难得。
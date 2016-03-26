
#第三次论文阅读
\\Dong 2015210938
\\2016.03.26

##Non-scalable locks are dangerous

###论文主要内容
作者实验证明不可拓展的锁机制在多核应用中严重影响性能，并且理论分析原因后，提过了可拓展的锁机制，并且能够很好的多核支持性。

###论文具体内容介绍
论文首先分析Linux lock机制在48核情况下，**FOPS, MEMPOP, PFIND & EXIM**四个指令的performance，并且还记录了一些和锁相关的测量指标。

而后论文提出自己的分析模型，用于解释分析上述实验结果。模型基于Markov模型，其中考虑c,home directory回应cache line请求的时间，也是这个关键解释了多核collapse的原因。

实验验证模型与真实实验的匹配（没有数据分析）。而后利用模型解释了多核collapse的原因。

而后简单介绍了scalable lock，并列举了几个代表如MCS Lock等，并实验对性能进行了比较。

最后将MCS Lock应用到Linux系统中去，比较了**FOPS, MEMPOP, PFIND & EXIM**四个指令的performance。

###论文实验分析
本篇论文和以往看的论文不一样，开篇就给出了一部分实验。目的是说了说明多核lock问题的重要性。

分析了几个指令在核数增多后performance的变化，强调多核性能collapse的问题严重性。

而后Model部分的实验验证了自己model的正确性。

而后分析了scalable lock的相比unscalable lock的拓展性，并将技术应用到linux系统中分析了性能，印证了文章的主题。

###论文贡献点
指出了unscalable lock的问题，实验证明了重要性。

提出了lock分析模型，有效解释了实验现象。

应用scalable lock技术，改善了问题。

###论文问题和可拓展性
48核是否还是继续拓展。

使用是现有技术，只是做了分析和验证。

而且在若进程的lock占用时间较长问题没有这么明显。使用有效的并行程序设计技术应该可以有效缓解问题。

###论文建议
？？？

##Scalable Kernel TCP Design and Implementation for Short-Lived Connections

###论文内容概括
实现了一个可拓展的TCP Socket设计，实现了table-level partition，并且对passive和active连接都实现了connection locality。
在24核时，有效提高了系统的tcp吞吐量。

###论文内容介绍
首先介绍论文主要面临的三个挑战。
+ 为了解决Global  Listen Table和Global Established Table的共享问题的table-level partition
+ 如何实现完全的connection locality
+ 如何能够使实际好的socket结构，兼容BSD socket api。

###论文实验


###论文贡献点

###论文问题和可拓展性

###论文建议


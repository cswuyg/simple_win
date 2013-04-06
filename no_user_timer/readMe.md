**A Timer without windows message dispatch.**  
**一个不需要消息循环的定时器例子**

####方法：
> 1、CreateTimerQueue(); CreateTimerQueueTimer();  DeleteTimerQueueEx();  
> 2、可等待计时器：CreateWaitableTimer(); SetWaitableTimer(); CloseHandle(); CancelWaitableTimer();   可等待计时器可以多线程、可以本线程异步调用。   比较：  用户计时器（SetTimer）需要在应用程序中使用大量的用户界面基础设施，从而消耗更多的资源。可等待计时器是内核对象，可以在多个线程中共享。

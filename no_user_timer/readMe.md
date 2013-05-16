**A Timer without windows message dispatch.**  
**一个不需要消息循环的定时器例子**

####方法：
- 1、CreateTimerQueue(); CreateTimerQueueTimer();  DeleteTimerQueueEx();  
- 2、可等待计时器：CreateWaitableTimer(); SetWaitableTimer(); CloseHandle(); CancelWaitableTimer();   可等待计时器可以多线程、可以本线程异步调用。   比较：  用户计时器（SetTimer）需要在应用程序中使用大量的用户界面基础设施，从而消耗更多的资源。可等待计时器是内核对象，可以在多个线程中共享。  
- 3、timeSetEvent，多媒体定时器，跟CreateTimeQueue类似，也是新开线程做定时器，多媒体定时器限制只能有16个。这是微软不推荐使用的版本，微软推荐使用CreateTimerQueueTimer()。详见：

http://msdn.microsoft.com/en-us/library/windows/desktop/dd757634(v=vs.85).aspx  
http://msdn.microsoft.com/en-us/library/windows/desktop/dd742877(v=vs.85).aspx



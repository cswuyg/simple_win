**An Example about Multi Medie Timer**  
**一个使用多媒体定时器的例子**  

There are another two examples [no ui thread timer](../hight_performance_time_calc),and [TimerQueue Timer](../timer_queue_timer_test) 。  

多媒体定时器可以实现更高的精度，不会有WM_TIMER优先级比较低的问题。但是如果可以选择，应该优先选择：CreateTimerQueue(); CreateTimerQueueTimer();  DeleteTimerQueueEx(); 这一系列的定时器。另外，需要注意多媒体定时器不能超过16个，之后的会失败。


cswuyg@gmail.com  
2013.5.16 整理






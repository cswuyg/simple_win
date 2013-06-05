There is an example about how to read/write big file.Maybe two choices for windows programer,one is \_wfopen\_s,another is MapViewFile.
MapViewFile is the First Choice,it's faster.




对大文件的处理，可以使用\_wfopen\_s，或者是内存映射文件，但应该优先选择内存映射文件，它更快。

注意：创建一个被写入文件的时候，需要在CreateFileMapping的时候设置文件大小，需要GENERIC\_READ | GENERIC\_WRITE 属性。MapViewOfFile的大小不能超过虚拟内存大小，所以必须分块映射。

cswuyg@gmail.com  
2013.6.5
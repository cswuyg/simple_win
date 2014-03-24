3 point:

1. Use SetUnhandledExceptionFilter to catch exception, and use MiniDumpWriteDump get dump file in process.  
2. Because the CRT function may call the SetUnhandledExceptionFilter before it check the pramater or something else, so our handler will no be call, we must hook the SetUnhandledExceptionFilter.    
3. And we alse should call \_set\_invalid\_parameter\_handler and \_set\_purecall\_handler functions to take care of the CRT error.  
 

  
chromium's breakpad project doesn't hook SetUnhandledExceptionFilter, So someone find it [can not handle all exception](http://stackoverflow.com/questions/11350801/why-does-google-breakpad-not-handle-all-crashes-how-can-i-debug-these-cases), Is it true? why?  


more  
> http://www.cnblogs.com/cswuyg/p/3207576.html  

cswuyg@gmail.com  
2013.7.21
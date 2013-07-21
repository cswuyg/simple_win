Use SetUnhandledExceptionFilter to catch exception, and use MiniDumpWriteDump get dump file in process.  
Because the CRT function may call the SetUnhandledExceptionFilter before it check the pramater or something else, so our handler will no be call, so we must hook the SetUnhandledExceptionFilter.(Call \_set\_invalid\_parameter\_handler and \_set\_purecall\_handler functions is NO enough, we have to use HOOK.)

  
chromium's breakpad project doesn't hook SetUnhandledExceptionFilter, only use \_set\_invalid\_parameter\_handler and \_set\_purecall\_handler.So someone find it [can not handle all exception](http://stackoverflow.com/questions/11350801/why-does-google-breakpad-not-handle-all-crashes-how-can-i-debug-these-cases).  
cswuyg@gmail.com  
2013.7.21
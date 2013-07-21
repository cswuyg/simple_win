Use SetUnhandledExceptionFilter to catch exception, and use MiniDumpWriteDump get dump file in process.  
Because the CRT function may call the SetUnhandledExceptionFilter before it check the pramater or something else, so our handler will no be call, so we must hook the SetUnhandledExceptionFilter.(Call \_set\_invalid\_parameter\_handler function and release the exception in the handler is NO enough, we have to use HOOK.)

cswuyg@gmail.com  
2013.7.21
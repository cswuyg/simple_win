cswuyg@gmail.com  
IDE:vs2010

> ###jsoncpp的使用测试【推荐使用】  
如果不知道json串的数据格式，那么需要使用递归分析。  
jsoncpp主要优点是，快，比现有的其它库都快。  
jsoncpp主要缺点是，不支持unicode，还有，相比SimpleJson，导出接口多，感觉比较复杂。  
工作中，建议使用jsoncpp。simplejson有一堆坑，用起来很辛苦。boost property_tree要带上boost库，太庞大。
> ###SimpleJSON的使用测试
SimpleJson 库插入是非常麻烦的事情。
JSONValue对象有智能指针的功能，会给你析构掉它所包含的JSON对象。而JSON的as....函数，返回的是const类型的引用，如果是array类型，那么是JSONValue\*的浅拷贝。对Parse的返回值实行delete之后，JSONValue又会再delete一次，于是出现多次析构的错误。所以必须保证，要么只有JSONValue对象去执行析构，要么只有主动的delete Parse的返回值。对于插入来说，这种逻辑会带来麻烦。定义了一个JSONValue，浅拷贝了parse返回值的一部分json对象，然后JSONValue析构了浅拷贝的JSONValue\*，先对Parse的返回值则很难做处理，如果delete，则多析构了JSON对象，如果不delete，则Parse内部new的map内存没有被析构。  
解决办法有两种：  
1、解析之后要插入值，不要定义JSONValue对象，而是定义JSONValue引用，又因为我要往JSONValue里插值，所以必须用到const_cast。  
2、递归拷贝出JSONValue*里的字符串格式的JSON对象，然后再Parse之后进入新JSONObj，保证新、旧对象分离。
> ###boost::property_tree的使用测试
boost来自：http://sourceforge.net/projects/boost/files/boost/  
里头有例子代码。
如果要自定义key，则需要实现path\_of类，不然默认key是string类型,boost已经为string实现了path\_of，就在string_path.hpp中。  
如果是直接使用read\_json依据路径读取，可能会有字符编码的问题，
在使用unicode格式json串的时候，如果直接使用路径读取、写入，使用起来挺乱的，所以建议先自己读取到stream，再让read\_json从stream获取json内容，避开字符编码.或者更彻底点，完全使用自己的读写文件函数，json库的函数只跟string打交道,在一个工程中，让json库只跟string打交道，这也是一种比较好的分层、封装。   
boost::property\_tree的好处是，支持多种数据格式，如：xml、json。其中xml使用的是rapidxml，速度比较快。缺点是，boost库太大了，如果我只想用json解析，还是使用jsoncpp比较好。
> ###
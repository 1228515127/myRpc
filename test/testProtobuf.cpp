#include"testProtobuf.pb.h"
#include<iostream>
#include<string>
using namespace rpcpr;//proto文件编译后生成的名称空间，需要使用其中的变量
int main()
{
    LoginRequest req;//根据生成的cc文件定义类对象
    req.set_name("zhangsan");//使用编译生成的成员方法添加成员变量
    req.set_pwd("123456");//使用编译生成的成员方法添加成员变量
    std::string send_str;//用于存储序列化后的信息
    if(req.SerializeToString(&send_str))
    //调用序列化方法来序列化字符串，成功则执行以下代码
    {
        std::cout<<send_str<<std::endl;
    }
    //从send_str反序列化一个login请求对象
    LoginRequest reqB;
    if(reqB.ParseFromString(send_str))//调用成员方法来反序列化字符串
    {
        std::cout<<reqB.name()<<std::endl;//输出反序列化后的信息
        std::cout<<reqB.pwd()<<std::endl;
    }
    return 0;
}

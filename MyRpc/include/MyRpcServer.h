#pragma once
#include"google/protobuf/service.h"
//muduo库的头文件
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
#include<muduo/net/InetAddress.h>
#include<muduo/net/TcpConnection.h>
#include<google/protobuf/descriptor.h>

#include <sys/epoll.h>
#include<vector>
#include"MyThreadPool.h"


// 负责发布和管理rpc服务
class MyRpcServer
{
public:
    ~MyRpcServer();
    //这里是框架提供给外部使用的，可以发布rpc方法的函数接口
    //此处应该使用Service类，而不是指定某个方法
    void NotifyService(google::protobuf::Service *service);

    //启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run1();
private:
    
    muduo::net::EventLoop _eventLoop;
    int _epfd;
    int _listenfd;
    std::vector<int>_clientfds;
    MyThreadPool* _threadPool;
    //service服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service *_service;//保存服务对象
        std::unordered_map<std::string,const google::protobuf::MethodDescriptor*> _methodMap;//保存服务方法

    };

    //存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string,ServiceInfo> _serviceMap;

    void onMessage(int,std::string);
    void sendRpcResponse(int ,google::protobuf::Message* );
};
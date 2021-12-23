#include<iostream>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include"MyRpcServer.h"
#include"MyRpcHeader.pb.h"
#include"ZkClient.h"
#include"MyRpcConfig.h"
#include"MyThreadPool.h"


void setnonblocking (int fd)
{
	int opts;
 
	opts = fcntl (fd, F_GETFL);
	if (opts < 0)
	{
		perror ("fcntl(F_GETFL)\n");
		exit (1);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl (fd, F_SETFL, opts) < 0)
	{
		perror ("fcntl(F_SETFL)\n");
		exit (1);
	}
}

MyRpcServer::~MyRpcServer(){
    for(int i = 0;i<_clientfds.size();i++){
        close(_clientfds[i]);
    }
}

// 添加服务
void MyRpcServer::NotifyService(google::protobuf::Service *service)
{
    std::cout<<"NotifyService"<<std::endl;
    // 该服务的相关信息
    ServiceInfo serviceInfo;
    serviceInfo._service=service;

    // 该服务的方法
    const google::protobuf::ServiceDescriptor *pServiceDesc=service->GetDescriptor();
    int methodCnt=pServiceDesc->method_count(); 
    for(int i=0;i<methodCnt;++i)
    {
        const google::protobuf::MethodDescriptor* pMethodDesc=pServiceDesc->method(i);
        std::string methodName=pMethodDesc->name();
        // 方法表,映射:方法名->方法描述
        serviceInfo._methodMap.insert({methodName,pMethodDesc});
    }

    // 服务表 映射：服务名->该服务的相关信息
    std::string serviceName=pServiceDesc->name();
    _serviceMap.insert({serviceName,serviceInfo});
}

void MyRpcServer::Run1(){
    std::string ip = MyRpcConfig::GetInstance().GetConfig("rpcserverip");
    uint32_t port=stoi(MyRpcConfig::GetInstance().GetConfig("rpcserverport"));
    //把当前rpc节点上要发布的服务全部注册在zk上，让rpc client可以从zk上发现服务
    ZkClient zkCli;
    zkCli.Start();//链接zkserver
    for(auto &sp:_serviceMap)
    {
        // 创建结点
        //  /serviceName
        std::string servicePath ="/"+sp.first;
        zkCli.Create(servicePath.c_str(),nullptr,0);
        for(auto &mp:sp.second._methodMap)
        {
            // 创建子结点
            //  /serviceName/methodName
            std::string methodPath=servicePath+"/"+mp.first;//拼接服务器路径和方法路径
            char methodPathData[128]={0};
            sprintf(methodPathData,"%s:%d",ip.c_str(),port);//向data中写入路径
            // std::cout << inet_addr(ip.c_str()) << ":"<<port<<std::endl;
            zkCli.Create(methodPath.c_str(),methodPathData,strlen(methodPathData),ZOO_EPHEMERAL);
        }
    }
    std::cout<<"线程池初始化开始"<<std::endl;
    _threadPool = new MyThreadPool(4);
    std::cout<<"线程池初始化完成"<<std::endl;
    const int maxEventNum = 20;
    _listenfd=socket(AF_INET,SOCK_STREAM,0);
    setnonblocking(_listenfd);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    std::cout<<"bind开始"<<std::endl;
    bind(_listenfd,(sockaddr*)&addr,sizeof(addr));
    std::cout<<"bind成功"<<std::endl;
    std::cout<<"listen开始"<<std::endl;
    listen(_listenfd,maxEventNum);
    std::cout<<"listen成功"<<std::endl;
    std::cout<<"epollcreat开始"<<std::endl;
    _epfd = epoll_create1(0);
    std::cout<<"epollcreat成功"<<std::endl;
    epoll_event ev,events[maxEventNum];
    ev.data.fd = _listenfd;
    ev.events = EPOLLIN|EPOLLET;
    epoll_ctl(_epfd,EPOLL_CTL_ADD,_listenfd,&ev);
    char buf[1024];
    std::cout<<"epoll循环开始"<<std::endl;
    for(;;){
        int nfds = epoll_wait(_epfd,events,maxEventNum,-1);
        for(int i=0;i<nfds;i++){
            int fd = events[i].data.fd;
            if(fd == _listenfd){
                int clientfd;
                struct sockaddr_in clientaddr;
                socklen_t clientaddrlen = sizeof(clientaddr);
                while((clientfd = accept(_listenfd,(sockaddr*)&clientaddr,&clientaddrlen))>0){
                    std::cout<<"connect success"<<std::endl;
                    setnonblocking(clientfd);
                    _clientfds.push_back(clientfd);
                    ev.data.fd = clientfd;
                    ev.events = EPOLLIN|EPOLLET;
                    epoll_ctl(_epfd,EPOLL_CTL_ADD,clientfd,&ev);
                }
                std::cout<<"connects end"<<std::endl;
            }else if(events[i].events & EPOLLIN){
                // 实际组成：总长度(4字节，未序列化)+头长度（4字节，未序列化）+header（序列化）+args（序列化）
                std::string recvStr;
                memset(buf,0,sizeof(buf));
                int clientfd=events[i].data.fd;
                int size=0;
                size = recv(clientfd,buf,4,0);
                if(size==0){
                    close(clientfd);
                    continue;
                }
                int32_t totalLen=0;
                strncpy((char*)&totalLen,buf,4);
                int cnt=totalLen;
                while(cnt>0){
                    size = recv(clientfd,buf,sizeof(buf),0);
                    std::string s;
                    s.resize(size);
                    for(int j=0;j<size;j++){
                        s[j]=buf[j];
                    }
                    recvStr+=s;
                    cnt-=size;
                }
                auto f = std::bind(&MyRpcServer::onMessage,this,clientfd,recvStr);
                _threadPool->execute(f);
                
            }
        }
    }
}

void MyRpcServer::onMessage(int clientfd,std::string recvBuf){
    // headerSize
    uint32_t headerSize=0;
    // strncpy((char*)&headerSize,recvBuf,4);
    recvBuf.copy((char*)&headerSize,4,0);
    // header
    std::string rpcHeaderStr=recvBuf.substr(4,headerSize);//substr从4开始读读取headerSize个字节的数据
    myRpc::MyRpcHeader rpcHeader;
    std::string serviceName;
    std::string methodName;
    uint32_t argsSize;
    std::cout << "开始反序列化header"<<std::endl;
    // 反序列化header
    if(rpcHeader.ParseFromString(rpcHeaderStr))
    {
        serviceName=rpcHeader.service_name();
        methodName=rpcHeader.method_name();
        argsSize=rpcHeader.args_size();
    }
    else
    {
        std::cout<<"rpcHeaderStr: "<<rpcHeaderStr<<"parse error!"<<std::endl;
        return;
    }

    //通过映射serviceName->service 和 映射methodName->method找到对应服务和方法
    auto it =_serviceMap.find(serviceName);
    if(it==_serviceMap.end())
    {
        std::cout<<serviceName<<"is not exist!"<<std::endl;
        return;
    }
    auto mit=it->second._methodMap.find(methodName);
    if(mit==it->second._methodMap.end())
    {
        std::cout<<serviceName<<":"<<methodName<<"is not exists!"<<std::endl;
    }
    google::protobuf::Service *service=it->second._service;
    const google::protobuf::MethodDescriptor *method=mit->second;

    std::cout << "开始反序列化request"<<std::endl;
    // args反序列化成request
    std::string argsStr=recvBuf.substr(4+headerSize,argsSize);
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(argsStr))
    {
        std::cout<<"request parse error,content:"<<argsStr<<std::endl;
        return;
    }

    // response
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();
    // Closure,回调函数,执行完CallMethod之后执行回调，即SendRpcResponse
    google::protobuf::Closure *done=google::protobuf::NewCallback
    <MyRpcServer,const int,google::protobuf::Message*>
    (this,&MyRpcServer::sendRpcResponse,clientfd,response);
    service->CallMethod(method,nullptr,request,response,done);
}

void MyRpcServer::sendRpcResponse(int clientfd,google::protobuf::Message* response){
    std::cout << "开始序列化response"<<std::endl;
    std::string responseStr;
    if(response->SerializeToString(&responseStr))//response进行序列化
    {
        //序列化成功后，通过网络把rpc方法执行的结果发送回rpc的调用方
        send(clientfd,responseStr.c_str(),responseStr.size(),0);
    }
    else
    {
        std::cout<<"Serialize response error!"<<std::endl;
    }
    std::cout << "发送完成"<<std::endl;
}

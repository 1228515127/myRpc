//网络编程部分
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string>

#include"MyRpcChannel.h"
#include"MyRpcHeader.pb.h"
#include"ZkClient.h"
#include"MyConnectPool.h"


void MyRpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, 
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response, 
                          google::protobuf::Closure* done)
{
    // service和method名字
    std::string methodName = method->name();
    const google::protobuf::ServiceDescriptor* service = method->service();
    std::string serviceName = service->name();

    // 序列化request，得到argsStr,其大小argsSize
    uint32_t argsSize = 0;
    std::string argsStr = "";
    if(request->SerializeToString(&argsStr)){
        argsSize = argsStr.size();
    }else{
        controller->SetFailed("serialize request error!");
        return;
    }

    // 请求头
    myRpc::MyRpcHeader rpcHeader;
    rpcHeader.set_service_name(serviceName);
    rpcHeader.set_method_name(methodName);
    rpcHeader.set_args_size(argsSize);
    
    // 请求头序列化,得到headerStr,及其大小headerSize
    uint32_t headerSize = 0;
    std::string headerStr = "";
    if(rpcHeader.SerializeToString(&headerStr)){
        headerSize = headerStr.size();
    }else{
        controller->SetFailed("serialize rpcHeader error!");
        return;
    }

    // 真正发送的字符串
    // 实际组成：总长度(4字节，未序列化)+头长度（4字节，未序列化）+header（序列化）+args（序列化）
    std::string sendStr = "";
    // 前4个字节为请求头长度，方便server区分请求头和请求体
    sendStr.insert(0,std::string((char *)&headerSize,4));
    sendStr+=headerStr+argsStr;
    int32_t packetLen=sendStr.size();
    sendStr.insert(0,std::string((char *)&packetLen,4));
    
    
    conn _conn = MyRpcConnectPool::GetInstance()->GetFreeConn(serviceName,methodName);
    int clientfd = _conn._fd;
    //发送rpc请求
    
    if(-1==send(clientfd,sendStr.c_str(),sendStr.size(),0))
    {
        MyRpcConnectPool::GetInstance()->PutConn(serviceName,methodName,_conn);
        controller->SetFailed("send socket error!");
        return;
    }
    // std::cout<<"发送成功"<<std::endl;
    //接受rpc请求的响应值
    char recvBuf[1024]={0};
    int recvSize=0;
    
    if(-1==(recvSize=recv(clientfd,recvBuf,1024,0)))
    {
        MyRpcConnectPool::GetInstance()->PutConn(serviceName,methodName,_conn);
        controller->SetFailed("recv socket error!");
        return;
    }
    // std::cout<<"接受成功"<<std::endl;
    //反序列化rpc调用的响应数据
    std::string response_str(recvBuf,0,recvSize);
    //if(!response->ParseFromString(response_str))
    if(!response->ParsePartialFromArray(recvBuf,recvSize))
    {
        MyRpcConnectPool::GetInstance()->PutConn(serviceName,methodName,_conn);
        controller->SetFailed("Parse response error! errno");
        return;
    }
    MyRpcConnectPool::GetInstance()->PutConn(serviceName,methodName,_conn);
}

#include<iostream>
#include<string>
#include"MyRpcServer.h"
#include"MyRpcConfig.h"
#include"MyMathService.pb.h"

// 1.重新虚函数，实现业务逻辑
class MyMathService:public MyMath::MyMathService
{
public:
    int32_t Add(int32_t num1, int32_t num2)
    {
        return num1+num2;
    }

    int32_t Sub(int32_t num1, int32_t num2)
    {
        return num1-num2;
    }

    void Add(::google::protobuf::RpcController* controller,
                       const ::MyMath::AddRequest* request,
                       ::MyMath::AddResponse* response,
                       ::google::protobuf::Closure* done)
    {
        int32_t num1 = request->num1();
        int32_t num2 = request->num2();
        
        //开始做本地业务
        int32_t ret = Add(num1,num2);

        //填充回调结果
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_ret(ret);

        done->Run();
    }
    void Sub(::google::protobuf::RpcController* controller,
                       const ::MyMath::AddRequest* request,
                       ::MyMath::AddResponse* response,
                       ::google::protobuf::Closure* done)
    {
        int32_t num1 = request->num1();
        int32_t num2 = request->num2();
        
        //开始做本地业务
        int32_t ret = Sub(num1,num2);

        //填充回调结果
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_ret(ret);

        done->Run();
    }
};

int main(){
    // 使用cmake时 使用相对路径出错，所以使用绝对路径  （未解决）
    // 1.config
    MyRpcConfig::GetInstance().Init("/home/lsd/myRpc/example/server/config/server.config");
    // 2.server
    MyRpcServer server;
    // 3.发布服务
    server.NotifyService(new MyMathService());
    // 4.运行
    server.Run1();
}
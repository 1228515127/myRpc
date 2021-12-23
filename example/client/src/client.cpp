#include<iostream>
#include<string>
#include<unistd.h>
#include"MyMathService.pb.h"
#include"MyRpcChannel.h"
#include"MyRpcController.h"
#include"MyRpcConfig.h"
#include"MyConnectPool.h"
#include"MyThreadPool.h"
int main(){
    // 使用cmake时 使用相对路径出错，所以使用绝对路径  （未解决）
    // 1.config
    std::cout<<"start"<<std::endl;
    MyRpcConfig::GetInstance().Init("/home/lsd/myRpc/example/client/config/client.config");
    // 2.stub,并传入channel
    MyMath::MyMathService_Stub stub(new MyRpcChannel());
    MyRpcConnectPool::GetInstance()->Init();
    for(int i=0;i<stub.GetDescriptor()->method_count();i++){
        MyRpcConnectPool::GetInstance()->AddService(3,stub.GetDescriptor()->name(),stub.GetDescriptor()->method(i)->name());
    }
    // 3.req,resp
    
    MyMath::AddRequest addReq[100];
    MyMath::AddResponse addResp[100];
    
    MyThreadPool* pool = new MyThreadPool(2);
    for(int i = 0;i < 100;i++){
        addReq[i].set_num1(i);
        addReq[i].set_num2(i);
        auto f = std::bind(&MyMath::MyMathService_Stub::Add,&stub,nullptr,&addReq[i],&addResp[i],nullptr);
        pool->execute(f);
    }
    sleep(10);
    // 4.调用，stub.Add(),  实际调用channel->callmethod
    // stub.Add(new MyRpcController(),&addReq,&addResp,nullptr);
    // int ret = addResp.ret();
    // std::cout<<ret<<std::endl;
    MyRpcConnectPool::GetInstance()->DeleteIns();
}

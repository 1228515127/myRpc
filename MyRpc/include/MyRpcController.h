#pragma once

#include<google/protobuf/service.h>
#include<string>

class MyRpcController:public google::protobuf::RpcController
{
public:
    MyRpcController();
    void Reset();
    bool Failed()const;//用以记录失败与否
    std::string ErrorText()const;//用于记录错误信息
    void SetFailed(const std::string& reason);

    //目前未实现具体的功能
    void StartCancel();
    bool IsCanceled()const;
    void NotifyOnCancel(google::protobuf::Closure *callback);
private:
    bool _failed;//RPC方法执行过程中的状态
    std::string _errtext;//RPC方法执行过程中的错误信息
};
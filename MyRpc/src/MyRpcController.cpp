#include"MyRpcController.h"

//类的构造函数
MyRpcController::MyRpcController()
{
    _failed=false;
    _errtext="";
}

void MyRpcController::Reset()
{
    _failed=false;
    _errtext="";
}

bool MyRpcController::Failed()const
{
    return _failed;
}

std::string MyRpcController::ErrorText()const
{
    return _errtext;
}

void MyRpcController::SetFailed(const std::string& reason)
{
    _failed=true;
    _errtext=reason;
}

void MyRpcController::StartCancel(){}
bool MyRpcController::IsCanceled()const{return false;}
void MyRpcController::NotifyOnCancel(google::protobuf::Closure *callback){}
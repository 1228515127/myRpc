#include<iostream>
#include<string>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<functional>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <arpa/inet.h>

#include"ZkClient.h"
#include"MyConnectPool.h"


MyConnectPool::MyConnectPool(int size,
                            std::string serviceName,
                            std::string methodName,
                            ZkClient& zkCli)
                            :_size(size),
                            _serviceName(serviceName),
                            _methodName(methodName),
                            _zkCli(zkCli){
    getAddr();
    conn _conn;
    for(int i=0;i<size;i++){
        int clientfd=socket(AF_INET,SOCK_STREAM,0);
        if(clientfd==-1){
            std::cout<<"creat socket error"<<std::endl;
        }
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port=_port;
        addr.sin_addr.s_addr=_ip;
        // std::cout<<_ip<<":"<<_port<<std::endl;
        if(-1==connect(clientfd,(sockaddr*)&addr,sizeof(addr))){
            std::cout<<"connect error"<<std::endl;
        }
        _conn._fd=clientfd;
        _conn._addr=addr;
        _connects.push(_conn);
    }
}

MyConnectPool::~MyConnectPool(){
    std::unique_lock<std::mutex> lk(_mtx);
    while(_connects.size()<_size){
        cond.wait(lk);
    }
    while(!_connects.empty()){
        close(_connects.front()._fd);
        _connects.pop();
    }
}

void MyConnectPool::updateConnect(conn& _conn){
    getAddr();
    if(_conn._addr.sin_port!=_port || _conn._addr.sin_addr.s_addr!=_ip){
        close(_conn._fd);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port=_port;
        addr.sin_addr.s_addr=_ip;
        _conn._addr=addr;
        connect(_conn._fd,(sockaddr*)&addr,sizeof(addr));
    }   
}

conn MyConnectPool::GetFreeConnect(){
    int clientfd=-1;
    std::unique_lock<std::mutex> lk(_mtx);
    while(_connects.empty()){
        cond.wait(lk);
    }
    conn _conn = _connects.front();
    _connects.pop();
    lk.unlock();
    updateConnect(_conn);
    return _conn;
}

void MyConnectPool::PutConnect(conn _conn){
    std::unique_lock<std::mutex> lk(_mtx);
    _connects.push(_conn);
    lk.unlock();
    cond.notify_one();
}

void MyConnectPool::getAddr(){
    std::string methodPath="/"+_serviceName+"/"+_methodName;
    //获取ip地址和端口号
    std::string addr=_zkCli.GetData(methodPath.c_str());
    int idx=addr.find(":");//分割符
    _ip = inet_addr(addr.substr(0,idx).c_str());
    _port=stoi(addr.substr(idx+1,addr.size()-idx));
}

MyRpcConnectPool::MyRpcConnectPool(){}

MyRpcConnectPool::~MyRpcConnectPool(){}

void MyRpcConnectPool::Init(){
    _zkCli.Start();
}

MyRpcConnectPool* MyRpcConnectPool::GetInstance(){
    static MyRpcConnectPool ins;
    return &ins;
}

void MyRpcConnectPool::DeleteIns(){
    for(int i=0;i<_pool.size();i++){
        delete _pool[i];
    }
}

void MyRpcConnectPool::AddService(int size,std::string serviceName,std::string methodName){
    // std::cout << serviceName<<methodName<<std::endl;
    std::string name = serviceName+"_"+methodName;
    int idx = _pool.size();
    _map.insert({name,idx});
    MyConnectPool* pool = new MyConnectPool(size,serviceName,methodName,_zkCli);
    _pool.push_back(pool);
}

conn MyRpcConnectPool::GetFreeConn(std::string serviceName,std::string methodName){
    // std::cout << serviceName<<methodName<<std::endl;
    std::string name = serviceName+"_"+methodName;
    if(_map.find(name)==_map.end()){
        std::cout<<"get not fount"<<std::endl;
        conn _conn;
        return _conn;
    }
    int idx=_map[name];
    MyConnectPool* p = _pool[idx];
    return p->GetFreeConnect();
}

void MyRpcConnectPool::PutConn(std::string serviceName,std::string methodName,conn _conn){
    std::string name = serviceName+"_"+methodName;
    if(_map.find(name)==_map.end()){
        std::cout<<"put not fount"<<std::endl;
        close(_conn._fd);
        return;
    }
    int idx=_map[name];
    MyConnectPool* p = _pool[idx];
    return p->PutConnect(_conn);
}
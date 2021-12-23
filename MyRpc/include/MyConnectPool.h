#pragma once
#include<string>
#include<mutex>
#include<queue>
#include<condition_variable>
#include<netinet/in.h>
#include <unordered_map>
#include"ZkClient.h"


typedef struct {
    int _fd;
    sockaddr_in _addr;
}conn; 

class MyConnectPool{
public:
    MyConnectPool(int size,std::string serviceName,std::string methodName,ZkClient& zkCli);
    ~MyConnectPool();
    // 获得一个连接
    conn GetFreeConnect();
    // 将连接放入连接池
    void PutConnect(conn _conn);
private:
    // 从zookeeper获取ip和port
    void getAddr();
    // 更新连接
    void updateConnect(conn& _conn);
    
    int _size;
    std::queue<conn> _connects;
    std::mutex _mtx;
    std::condition_variable cond;
    std::string _serviceName;
    std::string _methodName;
    ZkClient& _zkCli;
    uint32_t _ip;
    uint32_t _port;
};

class MyRpcConnectPool{
public:
    static MyRpcConnectPool* GetInstance();
    void Init();
    void DeleteIns();
    void AddService(int size,std::string serviceName,std::string methodName);
    conn GetFreeConn(std::string serviceName,std::string methodName);
    void PutConn(std::string serviceName,std::string methodName,conn _conn);
private:
    MyRpcConnectPool();
    ~MyRpcConnectPool();
    MyRpcConnectPool(const MyRpcConnectPool&);
    MyRpcConnectPool& operator=(const MyRpcConnectPool&);
    std::unordered_map<std::string,int>_map;
    std::vector<MyConnectPool*>_pool;
    ZkClient _zkCli;
};
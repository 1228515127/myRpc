#pragma once
#include<semaphore.h>
#include<zookeeper/zookeeper.h>
#include<string>
#include<unordered_map>

// 启动服务器  ./zkServer.sh start
class ZkClient{
public:
    ZkClient();
    ~ZkClient();
    //zkclinet启动链接zkserver
    void Start();
    //在zkserver上根据指定的path创建znode节点
    void Create(const char *path,const char *data,int datalen,int state=0);
    //传入参数指定的znode节点路径，或者znode节点的值
    std::string GetData(const char *path);
private:
    //zk的客户端句柄
    void update();
    zhandle_t *m_zhandle;
    std::unordered_map<std::string,std::string>_cache;
};
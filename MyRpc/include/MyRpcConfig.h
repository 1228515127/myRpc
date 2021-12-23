#pragma once
//配置文件为键值对，且不需要排序，所以使用无序哈希
#include<unordered_map>
#include<string>

//框架读取配置文件类
class MyRpcConfig
{
public:
    //查询配置项信息
    static MyRpcConfig& GetInstance();
    void Init(const char* file);
    std::string GetConfig(std::string const &key);
    void DeleteIns();
private:
    MyRpcConfig();
    ~MyRpcConfig();
    MyRpcConfig(const MyRpcConfig&);
    MyRpcConfig& operator=(const MyRpcConfig&);

    std::unordered_map<std::string,std::string> _configMap;
    
};

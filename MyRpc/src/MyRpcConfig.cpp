#include "MyRpcConfig.h"
#include <iostream>
#include <fstream>
#include <string>
MyRpcConfig::MyRpcConfig(){}
MyRpcConfig::~MyRpcConfig(){}
void MyRpcConfig::DeleteIns(){}
MyRpcConfig& MyRpcConfig::GetInstance(){
    static MyRpcConfig ins;
    return ins;
}

void MyRpcConfig::Init(const char* fileName){
    std::ifstream f(fileName);
    if(!f.is_open()){
        std::cout<<"file open fail"<<std::endl;
        return;
    }
    std::string temp;
    while(getline(f,temp)){
        // 删除注释
        int idx=temp.size();
        for(int i = 0;i < temp.size(); i++){
            if(temp[i]=='#'){
                idx=i;
                break;
            }
        }
        temp=temp.substr(0,idx);
        for(int i=0;i<temp.size();i++){
            if(temp[i]=='='){
                _configMap[temp.substr(0,i)]=temp.substr(i+1,temp.size()-i-1);
                break;
            }
        }
    }
    f.close();
}

std::string MyRpcConfig::GetConfig(std::string const &key){
    return _configMap[key];
}
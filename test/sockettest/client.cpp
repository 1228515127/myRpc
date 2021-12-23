#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<iostream>
#include<string>


int main(){
    int clientfd[3];
    for(int i=0;i<3;i++){
        clientfd[i]=socket(AF_INET,SOCK_STREAM,0);
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = 8000;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    for(int i=0;i<3;i++){
        if(-1==connect(clientfd[i],(sockaddr*)&addr,sizeof(addr))){
            std::cout<<"connect error"<<std::endl;
        }
    }
    std::string data = "client send data";
    send(clientfd[0],data.c_str(),sizeof(data),0);
    std::cout << "client send" << data << std::endl;
    char buf[1024];
    recv(clientfd[0],buf,1024,0);
    std::cout << "client recv" << buf << std::endl;
    sleep(1000);
    for(int i=0;i<3;i++){
        close(clientfd[i]);
    }
    
}
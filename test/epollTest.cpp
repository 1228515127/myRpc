#include <sys/epoll.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<iostream>
#include<string>
#include<memory.h>
int main(){
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = 8000;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd,(sockaddr*)&addr,sizeof(addr));
    listen(listenfd,1024);
    const int maxEventNum = 20;
    int epfd = epoll_create1(0); 
    epoll_event ev,events[maxEventNum];
    ev.data.fd = listenfd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    for(;;){
        int nfds = epoll_wait(epfd,events,maxEventNum,-1);
        for(int i=0;i<nfds;i++){
            int fd = events[i].data.fd;
            if(fd == listenfd){
                struct sockaddr_in clientaddr;
                socklen_t clientaddrlen = sizeof(clientaddr);
                int clientfd = accept(listenfd,(sockaddr*)&clientaddr,&clientaddrlen);
                ev.data.fd = clientfd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&ev);
            }else if(events[i].events & EPOLLIN){
                char buf[1024];
                memset(buf,0,sizeof(buf));
                recv(events[i].data.fd,buf,sizeof(buf),0);
                std::cout<<std::endl;
                std::cout<<buf<<std::endl;
                send(events[i].data.fd,buf,sizeof(buf),0);
                // close(events[i].data.fd);
            }
        }
    }
}
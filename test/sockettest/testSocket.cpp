#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#include<iostream>
#include<string>
#include<mutex>
#include<memory>
#include<condition_variable>
#include<thread>
#include<queue>
#include<functional>
class fixed_thread_pool {
public:
    explicit fixed_thread_pool(size_t thread_count)
        : data_(std::make_shared<data>()) {
        for (size_t i = 0; i < thread_count; ++i) {
            std::thread([data = data_] {
                std::unique_lock<std::mutex> lk(data->mtx_);
                for (;;) {
                    if (!data->tasks_.empty()) {
                        auto current = std::move(data->tasks_.front());
                        data->tasks_.pop();
                        lk.unlock();
                        current();
                        lk.lock();
                    }
                    else if (data->is_shutdown_) {
                        break;
                    }
                    else {
                        data->cond_.wait(lk);
                    }
                }
                }).detach();
        }
    }

    fixed_thread_pool() = default;
    fixed_thread_pool(fixed_thread_pool&&) = default;

    ~fixed_thread_pool() {
        if ((bool)data_) {
            {
                std::lock_guard<std::mutex> lk(data_->mtx_);
                data_->is_shutdown_ = true;
            }
            data_->cond_.notify_all();
        }
    }

    template <class F>
    void execute(F&& task) {
        {
            std::lock_guard<std::mutex> lk(data_->mtx_);
            data_->tasks_.emplace(std::forward<F>(task));
        }
        data_->cond_.notify_one();
    }

private:
    struct data {
        std::mutex mtx_;
        std::condition_variable cond_;
        bool is_shutdown_ = false;
        std::queue<std::function<void()>> tasks_;
    };
    std::shared_ptr<data> data_;
};

void OnConnect(int clientfd){
    char buf[1024];
    int n = recv(clientfd,buf,sizeof(buf),0);
    std::cout<<"server recv:"<<buf<<std::endl;
    std::string data = "recive data:"+std::string(buf);
    send(clientfd,data.c_str(),sizeof(data),0);
    close(clientfd);
}

int main(){
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = 8000;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd,(sockaddr*)&addr,sizeof(addr));
    listen(listenfd,5);
    fixed_thread_pool pool(5);
    while(1){
        struct sockaddr_in clientaddr;
        int clientaddrlen;
        int clientfd = accept(listenfd,(sockaddr*)&clientaddr,(socklen_t*)&clientaddrlen);
        auto f = std::bind(OnConnect,clientfd);
        pool.execute(f);
    }
    close(listenfd);

}
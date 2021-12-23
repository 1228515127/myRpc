#pragma once
#include <thread>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <functional>
#include<thread>
#include<iostream>

class MyThreadPool {
public:
    MyThreadPool(size_t size)
        : _data(std::make_shared<data>()) {
        for (size_t i = 0; i < size; ++i) {
            std::thread([data = _data] {
                std::unique_lock<std::mutex> lk(data->_mtx);
                for (;;) {
                    if (!data->_tasks.empty()) {
                        auto current = std::move(data->_tasks.front());
                        data->_tasks.pop();
                        lk.unlock();
                        current();
                        lk.lock();
                    }
                    else if (data->_isShutdown) {
                        break;
                    }
                    else {
                        data->_cond.wait(lk);
                    }
                }
                }).detach();
        }
    }

    ~MyThreadPool() {
        if ((bool)_data) {
            {
                std::lock_guard<std::mutex> lk(_data->_mtx);
                _data->_isShutdown = true;
            }
            _data->_cond.notify_all();
        }
    }

    template <class F>
    void execute(F&& task) {
        {
            std::lock_guard<std::mutex> lk(_data->_mtx);
            _data->_tasks.emplace(std::forward<F>(task));
        }
        _data->_cond.notify_one();
    }

private:
    struct data {
        std::mutex _mtx;
        std::condition_variable _cond;
        bool _isShutdown = false;
        std::queue<std::function<void()>> _tasks;
    };
    std::shared_ptr<data> _data;
};

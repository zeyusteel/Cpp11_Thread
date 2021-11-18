#include "ThreadPool.h"
#include <iostream>

namespace ThreadPool {


CThreadPool::CThreadPool(unsigned int uMaxThreadSize, unsigned int uMaxTaskSize) : m_queue{uMaxTaskSize}
{
    m_bIsRunning = true;
    for(unsigned int i = 0; i < uMaxThreadSize; i++){
        m_listThread.emplace_back(std::make_shared<std::thread>(&CThreadPool::run_task, this));
    }
}

CThreadPool::~CThreadPool()
{
    stop_task();
}

void CThreadPool::add_Task(Task&& task)
{
    m_queue.push_queue(std::forward<Task>(task));
}

void CThreadPool::run_task()
{
    while(m_bIsRunning){
        Task task{[]{}};
        m_queue.pop_queue(task);
        task();
    }
}

void CThreadPool::stop_task()
{
    m_queue.clear_queue();
    m_bIsRunning = false;

    for(unsigned int i = 0; i < m_listThread.size(); ++i){
        add_Task([]{});
    }

    for(auto &thread : m_listThread){
        if(thread && thread->joinable()){
            thread->join();
        }
    }
    m_listThread.clear();
}

}

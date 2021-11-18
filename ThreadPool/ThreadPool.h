#ifndef CTHREADPOOL_H
#define CTHREADPOOL_H
#include "SyncQueue.h"

#include <thread>
#include <atomic>
#include <functional>
#include <list>
#include <memory>
#include <future>

#define MAX_TASK_SIZE (100)

namespace ThreadPool {

class CThreadPool
{
    using Task = std::function<void()>;
public:
    CThreadPool(unsigned int uMaxThreadSize = std::thread::hardware_concurrency(), unsigned int uMaxTaskSize = MAX_TASK_SIZE);
    ~CThreadPool();

    void add_Task(Task&& task);

    template <typename F, typename... Args>
    auto add_Task(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);//绑定函数
        auto pTask = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);//封装成包
        // 利用闭包捕获task 转成void function
        Task voidFunc = [pTask]()
        {
            (*pTask)();
        };
        m_queue.push_queue(voidFunc);

        return pTask->get_future();
    }

private:
    void run_task();
    void stop_task();

private:
    std::list<std::shared_ptr<std::thread>> m_listThread;
    std::atomic_bool m_bIsRunning;
    SyncQueue<Task> m_queue;
};

}

#endif // CTHREADPOOL_H

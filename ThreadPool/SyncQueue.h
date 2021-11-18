#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
namespace ThreadPool{

template <typename T>
class SyncQueue {

public:
    SyncQueue(unsigned int iMax): m_iMaxSize(iMax) {}

    void push_queue(T&& task) {
        std::unique_lock<std::mutex> lock{m_mutex};
        m_condNotFull.wait(lock, [this]{ return m_queue.size() < m_iMaxSize; });
        m_queue.push(std::forward<T>(task));
        m_condNotEmpty.notify_one();
    }

    void push_queue(const T& task) {
        std::unique_lock<std::mutex> lock{m_mutex};
        m_condNotFull.wait(lock, [this]{ return m_queue.size() < m_iMaxSize; });
        m_queue.push(task);
        m_condNotEmpty.notify_one();
    }

    void pop_queue(T& task) {
        std::unique_lock<std::mutex> lock{m_mutex};
        m_condNotEmpty.wait(lock, [this]{ return !m_queue.empty(); });
        task = m_queue.front();
        m_queue.pop();
        m_condNotFull.notify_one();
    }

    void pop_queue(std::queue<T>& tasks) {
        std::unique_lock<std::mutex> lock{m_iMaxSize};
        m_condNotEmpty.wait(lock, [this]{ return !m_queue.empty(); });
        tasks = std::move(m_queue);
        m_condNotFull.notify_one();
    }

    bool is_empty() {
        std::lock_guard<std::mutex> lock{m_mutex};
        return m_queue.empty();
    }

    bool is_full() {
        std::lock_guard<std::mutex> lock{m_mutex};
        return m_iMaxSize == m_queue.size();
    }

    int get_size() {
        std::lock_guard<std::mutex> lock{m_mutex};
        return m_queue.size();
    }

    void clear_queue() {
        std::lock_guard<std::mutex> lock{m_mutex};
        std::queue<T> tmp;
        m_queue.swap(tmp);
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_condNotFull;
    std::condition_variable m_condNotEmpty;
    std::queue<T> m_queue;
    unsigned int m_iMaxSize;
};

}
#endif // SYNCQUEUE_H

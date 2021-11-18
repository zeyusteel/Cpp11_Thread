#include "ThreadPool.h"

#include <thread>
#include <iostream>
#include <chrono>
#include <memory>
#include <random>
#include <type_traits>
std::random_device rd; // 真实随机数产生器

std::mt19937 mt(rd());//生成计算随机数mt
std::uniform_int_distribution<int> dist(-1000, 1000); //生成-1000到1000之间的离散均匀分布数
auto rnd = std::bind(dist, mt);

// 设置线程睡眠时间
void simulate_hard_computation()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000 + rnd()));
}
// 添加两个数字的简单函数并打印结果
void multiply(const int a, const int b)
{
    simulate_hard_computation();
    const int res = a * b;
    std::cout << a << " * " << b << " = " << res << std::endl;
}
// 添加并输出结果
void multiply_output(int &out, const int a, const int b)
{
    simulate_hard_computation();
    out = a * b;
    std::cout << a << " * " << b << " = " << out << std::endl;
}
// 结果返回
int multiply_return(const int a, const int b)
{
    simulate_hard_computation();
    const int res = a * b;
    std::cout << a << " * " << b << " = " << res << std::endl;
    return res;
}

void task1() {
    std::cout << "task1 start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "task1 finish" << std::endl;
}

void task2() {
    std::cout << "task2 start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "task2 finish" << std::endl;
}

void task3() {
    std::cout << "task3 start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(8));
    std::cout << "task3 finish" << std::endl;
}

int main(int argc, char *argv[])
{
    ThreadPool::CThreadPool pool;
    pool.add_Task(task1);
    pool.add_Task(task2);
    pool.add_Task(task3);
 
    pool.add_Task(multiply, 3, 3);

    int output = 0;
    auto future = pool.add_Task(multiply_output, std::ref(output), 4, 4);
    future.get();
    std::cout << "ref out:" << output << std::endl;

    auto future1 = pool.add_Task(multiply_return, 5, 5);
    std::cout << "future return:" << future1.get() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}

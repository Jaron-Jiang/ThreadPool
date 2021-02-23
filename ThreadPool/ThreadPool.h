#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
#include<condition_variable>
#include"PriorityQueue.h"

/// <summary>
/// 执行的任务返回类型必须是void类型，任务必须有一个参数，不管使不使用这个参数
/// 即任务声明必须为【void funtion_name(task_paramete_type parameter_name)】
/// 同时task_parameter_type结构体必须对<运行符进行重载，用于比较任务的优先级
/// 任务必须是普通函数，不能是成员函数
/// </summary>
/// <typeparam name="task_parameter_type">task_parameter_type</typeparam>
template<typename task_parameter_type>
class ThreadPool
{
    struct Task
    {
        void (*f)(task_parameter_type);
        task_parameter_type _parameter;
        bool operator<(const task_parameter_type& _value)
        {
            return _parameter < _value;
        }
    };
private:
    bool ForcedShutdown;
    int MaxThread;
    int MinThread;
    int NowThread;
    std::vector<std::thread*>thread_list;
    std::mutex lock_task_list;
    std::mutex lock_thread;
    PriorityQueue<Task*>task_list;
    void runTask()
    {
        while (true)
        {
            lock_thread.lock();
            while (true)
            {
                lock_task_list.lock();
                if (!task_list.empty())
                {
                    lock_task_list.unlock();
                    break;
                }
                lock_task_list.unlock();
                if (NowThread > MinThread)
                {
                    deleteThread(std::this_thread::get_id());
                    lock_thread.unlock();
                    return;
                }
            }
            lock_task_list.lock();
            if (task_list.size() >= NowThread * 2 && NowThread < MaxThread)
            {
                creatThread();
            }
            Task* task = task_list.top();
            task_list.pop();
            lock_task_list.unlock();
            lock_thread.unlock();
            task->f(task->_parameter);
            delete task;
        }
    }
    void creatThread()
    {
        std::thread* t1 = new std::thread(&ThreadPool::runTask, this);
        thread_list.push_back(t1);
        NowThread++;
    }
    void deleteThread(std::thread::id delete_thread_id)
    {
        for (size_t i = 0; i < thread_list.size(); i++)
        {
            if (thread_list[i]->get_id() == delete_thread_id)
            {
                thread_list.erase(thread_list.begin() + i);
                NowThread--;
                break;
            }
        }
    }

public:
    ThreadPool(int MinThreadNum = 3, int MaxThreadNum = 5)
    {
        ForcedShutdown = false;
        NowThread = 0;
        MinThread = MinThreadNum;
        MaxThread = MaxThreadNum;
        while (NowThread < MinThread)
        {
            creatThread();
        }
    }
    ~ThreadPool()
    {
        if (!ForcedShutdown)
        {
            MinThread = 0;
            while (NowThread)
            {
                /**
                *
                * Wait for the thread to be destroyed automatically by setting MinThread equal to zero.
                * All tasks in the task queue are executed at the same time.
                *
                */
            }
        }
    }
    bool getForcedShutdown()
    {
        return ForcedShutdown;
    }
    void setForcedShutdown(bool _value)
    {
        ForcedShutdown = _value;
    }
    int getMaxThread()
    {
        return MaxThread;
    }
    int getMinThread()
    {
        return MinThread;
    }
    void setMaxThread(int num)
    {
        MaxThread = num;
    }
    void setMinThread(int num)
    {
        MinThread = num;
        while (NowThread < MinThread)
        {
            creatThread();
        }
    }
    void addTask(void (*func)(task_parameter_type),task_parameter_type& _parameter)
    {
        Task* task = new Task;
        task->f = func;
        task->_parameter = _parameter;
        lock_task_list.lock();
        task_list.push(task);
        lock_task_list.unlock();
    }
};

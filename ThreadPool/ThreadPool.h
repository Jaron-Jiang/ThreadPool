#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
#include<condition_variable>
#include"PriorityQueue.h"

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

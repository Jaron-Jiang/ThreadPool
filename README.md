### 成果
1. 线程池的基本功能已经实现
2. 通过设置任务的优先级，调整任务执行的顺序(怎么设置后面介绍)
3. 能够执行普通函数这类任务

### 不足
1. 无法执行成员函数这类任务
2. 通过轮询的方法不断查看任务队列，有损性能

### 文件介绍
1. main.cpp
>项目入口，不多介绍
</br>
2. PriorityQueue.h
>自己实现的一个任务优先队列，保证任务能够根据优先级进行添加(当然STL中有写好的相关容器，这里主要是想自己实现一遍)
</br>
3. ThreadPool.h
>实现的线程池

### 使用说明
>1. ThreadPool类是一个模板类，ThreadPool类的模板形参只有一个，这个形参表示的是执行任务的参数类型
>2. ThreadPool使用addTask()函数进行添加任务，addTask有两个参数，第一个参数是要添加的任务即函数，第二个函数的参数
>2.1 任务的声明必须为void funtion_name(parameter_type parameter)
>2.2 任务的参数必须保证<运算符能够使用，也就是说如果参数类型是结构体或者类必须重载<运算符，任务队列会通过<运算符进行优先级判断。然后添加任务    

### 演示
```c++
#include<iostream>
#include"ThreadPool.h"

struct Parameter
{
    int Priority;
    bool operator<(const Parameter& _value)
    {
        return Priority < _value.Priority;
    }
};
void funtion(Parameter parameter)
{
    std::cout<<std::this_thread::get_id()<<std::endl;
}
int main()
{
    ThreadPool<Parameter>threadPools;
    Parameter parameter;
    srand((unsigned)time(NULL));
    for(int i = 0;i < 100;i++)
    {
        parameter.Priority = rand() % 5;
        threadPools.addTask(funtion,parameter);
    }
    return 0;
}
```
</br>

### 改进的方向
1. 实现成员函数这类任务的执行
2. 采用condition_variable进行锁，减少轮询，提高性能
//#include<iostream>
//#include<mutex>
//#include"ThreadPool.h"
//struct test
//{
//	bool operator<(const test& a)
//	{
//		return true;
//	}
//};
//void fun(test a)
//{
//	std::cout << std::this_thread::get_id() << std::endl;
//}
//class person
//{
//private:
//	static person* p;
//	person() {};
//public:
//	static person* func()
//	{
//		if (p == NULL)
//		{
//			p = new person;
//		}
//		return p;
//	}
//	static int a;
//};
//person* person::p = NULL; //Private variable initialization
//int main()
//{
//	ThreadPool<test>pool;
//	test a;
//	for (int i = 0; i < 100; i++)
//	{
//		pool.addTask(fun, a);
//	}
//	std::cout << "任务添加完毕" << std::endl;
//	return 0;
//}
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
    std::cout << std::this_thread::get_id() << std::endl;
}
int main()
{
    ThreadPool<Parameter>threadPools;
    Parameter parameter;
    srand((unsigned)time(NULL));
    for (int i = 0; i < 100; i++)
    {
        parameter.Priority = rand() % 5;
        threadPools.addTask(funtion, parameter);
    }
    return 0;
}
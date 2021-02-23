#include<iostream>
#include<mutex>
#include"ThreadPool.h"
struct test
{
	bool operator<(const test& a)
	{
		return true;
	}
};
void fun(test a)
{
	std::cout << std::this_thread::get_id() << std::endl;
}
class person
{
private:
	static person* p;
	person() {};
public:
	static person* func()
	{
		if (p == NULL)
		{
			p = new person;
		}
		return p;
	}
	static int a;
};
person* person::p = NULL; //Private variable initialization
int main()
{
	ThreadPool<test>pool;
	test a;
	for (int i = 0; i < 100; i++)
	{
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 2 == 0 ? 0 : 1));
		pool.addTask(fun, a);
	}
	std::cout << "任务添加完毕" << std::endl;
	return 0;
}
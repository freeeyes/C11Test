#include <iostream>
#include <chrono>
#include <vector>
#include <ctime>
#include <iomanip>
#include <fstream>

#include "tms.hpp"
#include "LoadLibrary.hpp"
#include "singleton.h"
#include <gtest/gtest.h>

using namespace std;

//g++ -std=c++11 -Wunused-function -o test test_task.cpp -pthread -ldl

using time_point_tt = std::chrono::time_point<std::chrono::high_resolution_clock>;

class CBase
{
public:
	int a = 0;
};

class CB : public CBase
{
public:
	CB() 
	{
		std::cout << "[CB::CB].\n" << std::endl;
	}

	~CB()
	{
		std::cout << "[CB::~CB]b=" << b << ".\n" << std::endl;
	}

	int b = 1;
	time_point_tt tv;
};

const int MAX_THREAD_QUEUE_TEST_SIZE = 100000;

//测试线程类
class CTask
{
public:
	CTask() = default;

	~CTask() { m_run = false; }

	void Put(std::shared_ptr<CB> msg)
	{
		m_thread_queue.Push(msg);
	}

	void start()
	{
		m_run = true;
		tt = std::thread([&]()
			{
				svc();
			});

	}
	
	void Close()
	{
		tt.join();
		std::cout << "Thread is Finish." << std::endl;
	}

	void svc()
	{
		while (m_run)
		{
			shared_ptr<CB> msg;
			if (true == m_thread_queue.Pop(msg))
			{
				//获得了数据，进行处理
				if(msg->b == 0)
				{
					m_tv_bgein = msg->tv;
				}
				
				if (msg->b == MAX_THREAD_QUEUE_TEST_SIZE - 1)
				{
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> tm = end - m_tv_bgein;	// 毫秒
                    std::cout << "[svc]time: " << tm.count() << "ms" << std::endl;
				}
			}
		}

		std::cout << "Thread is over." << std::endl;
	}

private:
	CMessageQueue<shared_ptr<CB>> m_thread_queue;
	bool m_run = false;
	int m_msg_recv_count = 0;
	time_point_tt m_tv_bgein;
	std::thread tt;
};

void Test_Shard_Ptr_Queue(CTask& task)
{
	auto start = std::chrono::high_resolution_clock::now();

	//测算10000个对象处理时间
	for (int i = 0; i < MAX_THREAD_QUEUE_TEST_SIZE; i++)
	{
		auto msg = std::make_shared<CB>();
		if (i == 0)
		{
			msg->tv = start;
		}
		msg->b = i;

		task.Put(msg);
	}
}

void Test_Fmt()
{
	string value = fmt::format("Name={0}, Age={1}.\n", "freeeyes", 12);
		
	std::cout << value << std::endl;
	
	value= "";
	string strName = "freeeyes";
	
	for(auto f : strName)
	{
		value += fmt::format("{0:#x} ", (unsigned char)f);
	}
			
	std::cout << "value=" << value << std::endl;
		
	int nData = 1;
	value = fmt::format("{0:04d}", nData);
		
	std::cout << "number=" << value << std::endl;
		
	value = fmt::format("{0:04d}-{1:02d}-{2:02d} {3:02d}:{4:02d}:{5:02d}", 2020, 1, 1, 12, 0, 0);
		
	std::cout << "date=" << value << std::endl;		
}

std::string getCurrentSystemTime()
{
	auto tt = std::chrono::system_clock::to_time_t
		(std::chrono::system_clock::now());
	struct tm* ptm = localtime(&tt);
	char date[60] = { 0 };
	sprintf(date, "%d-%02d-%02d-%02d.%02d.%02d",
		(int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
		(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
	return std::string(date);
}

class CTime_Task
{
public:
	CTime_Task()  = default;
	
	void Test_Timer_Task(brynet::TimerMgr::Ptr timerMgr)
	{
		std::cout  << "CTime_Task:" << getCurrentSystemTime() << std::endl;
		
	  Start_Timer(timerMgr);
	}	
	
	void Start_Timer(brynet::TimerMgr::Ptr timerMgr)
	{
			auto timer = timerMgr->addTimer(std::chrono::seconds(1), [&, timerMgr]() {
	      Test_Timer_Task(timerMgr);
	    });
	}	
};

class CTime_Task_1
{
public:
	CTime_Task_1()  = default;
	
	void Test_Timer_Task(brynet::TimerMgr::Ptr timerMgr)
	{
		std::cout << "CTime_Task_1:" << getCurrentSystemTime() << std::endl;
		
	  Start_Timer(timerMgr);
	}	
	
	void Start_Timer(brynet::TimerMgr::Ptr timerMgr)
	{
			auto timer = timerMgr->addTimer(std::chrono::seconds(2), [&, timerMgr]() {
	      Test_Timer_Task(timerMgr);
	    });
	}	
};


void Test_Timer()
{
	brynet::TimerMgr::Ptr timerMgr;
		
	CTime_Task Time_Task;
	CTime_Task_1 CTime_Task_1;
		
	timerMgr = std::make_shared<brynet::TimerMgr>();
		
	Time_Task.Test_Timer_Task(timerMgr);
	CTime_Task_1.Test_Timer_Task(timerMgr);
		
	std::thread tt([timerMgr]()
        {
				timerMgr->schedule(); 
        });
        
  std::this_thread::sleep_for(chrono::seconds(60));
        
 	timerMgr->Close();       
  
  cout << "[Test_Timer]End" << endl;
  getchar();
}

void Test_Task()
{
	CTask task;

	task.start();

	Test_Shard_Ptr_Queue(task);
	
	task.Close();	
}

void Test_time_stamp()
{
	auto now = CTimeStamp::Get_Time_Stamp();
		
	cout << "time now is " << CTimeStamp::Get_DateTime(now) << endl;
		
	auto future = CTimeStamp::Add_Time_Milliseconds(now, 3000);
		
	cout << "time future is " << CTimeStamp::Get_DateTime(future) << endl;
		
	auto interval = CTimeStamp::Get_Time_Difference(future, now);
		
	cout << "time interval is " << interval << endl;
	
	auto ms = now.time_since_epoch();  // 计算纪元时间
  cout << "curr time second:" << std::chrono::duration_cast<std::chrono::seconds>(ms).count() <<endl;	
}

void Test_Logic_Thread()
{
	TMS objTms;
	int a = 1;
	
	objTms.Init();
	
	objTms.CreateLogic(1);
	
	objTms.AddMessage(1, [a]()
		{
			cout << "message is arrived(" << a << ")." << endl;
		});
		
	objTms.AddMessage(1, std::chrono::seconds(2), [a]()
		{
			cout << "timer message is arrived(" << a << ")." << endl;
		});		
		
	getchar();
	objTms.Close();
}

class CServerObject
{
public:
    CServerObject()
    {
    }
};

int (*LoadModuleData)(CServerObject* pServerObject) = nullptr;

void Load_Library()
{
	auto handler = PSS_dlopen("/root/shiqiang/PSS/Build/Linux/libTcpTest.so", RTLD_NOW);
	
	LoadModuleData = (int(*)(CServerObject*))PSS_dlsym(handler, "LoadModuleData");
	
	if(nullptr == LoadModuleData)
	{
		cout << "library error:" << PSS_dlerror() << endl;			
	}
	
	PSS_dlClose(handler);
}

using App_Singleton = PSS_singleton<CB>;

void Test_singletion()
{
	App_Singleton::instance()->b = 2;
}

int main()
{
	//Test_Task();
	
	//Test_Fmt();
	
	//Test_Timer();
	
	//Test_time_stamp();

	//Test_Logic_Thread();
	
	//Load_Library();
	
	Test_singletion();

  return 0;
}


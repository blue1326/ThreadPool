
#include "pch.h"
#pragma comment(lib,"../dll/System_Assist.lib")
#include <Windows.h>
#include "CThreadHolder.h"
using namespace ThreadPool;
//#include "CThreadHolder.h"
//using namespace std;
class testclass
{
public:
	testclass()
	{

	}
	int x = 0;
	testclass(const testclass &T)
	{
		cout << "cpy!" << endl;
	}
	void test()
	{
		x++;
		cout << "test" << x << endl;
	}
};
void independentTest()
{
	for (int i = 0; i < 10; i++)
	{
		cout <<"indep test" << i <<"sec increase " <<  endl;
		Sleep(1000);
	}
	cout << "10sec over independent test over" << endl;
}
void opentest()
{
	cout << "test2" << endl;
}

int main()
{
	cout << "Begin Program" << endl;

	//shared_ptr<CThreadHolder> worker = make_shared<CThreadHolder>();
	shared_ptr<CThreadHolder> worker = CThreadHolder::GetInstance();
	cout << "WorkerSystem Start" << endl;
	worker->SetIndependentWorkers(2);//make independent workers;
	shared_ptr<testclass> tst = make_shared<testclass>();
	testclass tst2;
	while (true)
	{
		string tmp;
		cin >> tmp;
		if (tmp == "exit")
		{
			worker->Release();
			break;
		}
		
		if (tmp == "awake")
			worker->Awake_all();
		if (tmp == "auto")
			worker->SetAwakeMode(CThreadHolder::AWAKE_AUTOAWAKE);
		else if (tmp == "passive")
			worker->SetAwakeMode(CThreadHolder::AWAKE_PASSIVERUNNING);
		else if (tmp == "manual")
			worker->SetAwakeMode(CThreadHolder::AWAKE_MANUALAWAKE);

		if (tmp == "independent")
			worker->SetTask(CThreadHolder::JOB_INDEP, independentTest);

		if (tmp == "member")
		{
			int x = 0;
			for (int i = 0; i < 100; i++)
			{
				worker->SetTask(CThreadHolder::JOB_MAIN, bind(&testclass::test,tst.get()));
				
			}
		}
		else
		{
			worker->SetTask(CThreadHolder::JOB_MAIN, bind([](string _str) {cout << "type : "<<_str << endl; },tmp));
		}
	}
	cout << "End Program" << endl;

}


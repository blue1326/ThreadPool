
#include "pch.h"

//#include "CThreadHolder.h"
//using namespace std;
int main()
{
	cout << "Begin Program" << endl;
	shared_ptr<CThreadHolder> worker = make_shared<CThreadHolder>();
	cout << "WorkerSystem Rollout" << endl;
	worker->SetIndependentWorkers(2);
	while (true)
	{
		string tmp;
		cin >> tmp;
		if (tmp == "exit")
			break;
		if (tmp == "aw")
			worker->Awake_all();
		else
		{
			worker->SetJob(CThreadHolder::JOB_MAIN, []() {cout << "type  " << endl; });
		}
	}
	cout << "End Program" << endl;

}


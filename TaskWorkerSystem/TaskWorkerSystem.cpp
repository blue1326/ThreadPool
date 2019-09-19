
#include "pch.h"

#include "CThreadHolder.h"
//using namespace std;
int main()
{
	cout << "Begin Program" << endl;
	shared_ptr<CThreadHolder> worker = make_shared<CThreadHolder>();
	cout << "WorkerSystem Rollout" << endl;
	worker->SetIndependentWorkers(2);
	cout << "End Program" << endl;

}


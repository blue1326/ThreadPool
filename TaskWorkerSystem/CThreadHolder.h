#pragma once
#include "CWorker.h"
#include "SystemAnalizer.h"
#include "CJopList.h"
class CThreadHolder
{

public:
	CThreadHolder(size_t _numWorkers);//customized Workers count
	CThreadHolder(); //Auto Setting Workers ex)thread count = ( processor count * 2 ) + 1
	virtual ~CThreadHolder();

private:
	void Initialize();
	void Activate();
	void Deactivate();

public:
	void SetIndependentWorkers(unsigned int _NumWorkers);

	
	//void Thread_Basic();

private:
	unique_ptr<SystemAnalizer> m_MA;
private:
	size_t m_NumWorkers;
private:
	typedef list<unique_ptr<CWorker>> WORKERLIST;
	shared_ptr<mutex> m_MainMtx;
	shared_ptr<condition_variable> m_MainCv;
	WORKERLIST m_WorkerList;
	shared_ptr<condition_variable> m_IndieCv;
	shared_ptr<mutex> m_IndieMutex;
	WORKERLIST m_IndependentWorkerList;

	enum JOBLISTFLAG
	{
		JOB_MAIN,
		JOB_INDIE,
		JOB_END
	};
	shared_ptr<CJopList> m_JobList[JOB_END];

private:
	enum BitFlag
	{
		CUSTOM = 0, //Custosize
		AUTO = 1, //Autosize
		STATE2 = 2,
		STATE3 = 4,
		STATE4 = 8,
		STATE5 = 16,
		STATE6 = 32,
		STATE7 = 64,
		STATE8 = 128,
		IS_ON = 256,
		FULLSTATE =  STATE2 | STATE3 | STATE4 | STATE5 | STATE6 | STATE7 | STATE8 | IS_ON
	};

private:
	
	unsigned short StateFlag;
};


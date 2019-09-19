#include "pch.h"
#include "CThreadHolder.h"
//#include "CWorker.h"
//////////////////////////////////////////////////////////////////////////
//AKA ThreadPool
//////////////////////////////////////////////////////////////////////////

CThreadHolder::CThreadHolder(size_t _numWorkers)
	: StateFlag(CUSTOM),
	m_NumWorkers(_numWorkers)
{
	m_MA = make_unique<SystemAnalizer>(SystemAnalizer::U_KBYTE);
	Initialize();
}


CThreadHolder::CThreadHolder()
	:StateFlag(AUTO)
{
	m_MA = make_unique<SystemAnalizer>(SystemAnalizer::U_KBYTE);
	Initialize();
}

CThreadHolder::~CThreadHolder()
{
	/*for (const auto &worker : m_WorkerList)
	{
		worker->ExitFunc();
	}
	for (const auto &worker : m_IndependentWorkerList)
	{
		worker->ExitFunc();
	}*/
}



void CThreadHolder::Initialize()
{
	m_MainMtx = make_shared<mutex>();
	m_MainCv = make_shared<condition_variable>();
	m_JobList[JOB_MAIN] = make_shared<CJopList>();
	if ((StateFlag&AUTO) == AUTO)
	{
		m_NumWorkers = (m_MA->GetSystemInfo()->dwNumberOfProcessors * 2) + 1;
	}
	for (size_t i = 0; i < m_NumWorkers; i++)
	{
		auto worker = make_unique<CWorker>();
		
		worker->initialize();
		worker->SetConditionVariable(m_MainCv);
		worker->SetMutex(m_MainMtx);
		worker->SetJobList(m_JobList[JOB_MAIN]);
		worker->Activate();
		m_WorkerList.push_back(move(worker));

	}

	

	Activate();
}


void CThreadHolder::Activate()
{
	StateFlag |= IS_ON;
}

void CThreadHolder::Deactivate()
{
	StateFlag ^= IS_ON;
}

void CThreadHolder::SetIndependentWorkers(unsigned int _NumWorkers)
{
	WORKERLIST::iterator iter = m_WorkerList.begin();
	WORKERLIST::iterator iter_end = m_WorkerList.end();
	int icnt = 0;
	for (; iter != iter_end; ++iter)
	{
		if (icnt < _NumWorkers)
		{
			if (!(*iter)->WorkingOn())
			{
				m_IndependentWorkerList.push_back(move(*iter));
				iter = m_WorkerList.erase(iter);
				icnt++;

			}
			else
			{
				iter++;
			}
		}
		else
		{
			break;
		}
	}
}


#include "stdafx.h"
#include "CThreadHolder.h"
//#include "CWorker.h"
//////////////////////////////////////////////////////////////////////////
//AKA ThreadPool
//////////////////////////////////////////////////////////////////////////
using namespace ThreadPool;

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

void CThreadHolder::Awake_all()
{
	m_MainCv->notify_all();
	cout << "all threads awaken" << endl;
}

void CThreadHolder::Awake(int _awakeCnt)
{
	for (int i = 0; i < _awakeCnt; i++)
	{
		m_MainCv->notify_one();
	}
	cout << "total " << _awakeCnt << " Awaken" << endl;
	//testcode
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

void CThreadHolder::SetJob(JOBLISTFLAG targetJlist, function<void()> _func)
{
	if (m_JobList[targetJlist] == nullptr)
	{
		cout << "Joblist is not allocate" << endl;
		return;
	}
	m_JobList[targetJlist]->Enqueue(_func);
}

void CThreadHolder::SetAwakeMode(AWAKEMODE _mode)
{
	switch (_mode)
	{
	case CThreadHolder::AWAKE_PASSIVE:
		StateFlag |= AWAKE;
		break;
	case CThreadHolder::AWAKE_AUTO:
		StateFlag ^= AWAKE;
		break;
	default:
		break;
	}
}


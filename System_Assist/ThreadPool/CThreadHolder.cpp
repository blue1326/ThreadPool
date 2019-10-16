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
	m_modeMain = CThreadHolder::AWAKE_AUTOAWAKE;
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


void ThreadPool::CThreadHolder::Awake_all_Indep()
{
	m_IndepCv->notify_all();
}

void ThreadPool::CThreadHolder::Awake_Indep(int _awakeCnt)
{
	for (int i = 0; i < _awakeCnt; i++)
	{
		m_IndepCv->notify_one();
	}
}


//has trouble
void CThreadHolder::SetIndependentWorkers(unsigned int _NumWorkers)
{
	m_modeIndep = CThreadHolder::AWAKE_AUTOAWAKE;
	m_IndepMutex = make_shared<mutex>();
	m_IndepCv = make_shared<condition_variable>();
	m_JobList[JOB_INDEP] = make_shared<CJopList>();
	WORKERLIST::iterator iter = m_WorkerList.begin();
	WORKERLIST::iterator iter_end = m_WorkerList.end();
	int icnt = 0;
	for (; iter != iter_end; ++iter)
	{
		if (icnt < _NumWorkers)
		{
			if (!(*iter)->WorkingOn())
			{
				(*iter)->initialize();
				(*iter)->SetConditionVariable(m_IndepCv);
				(*iter)->SetMutex(m_IndepMutex);
				(*iter)->SetJobList(m_JobList[JOB_INDEP]);
				(*iter)->Activate();
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
	Awake_all_Indep();
	
}

void CThreadHolder::SetTask(JOBLISTFLAG targetJlist, function<void()> _func)
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
	CWorker::AWAKEMODE awmod;
	switch (_mode)
	{
	case CThreadHolder::AWAKE_PASSIVERUNNING:
		m_modeMain = AWAKE_PASSIVERUNNING;
		awmod = CWorker::PASSIVERUNNING;
		break;
	case CThreadHolder::AWAKE_AUTOAWAKE:
		m_modeMain = AWAKE_AUTOAWAKE;
		awmod = CWorker::AUTOAWAKE;
		break;
	case CThreadHolder::AWAKE_MANUALAWAKE:
		m_modeMain = AWAKE_MANUALAWAKE;
		awmod = CWorker::MANUALAWAKE;
		break;
	default:
		return;
	}
	for (auto &worker : m_WorkerList)
	{
		worker->SetAwakeMode(awmod);
	}
	m_MainCv->notify_all();
}

void ThreadPool::CThreadHolder::SetAwakeModeIndep(AWAKEMODE _mode)
{
	CWorker::AWAKEMODE awmod;
	switch (_mode)
	{
	case CThreadHolder::AWAKE_PASSIVERUNNING:
		m_modeIndep = AWAKE_PASSIVERUNNING;
		awmod = CWorker::PASSIVERUNNING;
		break;
	case CThreadHolder::AWAKE_AUTOAWAKE:
		m_modeIndep = AWAKE_AUTOAWAKE;
		awmod = CWorker::AUTOAWAKE;
		break;
	case CThreadHolder::AWAKE_MANUALAWAKE:
		m_modeIndep = AWAKE_MANUALAWAKE;
		awmod = CWorker::MANUALAWAKE;
		break;
	default:
		return;
	}
	for (auto &worker : m_IndependentWorkerList)
	{
		worker->SetAwakeMode(awmod);
	}
	m_MainCv->notify_all();
}

#include "stdafx.h"
#include "CWorker.h"

using namespace ThreadPool;
CWorker::CWorker()
	:m_WorkerThread(nullptr)
	,isRunning(false)
{
	m_WorkerThread = nullptr;
	m_Job = nullptr;
	m_Joblist = nullptr;
	m_mode = AUTOAWAKE;
}


CWorker::~CWorker()
{
	m_Job = nullptr;
	isRunning = false;
	m_WorkerThread->join();
}



void CWorker::initialize()
{
	isRunning = false;
	if (m_WorkerThread != nullptr)
		m_WorkerThread->join();
	
	m_uLock.reset();
	m_cv.reset();
	m_mtx.reset();
	
}


void CWorker::Activate()
{
	isRunning = true;
	m_uLock = make_shared<unique_lock<mutex>>(*(m_mtx.get()),std::defer_lock);

	m_WorkerThread = unique_ptr<thread>(new thread([this]() {this->BasicWorking(); }));
	
}

void CWorker::SetConditionVariable(shared_ptr<condition_variable> _cv)
{
	m_cv = _cv;
}

void CWorker::SetMutex(shared_ptr <mutex> _mtx)
{
	m_mtx = _mtx;
}

void CWorker::SetJobList(shared_ptr<CTaskList> _joblist)
{
	m_Joblist = _joblist;
}

void CWorker::BasicWorking()
{
	while (isRunning)
	{
		if (m_mtx != nullptr)
		{
			
			m_uLock->lock();

			if (m_Joblist != nullptr && !m_Joblist->Empty())
			{
				m_Job = m_Joblist->GetJobAndDequeue();
			}
			m_uLock->unlock();
		}

		
		if (m_Job != nullptr)
		{
			m_atmc_ActiveCount->fetch_add(1);
			m_Job();
			m_Job = nullptr;
			m_atmc_ActiveCount->fetch_sub(1);
		}
		if (m_Joblist->Empty())
		{
			m_uLock->lock();
			switch (m_mode)
			{
			case ThreadPool::CWorker::PASSIVERUNNING:
				break;
			case ThreadPool::CWorker::AUTOAWAKE:
				m_cv->wait(*m_uLock.get(), [this]() {return this->m_Joblist->Empty(); });
				break;
			case ThreadPool::CWorker::MANUALAWAKE:
				m_cv->wait(*m_uLock.get());
				break;
			default:
				break;
			}
			//m_cv->wait(*m_uLock.get());
			m_uLock->unlock();
		}
		
		
	}
	cout << "thread stop!" << endl;
}

void CWorker::SetAtomicVariable(shared_ptr<atomic<int>> _atomicVariable)
{
	m_atmc_ActiveCount = _atomicVariable;
}

void CWorker::SetJobQueue()
{

}

bool CWorker::WorkingOn()
{
	if (m_Job == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void ThreadPool::CWorker::SetAwakeMode(AWAKEMODE _mode)
{
	m_mode = _mode;
}

void ThreadPool::CWorker::Release()
{
	isRunning = false;
	if (m_WorkerThread != nullptr)
		m_WorkerThread->join();
}


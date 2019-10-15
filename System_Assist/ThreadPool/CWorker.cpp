#include "stdafx.h"
#include "CWorker.h"

using namespace ThreadPool;
CWorker::CWorker()
	:m_WorkerThread(nullptr)
	,isRunning(false)
{
	m_WorkerThread = nullptr;
	m_Job = nullptr;
	//m_wk_funcList.reset();
	m_Joblist = nullptr;
}


CWorker::~CWorker()
{
	//m_wk_funcList.reset();
	m_Job = nullptr;
	isRunning = false;
	m_WorkerThread->join();
	//int x = 0;
}

void CWorker::ExitFunc()
{
	//isRunning = false;
}

void CWorker::initialize()
{
	isRunning = false;
	if (m_WorkerThread != nullptr)
		m_WorkerThread->join();
	m_cv.reset();
	m_mtx.reset();
}


void CWorker::Activate()
{
	isRunning = true;
	m_uLock = make_shared<unique_lock<mutex>>(*(m_mtx.get()),std::defer_lock);

	//m_uLock = shared_ptr<unique_lock<mutex>>(new unique_lock<mutex>(*m_mtx.get()));
	m_WorkerThread = unique_ptr<thread>(new thread([this]() {this->BasicWorking(); }));
	//m_WorkerThread = unique_ptr<thread>(new thread(BasicWorking))
	
}

void CWorker::SetConditionVariable(shared_ptr<condition_variable> _cv)
{
	m_cv = _cv;
}

void CWorker::SetMutex(shared_ptr <mutex> _mtx)
{
	m_mtx = _mtx;
}

//void CWorker::SetJob(function<void()>& _job)
//{
//
//}

void CWorker::SetJobList(shared_ptr<CJopList> _joblist)
{
	m_Joblist = _joblist;
}

void CWorker::BasicWorking()
{
	while (isRunning)
	{
		if (m_mtx != nullptr)
		{
			//unique_lock<mutex> lock(*m_mtx.get());
			//unique_lock<mutex> lock(*m_mtx.get(), std::defer_lock);
			//m_uLock
			//lock.try_lock();
			m_uLock->lock();
			//m_mtx->lock();

			if (m_Joblist != nullptr && !m_Joblist->Empty())
			{
				m_Job = m_Joblist->GetJobAndDequeue();
			}
			//m_mtx->unlock();
			m_uLock->unlock();
			//lock.unlock();
		}
	//	unique_lock<mutex> lock(*m_mtx.get());
		//m_cv->wait(lock, [this]() {return m_Job})
		
		
		//system("CLS");
		//cout << "Running..." << endl;

		//m_atmI_ActiveCount->fetch_add(1);
		if (m_Job != nullptr)
		{
			m_Job();
			m_Job = nullptr;
		}
		if (m_Joblist->Empty())
		{
			m_uLock->lock();
			m_cv->wait(*m_uLock.get());
			m_uLock->unlock();
		}
		//m_atmI_ActiveCount->fetch_sub(1);
		
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

void CWorker::SetUniqueLock(shared_ptr < unique_lock<mutex>> _ulock)
{
	m_uLock = _ulock;
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

#pragma once
#include "CJopList.h"
namespace ThreadPool
{
	class CWorker
	{
	public:
		CWorker();
		virtual ~CWorker();
	private:
		unique_ptr<thread> m_WorkerThread;
		bool isRunning;
	private:
		void BasicWorking();
	private:
		void StopThread();


	private:
		//weak_ptr<list<function<void()>>> m_wk_funcList;
		shared_ptr<CJopList> m_Joblist;
		function<void()> m_Job;
	private:
		shared_ptr<condition_variable> m_cv;
		shared_ptr <mutex> m_mtx;

		shared_ptr < unique_lock<mutex>> m_uLock;

		shared_ptr<atomic<int>> m_atmc_ActiveCount;
	public:
		void ExitFunc();
	public:
		void initialize();
		void Activate();
		void SetConditionVariable(shared_ptr<condition_variable> _cv);
		void SetMutex(shared_ptr <mutex> _mtx);
		//void SetJob(function<void()>& _job);
		void SetJobList(shared_ptr<CJopList> _joblist);
		void SetAtomicVariable(shared_ptr<atomic<int>> _atomicVariable);
		void SetJobQueue();
		void SetUniqueLock(shared_ptr < unique_lock<mutex>> _ulock);

		bool WorkingOn();
	};

}
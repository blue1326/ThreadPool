#ifndef SystemAssist_CWorker_h__
#define SystemAssist_CWorker_h__



#include "CJopList.h"
namespace ThreadPool
{
	class __declspec(dllexport) CWorker
	{
	public:
		CWorker();
		virtual ~CWorker();
	public :
		enum AWAKEMODE
		{
			PASSIVERUNNING,
			AUTOAWAKE,
			MANUALAWAKE
		};
	private:
		unique_ptr<thread> m_WorkerThread;
		bool isRunning;
		AWAKEMODE m_mode;
	private:
		void BasicWorking();
	
		


	private:
		//weak_ptr<list<function<void()>>> m_wk_funcList;
		shared_ptr<CTaskList> m_Joblist;
		function<void()> m_Job;
		

	private:
		shared_ptr<condition_variable> m_cv;
		shared_ptr <mutex> m_mtx;
		shared_ptr < unique_lock<mutex>> m_uLock;
		shared_ptr<atomic<int>> m_atmc_ActiveCount;
			
	public:
		void initialize();
		void Activate();
		void SetConditionVariable(shared_ptr<condition_variable> _cv);
		void SetMutex(shared_ptr <mutex> _mtx);
		//void SetJob(function<void()>& _job);
		void SetJobList(shared_ptr<CTaskList> _joblist);
		void SetAtomicVariable(shared_ptr<atomic<int>> _atomicVariable);
		void SetJobQueue();

		bool WorkingOn();
		void SetAwakeMode(AWAKEMODE _mode);
		void Release();
	};

}
#endif // CWorker_h__
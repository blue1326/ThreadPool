#ifndef SystemAssist_CThreadHolder_h__
#define SystemAssist_CThreadHolder_h__



#include "CWorker.h"
#include "SystemAnalizer.h"
#include "CJopList.h"
#include "Template.h"
using namespace SystemSpec;
namespace ThreadPool
{
	class __declspec(dllexport) CThreadHolder : public  CSingleton_SPtr<CThreadHolder>

	{
		/*private:
			static std::shared_ptr<CThreadHolder> m_SingletonInstance;
			static std::once_flag m_Singletonflag;*/
	public:
		CThreadHolder(size_t _numWorkers);//customized Workers count
		CThreadHolder(); //Auto Setting Workers ex)thread count = ( processor count * 2 ) + 1
		virtual ~CThreadHolder();

/*
		static shared_ptr<CThreadHolder> GetInstance()
		{
			call_once(m_Singletonflag, [] {m_SingletonInstance.reset(new CThreadHolder);
			});
			atexit(Destroy);
			return m_SingletonInstance;
		}
	private:
		static void Destroy()
		{
			m_SingletonInstance.reset();
		}*/
	public:
		enum TASKLISTFLAG
		{
			TASK_MAIN,
			TASK_INDEP,
			TASK_END
		};
		enum AWAKEMODE
		{
			AWAKE_PASSIVERUNNING,
			AWAKE_AUTOAWAKE,
			AWAKE_MANUALAWAKE
		};
	private:
		void Initialize();
		
	public:
		void Awake_all();
		void Awake_all_Indep();
		void Awake(int _awakeCnt);
		void Awake_Indep(int _awakeCnt);
	public:
		
		void SetTask(TASKLISTFLAG _targetTasklist, function<void()> _func);
		
		void SetAwakeMode(AWAKEMODE _mode);
		void SetAwakeModeIndep(AWAKEMODE _mode);

		void SetIndependentWorkers(unsigned int _NumWorkers);

	public:
		void Release();
	private:
		unique_ptr<SystemAnalizer> m_MA;
	private:
		size_t m_NumWorkers;
	private:
		typedef list<unique_ptr<CWorker>> WORKERLIST;
		shared_ptr<mutex> m_MainMtx;
		shared_ptr<condition_variable> m_MainCv;
		WORKERLIST m_WorkerList;
		AWAKEMODE m_modeMain;
		shared_ptr<atomic<int>> m_MainAtmc_ActiveCount;

		shared_ptr<condition_variable> m_IndepCv;
		shared_ptr<mutex> m_IndepMutex;
		WORKERLIST m_IndependentWorkerList;
		AWAKEMODE m_modeIndep;
		shared_ptr<atomic<int>> m_IndepAtmc_ActiveCount;

		shared_ptr<CTaskList> m_TaskList[TASK_END];
	public:
		int GetRunningThreadCnt(TASKLISTFLAG _targetTasklist);
		int GetTaskCnt(TASKLISTFLAG _targetTasklist);
	private:
		enum BitFlag
		{
			CUSTOM = 0, //Custosize
			AUTO = 1, //Autosize
		};

	private:

		unsigned short StateFlag;
	};
	/*template<typename T> std::shared_ptr<T> CSingleton<T>::m_pInstance = nullptr;
	template<typename T> std::once_flag CSingleton<T>::m_onceFlag;
*/
	 
}
#endif // CThreadHolder_h__
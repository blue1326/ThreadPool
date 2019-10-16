#pragma once
#include "CWorker.h"
#include "SystemAnalizer.h"
#include "CJopList.h"
#include "Template.h"
using namespace SystemSpec;
namespace ThreadPool
{
	class __declspec(dllexport) CThreadHolder :public CSingleton<CThreadHolder>
	{

	public:
		CThreadHolder(size_t _numWorkers);//customized Workers count
		CThreadHolder(); //Auto Setting Workers ex)thread count = ( processor count * 2 ) + 1
		virtual ~CThreadHolder();
	public:
		enum JOBLISTFLAG
		{
			JOB_MAIN,
			JOB_INDEP,
			JOB_END
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
		
		void SetTask(JOBLISTFLAG targetJlist, function<void()> _func);
		
		void SetAwakeMode(AWAKEMODE _mode);
		void SetAwakeModeIndep(AWAKEMODE _mode);

		void SetIndependentWorkers(unsigned int _NumWorkers);

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


		shared_ptr<condition_variable> m_IndepCv;
		shared_ptr<mutex> m_IndepMutex;
		WORKERLIST m_IndependentWorkerList;
		AWAKEMODE m_modeIndep;

		shared_ptr<CJopList> m_JobList[JOB_END];

	private:
		enum BitFlag
		{
			CUSTOM = 0, //Custosize
			AUTO = 1, //Autosize
		};

	private:

		unsigned short StateFlag;
	};

}
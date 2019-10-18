#pragma once
namespace ThreadPool
{
	class __declspec(dllexport) CTaskList
	{
	public:
		CTaskList();
		virtual ~CTaskList();

	private:
		shared_ptr<list<function<void()>>> m_Funclist;

	public:
		void Enqueue(function<void()> _func);
		void EnqueuePriority(function<void()> _func);
		function<void()> GetJobAndDequeue();
		bool Empty();
		int GetTaskCnt();
	};

}
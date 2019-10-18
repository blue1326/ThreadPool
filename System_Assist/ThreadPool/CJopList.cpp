#include "stdafx.h"
#include "CJopList.h"

using namespace ThreadPool;
CTaskList::CTaskList()
{
	m_Funclist = make_shared<list<function<void()>>>();
	
}


CTaskList::~CTaskList()
{
	m_Funclist->clear();
}

void CTaskList::Enqueue(function<void()> _func)
{
	m_Funclist->push_back(_func);
	
}

void CTaskList::EnqueuePriority(function<void()> _func)
{
	m_Funclist->push_front(_func);
}



function<void()> CTaskList::GetJobAndDequeue()
{
	function<void()> tmpfunc;
	tmpfunc = m_Funclist->front();
	m_Funclist->pop_front();
	return tmpfunc;
}

bool CTaskList::Empty()
{
	return m_Funclist->empty();
}

int ThreadPool::CTaskList::GetTaskCnt()
{
	return m_Funclist->size();
}




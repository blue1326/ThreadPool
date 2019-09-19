#include "pch.h"
#include "CJopList.h"


CJopList::CJopList()
{
	m_Funclist = make_shared<list<function<void()>>>();
}


CJopList::~CJopList()
{
	m_Funclist->clear();
}

void CJopList::Enqueue(function<void()> _func)
{
	m_Funclist->push_back(_func);
	
}

void CJopList::EnqueuePriority(function<void()> _func)
{
	m_Funclist->push_front(_func);
}

function<void()> CJopList::GetJobAndDequeue()
{
	function<void()> tmpfunc;
	tmpfunc = m_Funclist->front();
	m_Funclist->pop_front();
	return tmpfunc;
}

bool CJopList::Empty()
{
	return m_Funclist->empty();
}


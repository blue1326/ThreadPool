#pragma once
class CJopList
{
public:
	CJopList();
	virtual ~CJopList();

private:
	shared_ptr<list<function<void()>>> m_Funclist;

public:
	void Enqueue(function<void()> _func);
	void EnqueuePriority(function<void()> _func);
	function<void()> GetJobAndDequeue();
	bool Empty();
};


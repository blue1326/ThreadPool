#pragma once
#include <memory>
template<typename T>
class CSingleton
{
private:
	static std::shared_ptr<T> m_pInstance;
	static std::once_flag m_onceFlag;
public:
	static shared_ptr<T> GetInstance()
	{
		call_once(m_onceFlag, [] {m_pInstance.reset(new T);
		});
		atexit(Destroy);
		return m_pInstance;
	}
private:
	static void Destroy()
	{
		m_pInstance.reset();
	}

};
template<typename T> std::shared_ptr<T> CSingleton<T>::m_pInstance = nullptr;
template<typename T> std::once_flag CSingleton<T>::m_onceFlag;
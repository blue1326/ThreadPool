#include "stdafx.h"
#include "SystemAnalizer.h"


using namespace SystemSpec;
SystemAnalizer::SystemAnalizer(UNIT _unit)
	:m_unit(_unit)
	,byteDiv(1)
{
	memset(&m_MemStat, 0, sizeof(_MEMORYSTATUSEX));
	m_MemStat.dwLength = sizeof(m_MemStat);
	GlobalMemoryStatusEx(&m_MemStat);

	memset(&m_Sysinfo, 0, sizeof(SYSTEM_INFO));
	GetNativeSystemInfo(&m_Sysinfo);

}

SystemAnalizer::~SystemAnalizer()
{
}
void SystemAnalizer::AnalizeMemory()
{
	//_MEMORYSTATUSEX stat;
	GlobalMemoryStatusEx(&m_MemStat);
	//m_MemStat = stat;
	PrintMemorySpecs();
}

void SystemAnalizer::AnalizeSystem()
{
	GetNativeSystemInfo(&m_Sysinfo);
	PrintSystemSpecs();
}

SYSTEM_INFO* SystemAnalizer::GetSystemInfo()
{
	GetNativeSystemInfo(&m_Sysinfo);
	
	return &m_Sysinfo;
}

_MEMORYSTATUSEX* SystemAnalizer::GetMemoryStat()
{
	GlobalMemoryStatusEx(&m_MemStat);
	return &m_MemStat;
}

void SystemAnalizer::PrintMemorySpecs()
{
	for (size_t i = 0; i < m_unit; i++)
	{
		byteDiv *= Div;
	}
	_tprintf(TEXT("There is  %*ld percent of memory in use.\n"),
		7, m_MemStat.dwMemoryLoad);
	_tprintf(TEXT("There are %*I64d total KB of physical memory.\n"),
		7, m_MemStat.ullTotalPhys / byteDiv);
	_tprintf(TEXT("There are %*I64d free  KB of physical memory.\n"),
		7, m_MemStat.ullAvailPhys / byteDiv);
	_tprintf(TEXT("There are %*I64d total KB of paging file.\n"),
		7, m_MemStat.ullTotalPageFile / byteDiv);
	_tprintf(TEXT("There are %*I64d free  KB of paging file.\n"),
		7, m_MemStat.ullAvailPageFile / byteDiv);
	_tprintf(TEXT("There are %*I64d total KB of virtual memory.\n"),
		7, m_MemStat.ullTotalVirtual / byteDiv);
	_tprintf(TEXT("There are %*I64d free  KB of virtual memory.\n"),
		7, m_MemStat.ullAvailVirtual / byteDiv);

	// Show the amount of extended memory available.

	_tprintf(TEXT("There are %*I64d free  KB of extended memory.\n"),
		7, m_MemStat.ullAvailExtendedVirtual / byteDiv);

}

void SystemAnalizer::PrintSystemSpecs()
{
	
	_tprintf(TEXT("There is  %*ld sysprocessor.\n"), 7, m_Sysinfo.dwNumberOfProcessors );
	_tprintf(TEXT("There is  %*ld TYPE OF processor.\n"), 7, m_Sysinfo.dwProcessorType);
	
}

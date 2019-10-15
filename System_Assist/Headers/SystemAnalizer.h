#pragma once
#ifndef _MEMORYSTATUS
#include <Windows.h>
#endif
#ifndef _tprintf
#include <tchar.h>
#endif

//////////////////////////////////////////////////////////////////////////
//it is proto type
//this class is for the Check Global Memory Specs
//Will be soon i'll make process side Memory check module
//Memo:GetProcessMemoryInfo()...
namespace SystemSpec
{
	class SystemAnalizer
	{
	public:
		enum UNIT
		{
			U_BYTE = 1, U_KBYTE = 2, U_MBYTE = 3, U_GBYTE = 4
		};
	public:
		SystemAnalizer(UNIT _unit);
		virtual ~SystemAnalizer();

	private://MemoryAnalize
		_MEMORYSTATUSEX m_MemStat;

		const int Div = 1024;

		UNIT m_unit;
		size_t byteDiv;
		void PrintMemorySpecs();
	private://systemAnalize
		SYSTEM_INFO m_Sysinfo;


		void PrintSystemSpecs();
	public:
		void AnalizeMemory();
		void AnalizeSystem();
		SYSTEM_INFO* GetSystemInfo();
		_MEMORYSTATUSEX*GetMemoryStat();
	};
}

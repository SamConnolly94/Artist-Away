#ifndef LOGGER_H
#define LOGGER_H

#include <typeinfo>
#include <string>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <list>

#ifdef _DEBUG
#define _LOGGING_ENABLED
#endif

// The line number which will preceed the log line in the text file.
static int mLineNumber;
static int mMemoryLogLineNumber;

class CLogger
{
/* Signleton class methods. */
public:
	// Constructor.
	CLogger();
	~CLogger();
private:
	// String which writes a line of astericks.
	const std::string k256Astericks = "****************************************************************************************************************************************************************************************************************************************************************";
	// The log file which we will write debug info to.
	std::ofstream mLogFile;
	// Name of the debug log file..
	const std::string mDebugLogName = "Prio_Engine_Debug_Log.txt";
	// Name of the memory log file.
	const std::string mMemoryLogName = "Prio_Engine_Memory_Log.txt";
	// The log file which we will write memory info to.
	std::ofstream mMemoryLogFile;
	// A boolean flag which is toggled on / off depending on if _LOGGING_ENABLED is defined by the preprocessor and successfully opening the log file.
	bool mLoggingEnabled;
public:
	void WriteSubtitle(std::string name);
	void WriteLine(std::string text);
	void MemoryAllocWriteLine(std::string name);
	void MemoryDeallocWriteLine(std::string name);
	void CloseSubtitle();
private:
	// Analyse the logs and check if any memory has been allocated which has not been deallocated. 
	// Only works if allocWriteline and deallocWriteline are used.
	void MemoryAnalysis();
};

#endif
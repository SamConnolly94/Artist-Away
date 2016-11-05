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
	static CLogger& GetLogger();
private:
	// Constructor.
	CLogger();
public:
	// Delete the object prevent memory leaks.
	CLogger(CLogger const&) = delete;
	void operator = (CLogger const&) = delete;
private:
	const std::string k256Astericks = "****************************************************************************************************************************************************************************************************************************************************************";
	// A boolean flag which is toggled on / off depending on if _LOGGING_ENABLED is defined by the preprocessor and successfully opening the log file.
	bool mLoggingEnabled;

	// The log file which we will write debug info to.
	std::ofstream mLogFile;

	const std::string mDebugLogName = "Prio_Engine_Debug_Log.txt";

	// The log file which we will write memory info to.
	std::ofstream mMemoryLogFile;

	const std::string mMemoryLogName = "Memory_Log.txt";
public:
	void WriteLine(std::string text);

	void MemoryAllocWriteLine(std::string name);
	void MemoryDeallocWriteLine(std::string name);

	void WriteSubtitle(std::string name);
	void CloseSubtitle();

	void Shutdown();

	void MemoryAnalysis();
};

#endif
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include "../file/FileWriter.h"
#include <ctime>
#include <sstream>
#include <iomanip>

namespace utilities
{
	/*
	 * Warning levels to be used for logging.
	 * 
	 * Define #define NO_LOGGING to disable all logging, to allow the compiler to optimize calls.
	 * 
	 */
	enum class Severity : std::uint8_t
	{
		Info = 0,		//Info is just information.
		Warning = 1,	//Warnings should be resolved, but don't directly affect the program in a bad way.
	    Error = 2,		//Something went wrong and program execution is compromised, but can continue.
		Fatal = 3		//Fatal indicates the program should stop executing because it is beyond repair.
	};

	/*
	 * Log messages to the console and/or file.
	 */
	class Logger
	{
	public:
		explicit Logger(const std::string& fileName);

		~Logger();

	public:
		void log(const Severity severity, const std::string& message) const;

		/*
		 * Enable or disable console logging.
		 */
		void setConsoleLogging(bool console);

		/*
		 * Enable or disable file logging.
		 */
		void setFileLogging(bool file);

		/*
		 * Toggle timestamps on or off.
		 */
		void setTimeStamp(bool timeStamp);

		/*
		 * Set the cap at which the log queue will be written to file.
		 */
		void setFlushCap(const std::uint16_t cap);

		/*
		 * Save all the queued data to file.
		 */
		void saveToFile() const;

	private:
		std::string fileName;
		mutable std::vector<std::string> logged;

		bool timeStamp;
		bool logToFile;
		bool logToConsole;
		std::uint16_t flushCap;

		//Thread safety
	private:
		mutable std::mutex mutex;
	};

	inline Logger::Logger(const std::string& fileName) : fileName(fileName), timeStamp(true), logToFile(true), logToConsole(true), flushCap(100)
	{
		logged.reserve(flushCap);
	}

	inline Logger::~Logger()
	{
		saveToFile();
	}

	inline void Logger::log(const Severity severity, const std::string& message) const
	{
#ifndef NO_LOGGING

		//Lock for multithreaded access.
		std::lock_guard<std::mutex> lock(mutex);

		//Static array of prefixes.
		static const std::string prefixes[4] = { "[Info] ", "[Error] ", "[Warning] ", "[Fatal] " };

		if(logToConsole || logToFile)
		{
			std::string time;
			if(timeStamp)
			{
				//Old junk, use the new C++20 library instead when that is available. date.h
				struct std::tm newtime;
				auto t = std::time(nullptr);
				localtime_s(&newtime, &t);
				std::ostringstream oss;
				oss << std::put_time(&newtime, "[%d-%m-%Y %H-%M-%S] ");
				time = oss.str();
			}

			const std::string msg = time + (prefixes[static_cast<std::uint8_t>(severity)] + message);


			if(logToConsole)
			{
				std::cout << msg << std::endl;
			}

			if(logToFile)
			{
				logged.emplace_back(msg + "\n");
			}

			if(logged.size() >= flushCap)
			{
				saveToFile();
			}
		}
#endif
	}

	inline void Logger::setConsoleLogging(bool console)
	{
		logToConsole = console;
	}

	inline void Logger::setFileLogging(bool file)
	{
		logToFile = file;
	}

	inline void Logger::setTimeStamp(bool timeStamp)
	{
		this->timeStamp = timeStamp;
	}

	inline void Logger::setFlushCap(const std::uint16_t cap)
	{
		flushCap = cap;
		logged.reserve(flushCap);
	}

	inline void Logger::saveToFile() const
	{
		FileWriter writer(fileName);

		if(writer.open())
		{
			for (const auto& string : logged)
			{
				writer.write(string);
			}

			logged.clear();

			writer.close();
		}
		else
		{
			std::cout << "[Error] Could not log to file." << std::endl;
		}
	}
}

#pragma once

#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

class CLogger
{
	public:
		CLogger(std::string logfilename);
		
		enum class ESeverity
		{
			LOG_DEBUG,
			LOG_INFO,
			LOG_WARNING,
			LOG_ERROR
		};

		void log(std::string message, CLogger::ESeverity s);
		void debug(std::string message);
		void info(std::string message);
		void warning(std::string message);
		void error(std::string message);

	private:
		std::ofstream m_logfile;
};

std::ostream & operator<<(std::ostream &os, CLogger::ESeverity const &s);

#include "Logger.h"

CLogger::CLogger(std::string logfilename)
{
	m_logfile.open("test.txt", std::ifstream::app);
}

void CLogger::log(std::string message, CLogger::ESeverity s)
{
	std::stringstream ss;
	std::time_t t = std::time(nullptr);

	ss << t;
	ss << " ";
	ss << s;
	ss << message;
	ss << std::endl;
}

void CLogger::debug(std::string message)
{
	log(message, ESeverity::LOG_DEBUG);
}
void CLogger::info(std::string message)
{
	log(message, ESeverity::LOG_INFO);
}
void CLogger::warning(std::string message)
{
	log(message, ESeverity::LOG_WARNING);
}
void CLogger::error(std::string message)
{
	log(message, ESeverity::LOG_ERROR);
}

std::ostream & operator<<(std::ostream &os, CLogger::ESeverity const &s)
{
	char * eSeverityToString[] = {
		"DEBUG",
		"INFO",
		"WARNING",
		"ERROR"
	};

	unsigned int eSeverityToUInt = (int)(s);

	return os << eSeverityToString[eSeverityToUInt];
}
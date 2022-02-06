#pragma once

#include <string>
#include <memory>

class MessageOutputter
{
public:
	typedef std::shared_ptr<MessageOutputter> Ptr;

	virtual void critical(const std::string& message) = 0;
	virtual void warning(const std::string& message) = 0;
	virtual void info(const std::string& message) = 0;
};
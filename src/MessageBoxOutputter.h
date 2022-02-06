#pragma once

#include "MessageOutputter.h"

#include <QWidget>

class MessageBoxOutputter : public MessageOutputter
{
public:
	MessageBoxOutputter(const std::string& title, QWidget* parent);
	~MessageBoxOutputter();

	void critical(const std::string& message) override;
	void warning(const std::string& message) override;
	void info(const std::string& message) override;

private:
	QWidget* _parent;
	std::string _title;
};
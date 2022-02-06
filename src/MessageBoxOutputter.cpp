#include "MessageBoxOutputter.h"

#include <QMessageBox>

MessageBoxOutputter::MessageBoxOutputter(const std::string & title, QWidget * parent):
	_title(title),
	_parent(parent)
{
}

MessageBoxOutputter::~MessageBoxOutputter()
{
}

void MessageBoxOutputter::critical(const std::string & message)
{
	QMessageBox::critical(_parent, QString::fromStdString(_title),
			QString::fromStdString(message), QMessageBox::Ok);
}

void MessageBoxOutputter::warning(const std::string & message)
{
	QMessageBox::warning(_parent, QString::fromStdString(_title),
		QString::fromStdString(message), QMessageBox::Ok);
}

void MessageBoxOutputter::info(const std::string & message)
{
	QMessageBox::information(_parent, QString::fromStdString(_title),
		QString::fromStdString(message), QMessageBox::Ok);
}

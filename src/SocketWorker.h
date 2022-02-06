#pragma once

#include "MessageOutputter.h"

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <stack>
#include <mutex>

class SocketWorker : public QObject 
{
	Q_OBJECT
public:
	SocketWorker(const MessageOutputter::Ptr& messOutputter, const int localPort, const int remotePort);
	~SocketWorker();

	int getLastDataGramm(QNetworkDatagram& data);
	void writeData(const QByteArray& byteArray);

public slots:
	void process(); 	/*  создает и запускает построитель отчетов */
	void release();    	/*  останавливает построитель отчетов */

signals:
	void finished(); 	/* сигнал о завершении  работы построителя отчетов */
	void newData();

private:
	QUdpSocket*				_localSocket;
	QUdpSocket*				_remoteSocket;
	MessageOutputter::Ptr	_messageOutputter;
	std::stack<QNetworkDatagram> _dataGramms;

	std::mutex _mutex;
	int _localPort;
	int _remotePort;

	void on_Recv();
};

#include "SocketWorker.h"
#include "Messages.h"

SocketWorker::SocketWorker(const MessageOutputter::Ptr& messOutputter, const int localPort, const int remotePort) :
	_messageOutputter(messOutputter),
	_localPort(localPort),
	_remotePort(remotePort)
{
	_localSocket = new QUdpSocket(this);
	_remoteSocket = new QUdpSocket(this);
}

SocketWorker::~SocketWorker()
{
	delete _localSocket;
	delete _remoteSocket;
}

int SocketWorker::getLastDataGramm(QNetworkDatagram & data)
{
	std::lock_guard<std::mutex> lock(_mutex);

	if (_dataGramms.empty())
		return -1;

	data = _dataGramms.top();
	_dataGramms.pop();

	return 0;
}

void SocketWorker::writeData(const QByteArray & byteArray)
{
	if (_remoteSocket->writeDatagram(byteArray, QHostAddress::LocalHost, _remotePort) == -1)
	{
		_messageOutputter->critical(messages::s_Error_RecvBytes);
		return;
	}
}

void SocketWorker::release()
{
	_localSocket->abort();
	emit finished();
}

void SocketWorker::process()
{
	if (!_localSocket->bind(QHostAddress::LocalHost, _localPort))
	{
		_messageOutputter->critical(messages::s_Error_SocketError);
		emit finished();
		return;
	}

	connect(_localSocket, &QUdpSocket::readyRead, this, &SocketWorker::on_Recv);
}

void SocketWorker::on_Recv()
{
	while (_localSocket->hasPendingDatagrams()) 
	{
		{
			std::lock_guard<std::mutex> lock(_mutex);
			QNetworkDatagram datagram = _localSocket->receiveDatagram();
			_dataGramms.push(datagram);
		}

		emit newData();
	}
}

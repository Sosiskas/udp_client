#include "ClientView.h"
#include "ui_MainWindow.h"
#include "MessageBoxOutputter.h"
#include "Messages.h"

#include <QThread>

ClientView::ClientView():
	_ui(new Ui::MainWindow()),
	_messageOutputter(new MessageBoxOutputter("UDP Client", this))
{
	_ui->setupUi(this);

	connectSignals();
	updateEnableBtns(false);
}

ClientView::~ClientView()
{
	delete _ui;
}

void ClientView::connectSignals()
{
	connect(_ui->sendBtn, &QPushButton::clicked, this, &ClientView::on_SendBtn);
	connect(_ui->connectBtn, &QPushButton::clicked, this, &ClientView::on_ConnectBtn);
	connect(_ui->disconnectBtn, &QPushButton::clicked, this, &ClientView::on_DisconnectBtn);
}

void ClientView::on_SendBtn()
{
	QString message_to_send = _ui->messageEdit->text();

	if (message_to_send.isEmpty())
	{
		_messageOutputter->warning(messages::s_Warning_EmptySendedMessage);
		return;
	}
	if (_worker == nullptr)
	{
		_messageOutputter->warning(messages::s_Error_NotInitSoks);
		return;
	}
	
	_ui->textBrowser->append("127.0.0.1:" + _ui->localSpin->text() + " " + message_to_send);

	_worker->writeData(message_to_send.toUtf8());
}

void ClientView::on_Recv()
{
	QNetworkDatagram datagram;

	while (_worker->getLastDataGramm(datagram) >= 0)
	{
		processTheDatagram(datagram);
	}
}

void ClientView::on_ConnectBtn()
{
	_worker = new SocketWorker(_messageOutputter, _ui->localSpin->value(), _ui->remoteSpin->value());
	QThread* thread = new QThread;
	_worker->moveToThread(thread);

	connect(_worker, &SocketWorker::newData, this, &ClientView::on_Recv);
	connect(thread, SIGNAL(started()), _worker, SLOT(process()));
	connect(_worker, SIGNAL(finished()), thread, SLOT(quit()));
	connect(_worker, SIGNAL(finished()), _worker, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(this, SIGNAL(stopAll()), _worker, SLOT(release()));
	thread->start();

	updateEnableBtns(true);
}

void ClientView::on_DisconnectBtn()
{
	disconnect(_worker, &SocketWorker::newData, this, &ClientView::on_Recv);

	emit stopAll();
	_worker = 0;

	updateEnableBtns(false);
}

void ClientView::processTheDatagram(const QNetworkDatagram & data)
{
	QString recived_message = QString::fromUtf8(data.data());
	_ui->textBrowser->append(data.senderAddress().toString() + ":" 
		+ _ui->remoteSpin->text() + " " + recived_message);
}

void ClientView::updateEnableBtns(const bool socks_init)
{
	if (socks_init)
	{
		_ui->connectBtn->setEnabled(false);
		_ui->disconnectBtn->setEnabled(true);
		_ui->sendBtn->setEnabled(true);
		_ui->localSpin->setEnabled(false);
		_ui->remoteSpin->setEnabled(false);
	}
	else
	{
		_ui->connectBtn->setEnabled(true);
		_ui->disconnectBtn->setEnabled(false);
		_ui->sendBtn->setEnabled(false);
		_ui->localSpin->setEnabled(true);
		_ui->remoteSpin->setEnabled(true);
	}	
}

#pragma once

#include "MessageOutputter.h"
#include "SocketWorker.h"

#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

class ClientView : public QMainWindow
{
	Q_OBJECT
public:
	ClientView();
	~ClientView();

private:
	Ui::MainWindow*			_ui;
	MessageOutputter::Ptr	_messageOutputter;
	SocketWorker*			_worker;

	void connectSignals();
	void on_SendBtn();
	void on_Recv();
	void on_ConnectBtn();
	void on_DisconnectBtn();
	void processTheDatagram(const QNetworkDatagram& data);
	void updateEnableBtns(const bool socks_init);

signals:
	void stopAll();
};
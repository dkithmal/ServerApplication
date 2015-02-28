#ifndef CLIENTCOMMANDHANDLER_H
#define CLIENTCOMMANDHANDLER_H

#include <QThread>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QByteArray>
#include <QStringList>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QDirIterator>
#include <QProcess>


class ClientCommandHandler : public QThread
{
	Q_OBJECT

public:
	ClientCommandHandler(int id,QObject *parent=0);
	~ClientCommandHandler();
	void run();

private:
	QTcpSocket *socket;
	int socketDescriptor;
	QByteArray mCommandBuffer;
	QFile *newImage;
	QSettings* settings;
	QString imageDirectory;
	QProcess *process;
	void DoLIST();
	void DoINIT(QString ip);
	void ProcessCommand(QString ClientCommand);
	void ExecuteCommand(QString ClientCommand);
	void SendResponse(QString data);
	void DoSaveNewImage(QByteArray Arg);
	void DoShowDeault();
	void DoShowSlideShow();
	void DoDeleteImages(QString imageNameList);
	void DoSetDeault(QString imageName);
	void loadConfigs();


private slots:
		void readyRead();
		void bytesWritten(qint64 bytes);
		void disconnected();

	
};

#endif // CLIENTCOMMANDHANDLER_H

#include "clientcommandhandler.h"

ClientCommandHandler::ClientCommandHandler(int id,QObject *parent)
	: QThread(parent)
{
	this->socketDescriptor = id;
	settings= new QSettings("settings.ini",QSettings::IniFormat);
	loadConfigs();
	process= new QProcess();

}

ClientCommandHandler::~ClientCommandHandler()
{

}


void ClientCommandHandler::loadConfigs()
{

	if(!(settings->value("imageDirectory").toString().isEmpty()))
	{
		imageDirectory=settings->value("imageDirectory").toString();

	}
	else
	{
		imageDirectory="D://Programming//images//";
		//imageDirectory="//home//damitha//programming//c++//test//";

	}

}


void ClientCommandHandler::run()
{
	//thread starting
	qDebug()<<socketDescriptor<<"Starting Client Handling Tread...";
	socket = new QTcpSocket();
	if(!socket->setSocketDescriptor(this->socketDescriptor))
	{		
		return;
	}


	connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
	connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);
	connect(socket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)),Qt::DirectConnection);
	qDebug()<<socketDescriptor<<"Client Connected";

	exec();
}




void ClientCommandHandler::readyRead()
{

	//Append to the command buffer
	mCommandBuffer.append(socket->readAll());

	if(mCommandBuffer.endsWith('\n')||mCommandBuffer.contains("FILEEND"))
	{
		if (mCommandBuffer.startsWith("NEWIMAGE"))
		{
			if(mCommandBuffer.contains("FILEEND"))
			{
				DoSaveNewImage(mCommandBuffer);
				mCommandBuffer.clear();

			}

		}
		else
		{
			QString command(mCommandBuffer.data());
			ProcessCommand(command);	
			qDebug()<<"data"<<mCommandBuffer;
			mCommandBuffer.clear();

		}

	}


}



void ClientCommandHandler::disconnected()
{
	qDebug()<<socketDescriptor<<"Disconnected";
	socket->deleteLater();
	exit(0);

}


void ClientCommandHandler::bytesWritten(qint64 bytes)
{
	qDebug()<<bytes<<" bytes written";
}



void ClientCommandHandler::ProcessCommand(QString ClientCommand)
{
	QStringList Commands = ClientCommand.split("\n");

	foreach(QString Command, Commands)
	{
		QString ClientCommand = Command.trimmed();
		if(ClientCommand != "")
		{
			//Execute the command
			ExecuteCommand(ClientCommand);
		}
	}
}




void ClientCommandHandler::ExecuteCommand(QString ClientCommand)
{


	QString Command = "";
	QString Arg = "";

	if(ClientCommand.contains(" "))
	{
		//Contains arguments
		int pos = ClientCommand.indexOf(" ");
		Command = ClientCommand.mid(0,pos).trimmed();
		Arg = ClientCommand.mid(pos + 1).trimmed();
	}
	else
	{
		//No arguments
		Command = ClientCommand.trimmed();
	}

	qDebug() << "Client: " << Command << " " << Arg;


	if (Command=="INIT")
	{
		DoINIT(Arg);
	}
	if (Command=="LIST")
	{
		DoLIST();
	}
	else if (Command=="DELETE")
	{
		DoDeleteImages(Arg);
	}
	else if (Command=="SETDEFAULT")
	{		
		DoSetDeault(Arg);
	}
	else if (Command=="SHOWDEFAULT")
	{
		DoShowDeault();
	}
	else if (Command=="SHOWSLIDESHOW")
	{
		DoShowSlideShow();
	}

}

void ClientCommandHandler::SendResponse(QString data)
{
	QByteArray datasend;
	datasend = data.toLatin1();
	socket->write(datasend);
	socket->waitForBytesWritten(5000);
}


void ClientCommandHandler::DoINIT(QString ip)
{
	QString name;
	 name = qgetenv("USER");
	//name = qgetenv("USERNAME");
	SendResponse(ip+'@'+name+"\n");
}



void ClientCommandHandler::DoLIST()
{
	QString imageList;

	QDirIterator dirIt(imageDirectory,QDirIterator::Subdirectories);
	while (dirIt.hasNext()) {
		dirIt.next();
		if (QFileInfo(dirIt.filePath()).isFile())
			if (QFileInfo(dirIt.filePath()).suffix() != "txt")
			{
				qDebug()<<dirIt.filePath();
				imageList.append(dirIt.fileName());
				imageList.append(',');
			}					
	}

	SendResponse("LIST "+imageList+'\n');

}

void ClientCommandHandler::DoSetDeault(QString imageName)
{
	QFile file(imageDirectory+"//default.txt");
	file.open((QIODevice::WriteOnly));	
	file.write(imageName.toLatin1());
	file.close();

}


void ClientCommandHandler::DoShowDeault()
{
	QFile file(imageDirectory+"//default.txt");
	file.open(QIODevice::ReadOnly);
	QString line = file.readLine().trimmed();

	if(!line.isNull())
	{
		process->close();
		process->start("feh -F -Z -z -Y "+imageDirectory+"//"+line);

	}



}

void ClientCommandHandler::DoShowSlideShow()
{

	process->close();
	process->start("feh -F -Z -z -Y -D5 "+imageDirectory);


}

void  ClientCommandHandler::DoDeleteImages(QString imageNameList)
{
	QStringList deleteImageList=imageNameList.split(',');

	for (int x=0;x<deleteImageList.size();x++)
	{
		if(QFile(imageDirectory+deleteImageList.at(x)).exists())
			QFile(imageDirectory+deleteImageList.at(x)).remove();
	}


}


void  ClientCommandHandler::DoSaveNewImage(QByteArray Arg)
{
	QByteArray fileName=Arg.mid(9,Arg.indexOf("NAMEEND")-9);
	QByteArray data=Arg.mid(Arg.indexOf("NAMEEND")+7,Arg.size()-(Arg.indexOf("NAMEEND")+7)+7).trimmed();

	newImage=new QFile(imageDirectory+fileName);
	newImage->open(QIODevice::Append);
	newImage->write(data);
	newImage->close();

}









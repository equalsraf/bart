#include "i3pagerwidget.moc"

#include <QtGui>
#include <qjson/serializer.h>
#include <qjson/parser.h>


I3PagerWidget::I3PagerWidget(BartWidget *parent)
:QLabel(parent), sock(0)
{

	connectToI3();
}

void I3PagerWidget::connectToI3()
{
	if ( sock && sock->state() != QLocalSocket::UnconnectedState ) {
		return;
	}

	sock = new QLocalSocket();
	connect(sock, SIGNAL(connected()),
			this, SLOT(connected()) );
	connect(sock, SIGNAL(disconnected()), 
			this, SLOT(disconnected()) );
	connect(sock, SIGNAL(error(QLocalSocket::LocalSocketError)), 
			this, SLOT(error(QLocalSocket::LocalSocketError)) );

	setText("---");
	sock->connectToServer("/home/raf/.i3/ipc.sock");
}

void I3PagerWidget::error(QLocalSocket::LocalSocketError socketError)
{
	QLocalSocket *sock = this->sock;

	if ( !sock  ) {
		return;
	} else if ( sock->error() != QLocalSocket::PeerClosedError ) {
		connect(parent(), SIGNAL(timeout()), this, SLOT(connectToI3()));
	}

}

void I3PagerWidget::connected()
{
	connect(sock, SIGNAL(readyRead()),
			this, SLOT(readyRead()) );

	QStringList l;
	l.append("workspace");
//	l.append("output");

	subscribe( l );
	getWorkspaces();
	getOutputs();
}

void I3PagerWidget::disconnected()
{
	connectToI3();
}

void I3PagerWidget::readyRead()
{
	while ( sock->bytesAvailable() ) {
		consume();
	}
}

void I3PagerWidget::refreshLabel()
{
	QListIterator< QMap<QString, QVariant> > it(workspaces);
	
	QString tmp;
	while( it.hasNext() ) {
		const QMap<QString, QVariant> &map  = it.next();

		tmp += "[";
		if ( map.value("urgent").toBool() ) {
			tmp += "<font color=\"red\">";
		}

		if ( map.value("focused").toBool() ) {
			tmp += QString("<b>%1</b>").arg(map.value("name").toString());
		} else {
			tmp += QString("%1").arg(map.value("name").toString());
		}
		if ( map.value("urgent").toBool() ) {
			tmp += "/<font>";
		}
		tmp += "]";

	}

	setText(tmp);
	update();
	
}

void I3PagerWidget::consume()
{
	QByteArray payload, rsp_m;
	qint32 len, cmd;
	QByteArray magic = QString(I3_IPC_MAGIC).toAscii();

	rsp_m = sock->read( magic.length() );
	if ( magic != rsp_m ) {
		qDebug() << "I3 magic mismatch";
		sock->close();
	}

	sock->read((char *)&len, 4);
	sock->read((char *)&cmd, 4);
	payload = sock->read(len);

	bool ok;
	QJson::Parser parser;
	QVariant v = parser.parse(payload, &ok);
	if ( !ok ) {
		qDebug() << "Error parsing payload:", parser.errorString();
	}

	switch(cmd) {
	case I3_IPC_EVENT_WORKSPACE:
		getWorkspaces();
		break;
	case I3_IPC_EVENT_OUTPUT:
		getOutputs();
		break;
	case I3_IPC_REPLY_TYPE_WORKSPACES:
		updateWorkspaces(v);
		break;
	case I3_IPC_REPLY_TYPE_OUTPUTS:
		break;
	}

}

void I3PagerWidget::updateWorkspaces(const QVariant &v)
{
	QList<QVariant> l = v.toList();
	QListIterator<QVariant> it(l);

	workspaces.clear();
	while( it.hasNext() ) {
		QVariant w = it.next();
		workspaces.append( w.toMap() );
	}

	refreshLabel();
}

void I3PagerWidget::subscribe(const QStringList& events)
{
	QJson::Serializer json;
	QByteArray qb = json.serialize(events);

	send(I3_IPC_MESSAGE_TYPE_SUBSCRIBE, qb);
}

void I3PagerWidget::send(qint32 cmd, const QByteArray& payload)
{
	qint32 len;

	sock->write(I3_IPC_MAGIC);

	len = payload.length();
	sock->write( (char *)&len, 4 );
	sock->write( (char *)&cmd, 4 );
	sock->write(payload);
}

void I3PagerWidget::getWorkspaces()
{
	send(I3_IPC_MESSAGE_TYPE_GET_WORKSPACES, QByteArray());
}

void I3PagerWidget::getOutputs()
{
	send(I3_IPC_MESSAGE_TYPE_GET_OUTPUTS, QByteArray());
}


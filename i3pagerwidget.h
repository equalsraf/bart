#ifndef __BART_I3PAGER__
#define __BART_I3PAGER__

#include <QtGui>
#include <QLocalSocket>
#include <i3/ipc.h>

#include "bartwidget.h"

class I3PagerWidget: public QLabel
{
	Q_OBJECT
public:
	I3PagerWidget(BartWidget *parent=0);

protected:
	void subscribe( const QStringList &);
	void consume();
	void send(qint32 cmd, const QByteArray& payload);
	void getWorkspaces();
	void getOutputs();

	void updateWorkspaces(const QVariant &);

	void refreshLabel();

private slots:
	void connected();
	void readyRead();
	void disconnected();
	void error(QLocalSocket::LocalSocketError socketError);

protected slots:
	void connectToI3();

private:
	QLocalSocket *sock;

	QList< QMap<QString, QVariant> > workspaces;
};


#endif

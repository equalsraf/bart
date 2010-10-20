#ifndef __BART_NOTIFYWIDGET__
#define __BART_NOTIFYWIDGET__

#include <QtGui>
#include <QtDBus>

#include "bartwidget.h"
#include "ui_simplewidget.h"

class NotifyWidget;

Q_DECLARE_METATYPE(QVariantMap)

class NotifyWidgetAdaptor: public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.freedesktop.Notifications")
public:
	NotifyWidgetAdaptor(NotifyWidget* w)
	: QDBusAbstractAdaptor((QObject* )w), widget(w)
	{
	}

public slots:

	QStringList GetCapabilities();

	uint Notify(const QString &app_name, uint replaces_id, const QString &app_icon, 
		const QString &summary, const QString &body, const QStringList &actions,
	       	const QVariantMap &hints, int expire_timeout);

	void CloseNotification(quint32 id);

signals:
	void NotificationClosed(quint32 id, quint32 reason);
	void ActionInvoked(quint32 id, const QString& reason);

private:
	NotifyWidget *widget;

};


class NotifyWidget: public QWidget, private Ui::SimpleWidget
{
	Q_OBJECT
public:
	NotifyWidget(BartWidget *parent);

public slots:
	QStringList GetCapabilities();
	void CloseNotification(quint32 id);
	int Notify(const QString& app, uint replace, const QString& icon, 
			const QString& summary, const QString& body,
			const QStringList& actions, const QMap<QString, QVariant> &hints,
			int timeout);

	void clear();


signals:
	void NotificationClosed(uint id, uint reason);
	void ActionInvoked(uint id, const QString& reason);

protected:
	QAtomicInt version;

};


#endif

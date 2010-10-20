#include "notifywidget.moc"

QStringList NotifyWidgetAdaptor::GetCapabilities() {
	return widget->GetCapabilities();
}


void NotifyWidgetAdaptor::CloseNotification(quint32 id) {
	widget->CloseNotification(id);
}


uint NotifyWidgetAdaptor::Notify(const QString &app_name, uint replaces_id, const QString &app_icon, 
		const QString &summary, const QString &body, const QStringList &actions,
	       	const QVariantMap &hints, int expire_timeout)
{

	return widget->Notify(app_name, replaces_id, app_icon, summary, body, actions, hints, expire_timeout);
}


////////////////////////
// Notify Widget
////////////////////////


NotifyWidget::NotifyWidget(BartWidget *parent)
:QWidget(parent), version(1)
{
	setupUi(this);
	label->setText("");
	content->setText("");

	NotifyWidgetAdaptor *ad = new NotifyWidgetAdaptor(this);
	QDBusConnection::sessionBus().registerObject("/org/freedesktop/Notifications", this);
	QDBusConnection::sessionBus().registerService("org.freedesktop.Notifications");
}


// Slots

QStringList NotifyWidget::GetCapabilities() 
{
	return QStringList();
}

void NotifyWidget::CloseNotification(quint32 id)
{
	if ( id == version ) {
		label->setText("");
		content->setText("");
	}
}

int NotifyWidget::Notify(const QString& app, uint replace, const QString& icon, 
		const QString& summary, const QString& body,
		const QStringList& actions, const QMap<QString, QVariant> &hints,
		int timeout) {

	label->setText(app);
	content->setText(summary);
	
	qDebug() << app << summary << body << icon << hints;

	if (timeout) {
		QTimer::singleShot(timeout, this, SLOT(clear()));
	}

	return version.fetchAndStoreRelaxed(1);
}

void NotifyWidget::clear()
{
	label->setText("");
	content->setText("");
}



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

	qDebug() << app << summary << body << icon << hints;

	// Technically we don't support formatting tags
	// but some clients are too lazy to check
	label->setText(QString(app).replace("<br/>", " "));

	// For some reasone several apps enjoy passing
	// over an empty body while other just give the
	// app name as summary
	if ( !body.isEmpty() ) {
		content->setText(body);
	} else {
		content->setText(summary);
	}

	if (timeout > 0) {
		// We dont respect the argument as passed over DBus
		// but we will remove the notification ... eventually

		connect(parent(), SIGNAL(timeout()),
				this, SLOT(clear()));

		clearCount = 2; // Wait 2 cycles before removing notification
	}

	return version.fetchAndStoreRelaxed(1);
}

void NotifyWidget::clear()
{
	clearCount--;
	if ( !clearCount ) {
		label->setText("");
		content->setText("");

		disconnect(parent(), SIGNAL(timeout()),
				this, SLOT(clear()));
	}
}



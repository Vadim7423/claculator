#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QSharedPointer>
#include "settings.h"
#include "queue_requests.h"
#include "queue_results.h"

class Calculator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString request READ request WRITE setRequest NOTIFY requestChanged FINAL)
    Q_PROPERTY(QueueRequests* reqsQueue READ reqsQueue CONSTANT FINAL)
    Q_PROPERTY(QueueResults* results READ results CONSTANT FINAL)
    Q_PROPERTY(Settings* settings READ settings CONSTANT FINAL)
public:
    explicit Calculator(QObject *parent = nullptr);

    static Calculator& instance();

private:
    const QString& request() const;
    void setRequest(const QString& value);
    void processString(const QString &value);
    QueueRequests* reqsQueue();
    QueueResults* results();
    Settings* settings();

    Settings* m_settings = nullptr;
    QString m_request_str;
    QSharedPointer<QueueRequests> m_requests;
    QSharedPointer<QueueResults> m_results;

    static QSharedPointer<Calculator> calc_instance;

signals:
    void requestChanged();
};

#endif // CALCULATOR_H

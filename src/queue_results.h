#ifndef QUEUE_RESULTS_H
#define QUEUE_RESULTS_H

#include <QObject>

class QueueResults : public QObject
{
    Q_OBJECT
public:
    explicit QueueResults(QObject *parent = nullptr);

public slots:
    void addResult(double result);
    void addError(QString value);

private:
    QThread* m_workerThread = nullptr;
    QVector<QString> m_data;

signals:
    void sizeChanged(uint8_t size);
    void success(QString value);
    void error(QString value);
};

#endif // QUEUE_RESULTS_H

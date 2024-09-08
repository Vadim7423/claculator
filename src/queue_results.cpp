#include "queue_results.h"
#include <QThread>
#include <QQmlEngine>

QueueResults::QueueResults(QObject *parent)
    : QObject{parent}
    , m_workerThread(new QThread)
{
    moveToThread(m_workerThread);
    connect(this, &QObject::destroyed, m_workerThread, &QThread::quit);
    connect(m_workerThread, &QThread::finished,
            m_workerThread, &QThread::deleteLater);
    m_workerThread->start();
     QJSEngine::setObjectOwnership(this, QJSEngine::ObjectOwnership::CppOwnership);
}

void QueueResults::addResult(double result)
{
    m_data.push_back(QString::number(result));
    emit success(m_data.back());
    emit sizeChanged(m_data.size());
}

void QueueResults::addError(QString value)
{
    m_data.push_back(value);
    emit error(m_data.back());
    emit sizeChanged(m_data.size());
}

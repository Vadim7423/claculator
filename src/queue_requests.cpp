#include "queue_requests.h"
#include <QThread>
#include <QDebug>
#include <QQmlEngine>

QueueRequests::QueueRequests(const Settings& settings, QObject *parent)
    : QObject{parent}
    , m_workerThread(new QThread)
    , m_timer(new QTimer)
    , m_settings(settings)
{
    m_timer->moveToThread(m_workerThread);
    moveToThread(m_workerThread);
    connect(this, &QObject::destroyed, m_workerThread, &QThread::quit);
    connect(m_workerThread, &QThread::finished,
            m_workerThread, &QThread::deleteLater);
    connect(m_workerThread, &QThread::finished,
            m_timer, &QTimer::deleteLater);
    connect(m_timer, &QTimer::timeout,
            this, &QueueRequests::processQueue);
    m_workerThread->start();

    QJSEngine::setObjectOwnership(this, QJSEngine::ObjectOwnership::CppOwnership);
}

void QueueRequests::addRequest(const Request &req)
{
    QMetaObject::invokeMethod(this,
                              "_addRequest",
                              Qt::ConnectionType::QueuedConnection,
                              Q_ARG(const Request&, req)
                              );
}

QString QueueRequests::getRequestString(const Request &req) const
{
    QString result;
    auto iter = req.begin();

    while (iter != req.end()) {
        result += iter->data()->strView();
        ++iter;
    }

    return result;
}

void QueueRequests::processQueue()
{
    if(m_queue.isEmpty() || is_busy)
        return;

    is_busy = true;
    QTimer::singleShot(m_settings.duration() * 1000, this, &QueueRequests::processExpression);
}

void QueueRequests::processExpression()
{
    Request req = m_queue.dequeue();
    emit requestAdopted(getRequestString(req));
    QVector<QSharedPointer<Token>> expression;

    while(req.size()) {
        expression.push_back(req.front());
        req.pop_front();
        if(expression.size() == 3) {
            std::optional<double> result;
            Operator* operation = static_cast<Operator*>(expression[1].get());
            Operand* left = static_cast<Operand*>(expression[0].get());
            Operand* right = static_cast<Operand*>(expression[2].get());
            result = getResult(operation->type(), left->value(), right->value());
            // Тут можно использовать другую функцию вместо getResult
            // в том числе функцию внешней библиотеки.
            // На самом деле не до конца понятен этот момент.
            QSharedPointer<Token> new_token = QSharedPointer<Token>(new Operand(result.value()));
            req.push_front(new_token);
            expression.clear();

            if(req.size() == 1) {
                emit resultProcessed(result.value());
                break;
            }
        }
    }

    is_busy = false;
    emit sizeChanged(m_queue.size());
}

double QueueRequests::getResult(Operator::Type operation, double left, double right)
{
    double result = 0;
    switch (operation) {
    case Operator::Type::Addition:
        result = left + right;
        break;
    case Operator::Type::Subtraction:
        result = left - right;
        break;
    case Operator::Type::Multiplication:
        result = left * right;
        break;
    case Operator::Type::Division:
        result = left / right;
        break;
    default:
        break;
    }
    return result;
}

void QueueRequests::_addRequest(const Request &req)
{
    m_queue.append(req);
    if(!m_timer->isActive())
        m_timer->start(1);
    emit sizeChanged(m_queue.size());
}

Token::Token(const QString &str)
    : m_str(str)
{

}

Token::~Token()
{

}

bool Token::isOperator() const
{
    return is_operator;
}

const QString &Token::strView()
{
    return m_str;
}

QHash<QString, Operator::Type> Operator::assoc = {
    {"+", Type::Addition},
    {"-", Type::Subtraction},
    {"*", Type::Multiplication},
    {"/", Type::Division}
};

Operator::Operator(const QString &str)
    : Token(str)
{
    is_operator = true;
    if(assoc.contains(str)) {
        m_type = assoc.value(str);
    }
}

Operator::Type Operator::type() const
{
    return m_type;
}

void Operator::Print()
{
    qDebug() << assoc.key(m_type);
}

Operand::Operand(const QString &str)
    : Token(str)
{
    m_value = str.toDouble();
}

Operand::Operand(double val)
    : Token(QString::number(val))
{
    m_value = val;
}

double Operand::value() const
{
    return m_value;
}

void Operand::Print()
{
    qDebug() << m_value;
}

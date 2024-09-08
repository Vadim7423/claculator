#include "calculator.h"
#include <QHash>
#include <QtConcurrent/QtConcurrent>
#include <QQmlEngine>
#include <set>

QSharedPointer<Calculator> Calculator::calc_instance;

Calculator::Calculator(QObject *parent)
    : QObject{parent}
    , m_settings(new Settings(this))
    , m_requests(QSharedPointer<QueueRequests>(new QueueRequests(*m_settings)))
    , m_results(QSharedPointer<QueueResults>(new QueueResults))
{
    qmlRegisterType<QueueRequests> ("App", 1, 0, "QueueRequests");
    qmlRegisterType<QueueResults> ("App", 1, 0, "QueueResults");
    qmlRegisterType<Settings> ("App", 1, 0, "Settings");
    connect(m_requests.get(), &QueueRequests::resultProcessed,
            m_results.get(), &QueueResults::addResult);
    connect(m_requests.get(), &QueueRequests::errorProcessed,
            m_results.get(), &QueueResults::addError);
}

Calculator &Calculator::instance()
{
    if(!calc_instance.get()) {
        calc_instance = QSharedPointer<Calculator>(new Calculator);
    }

    return *calc_instance;
}

const QString &Calculator::request() const
{
    return m_request_str;
}

void Calculator::setRequest(const QString &value)
{
    m_request_str = value;

    auto calcFunc = QtConcurrent::run([&] () {
        processString(value);
    });

    emit requestChanged();
}

void Calculator::processString(const QString &value)
{
    if(value.isEmpty())
        return;

    std::deque<QSharedPointer<Token>> request;
    int pos = 0;
    QString tmp = "";
    static std::set<QString> operators = {"+", "-", "*", "/"};
    static std::set<QString> operands = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "."};

    if(value.front() == '+' || value.front() == '-') {
        request.push_back(QSharedPointer<Token>(new Operand("0")));
    }

    while(pos < m_request_str.size()) {
        if(operators.contains(m_request_str[pos])) {
            if(!tmp.isEmpty()) {
                if(request.size() && request.back()->strView() == "/" && !tmp.toFloat()) {
                    emit m_requests->requestAdopted(value);
                    emit m_requests->errorProcessed("Деление на 0");
                    return;
                }

                request.push_back(QSharedPointer<Token>(new Operand(tmp)));
                tmp.clear();
            }

            if(request.size() && request.back()->isOperator()) {
                emit m_requests->requestAdopted(value);
                emit m_requests->errorProcessed("Несколько опереторов подряд");
                return;
            }

            request.push_back(QSharedPointer<Token>(new Operator(m_request_str[pos])));
        } else if(operands.contains(m_request_str[pos])) {
            tmp += m_request_str[pos];
        } else {
            emit m_requests->requestAdopted(value);
            emit m_requests->errorProcessed("Ошибочный символ в запросе");
            return;
        }

        ++pos;
    }

    if(!tmp.isEmpty()) {
        if(request.size() && request.back()->strView() == "/" && !tmp.toFloat()) {
            emit m_requests->requestAdopted(value);
            emit m_requests->errorProcessed("Деление на 0");
            return;
        }
        request.push_back(QSharedPointer<Token>(new Operand(tmp)));
        tmp.clear();
    }

    if(request.size() < 3 || request.back()->isOperator()) {
        emit m_requests->requestAdopted(value);
        emit m_requests->errorProcessed("Выражение неполное");
    } else {
        m_requests->addRequest(request);
    }

}

QueueRequests *Calculator::reqsQueue()
{
    return m_requests.get();
}

QueueResults *Calculator::results()
{
    return m_results.get();
}

Settings *Calculator::settings()
{
    return m_settings;
}

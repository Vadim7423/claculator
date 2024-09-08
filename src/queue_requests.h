#ifndef QUEUE_REQUESTS_H
#define QUEUE_REQUESTS_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <deque>
#include "settings.h"

class Token {
public:
    explicit Token(const QString& str);
    virtual ~Token();
    virtual void Print() = 0;
    bool isOperator() const;
    const QString& strView();

protected:
    bool is_operator = false;
    QString m_str;
};

class Operator : public Token {
public:
    enum class Type {
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Undefined
    };

    explicit Operator(const QString& str);

    Type type() const;
    void Print() override;

private:
    Type m_type = Type::Undefined;
    static QHash<QString, Type> assoc;
};

class Operand : public Token {
public:
    explicit Operand(const QString& str);
    explicit Operand(double val);
    double value() const;
    void Print() override;

private:
    double m_value;
};

class QueueRequests : public QObject
{
    Q_OBJECT
    using Request = std::deque<QSharedPointer<Token>>;
public:
    explicit QueueRequests(const Settings& settings, QObject *parent = nullptr);
    void addRequest(const Request& req);
    QString getRequestString(const Request& req) const;

private:
    void processQueue();
    void processExpression();
    double getResult(Operator::Type operation, double left, double right);
    Q_INVOKABLE void _addRequest(const Request& req);
    QThread* m_workerThread = nullptr;
    QTimer* m_timer = nullptr;
    QQueue<Request> m_queue;
    const Settings& m_settings;
    bool is_busy = false;

signals:
    void sizeChanged(uint8_t size);
    void requestAdopted(QString request);
    void resultProcessed(double value);
    void errorProcessed(QString value);

};

#endif // QUEUE_REQUESTS_H

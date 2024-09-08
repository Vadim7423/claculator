#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int leftPos READ leftPos WRITE setLeftPos NOTIFY leftPosChanged FINAL)
    Q_PROPERTY(int topPos READ topPos WRITE setTopPos NOTIFY topPosChanged FINAL)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged FINAL)
    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged FINAL)
public:
    explicit Settings(QObject *parent = nullptr);
    int duration() const;

private:
    int leftPos() const;
    void setLeftPos(int pos);
    int topPos() const;
    void setTopPos(int pos);
    int width() const;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);
    void setDuration(int val);
    QSettings m_settings;

signals:
    void leftPosChanged();
    void topPosChanged();
    void widthChanged();
    void heightChanged();
    void durationChanged();
};

#endif // SETTINGS_H

#include "settings.h"
#include <QQmlEngine>
#include <QGuiApplication>

Settings::Settings(QObject *parent)
    : QObject{parent}
    , m_settings(QGuiApplication::organizationName(),
                 QGuiApplication::applicationName())
{
    QJSEngine::setObjectOwnership(this, QJSEngine::ObjectOwnership::CppOwnership);
}

int Settings::leftPos() const
{
    return m_settings.value("left_pos", 100).toInt();
}

void Settings::setLeftPos(int pos)
{
    m_settings.setValue("left_pos", pos);
    emit leftPosChanged();
}

int Settings::topPos() const
{
    return m_settings.value("top_pos", 100).toInt();
}

void Settings::setTopPos(int pos)
{
    m_settings.setValue("top_pos", pos);
    emit topPosChanged();
}

int Settings::width() const
{
    return m_settings.value("width", 400).toInt();
}

void Settings::setWidth(int width)
{
    m_settings.setValue("width", width);
    emit widthChanged();
}

int Settings::height() const
{
    return m_settings.value("height", 580).toInt();
}

void Settings::setHeight(int height)
{
    m_settings.setValue("height", height);
    emit heightChanged();
}

int Settings::duration() const
{
    return m_settings.value("duration", 1).toInt();
}

void Settings::setDuration(int val)
{
    m_settings.setValue("duration", val);
    emit durationChanged();
}

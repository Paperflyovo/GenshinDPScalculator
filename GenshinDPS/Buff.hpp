#ifndef BUFF_HPP
#define BUFF_HPP

#include <QString>
#include <QJsonObject>

class Buff {
public:
    Buff() = default;
    Buff(const QString& name, const QString& type, double value, bool isPercentage = true)
        : m_name(name), m_type(type), m_value(value), m_isPercentage(isPercentage) {}

    QString name() const { return m_name; }
    void setName(const QString& n) { m_name = n; }

    QString type() const { return m_type; }
    double value() const { return m_value; }
    bool isPercentage() const { return m_isPercentage; }

    QJsonObject toJson() const;
    static Buff fromJson(const QJsonObject& obj);

private:
    QString m_name;
    QString m_type;
    double m_value = 0.0;
    bool m_isPercentage = true;
};

#endif

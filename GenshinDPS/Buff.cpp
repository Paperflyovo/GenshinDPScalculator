#include "Buff.hpp"

QJsonObject Buff::toJson() const
{
    QJsonObject obj;
    obj["name"] = m_name;
    obj["type"] = m_type;
    obj["value"] = m_value;
    obj["isPercentage"] = m_isPercentage;
    return obj;
}

Buff Buff::fromJson(const QJsonObject& obj)
{
    Buff b;
    b.m_name = obj["name"].toString();
    b.m_type = obj["type"].toString();
    b.m_value = obj["value"].toDouble();
    b.m_isPercentage = obj["isPercentage"].toBool(true);
    return b;
}

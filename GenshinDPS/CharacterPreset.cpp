#include "CharacterPreset.hpp"

QJsonObject CharacterPreset::toJson() const
{
    QJsonObject obj;
    obj["name"] = m_name;

    QJsonObject stats;
    stats["maxHP"] = m_baseStats.maxHP;
    stats["attack"] = m_baseStats.attack;
    stats["defense"] = m_baseStats.defense;
    stats["elementalMastery"] = m_baseStats.elementalMastery;
    stats["energyRecharge"] = m_baseStats.energyRecharge;
    stats["hydroBonus"] = m_baseStats.hydroBonus;
    stats["pyroBonus"] = m_baseStats.pyroBonus;
    stats["cryoBonus"] = m_baseStats.cryoBonus;
    stats["electroBonus"] = m_baseStats.electroBonus;
    stats["anemoBonus"] = m_baseStats.anemoBonus;
    stats["geoBonus"] = m_baseStats.geoBonus;
    stats["dendroBonus"] = m_baseStats.dendroBonus;
    stats["physicalBonus"] = m_baseStats.physicalBonus;
    stats["critRate"] = m_baseStats.critRate;
    stats["critDamage"] = m_baseStats.critDamage;
    obj["baseStats"] = stats;

    QJsonArray skillsArr;
    for (const auto& s : m_skills) {
        QJsonObject skill;
        skill["name"] = s.name;
        skill["type"] = static_cast<int>(s.type);
        skill["multiplier"] = s.multiplier;
        skill["actionTime"] = s.actionTime;
        skillsArr.append(skill);
    }
    obj["skills"] = skillsArr;

    return obj;
}

CharacterPreset CharacterPreset::fromJson(const QJsonObject& obj)
{
    CharacterPreset preset(obj["name"].toString());

    QJsonObject stats = obj["baseStats"].toObject();
    CharacterBaseStats b;
    b.maxHP = stats["maxHP"].toDouble();
    b.attack = stats["attack"].toDouble();
    b.defense = stats["defense"].toDouble();
    b.elementalMastery = stats["elementalMastery"].toDouble();
    b.energyRecharge = stats["energyRecharge"].toDouble();
    b.hydroBonus = stats["hydroBonus"].toDouble();
    b.pyroBonus = stats["pyroBonus"].toDouble();
    b.cryoBonus = stats["cryoBonus"].toDouble();
    b.electroBonus = stats["electroBonus"].toDouble();
    b.anemoBonus = stats["anemoBonus"].toDouble();
    b.geoBonus = stats["geoBonus"].toDouble();
    b.dendroBonus = stats["dendroBonus"].toDouble();
    b.physicalBonus = stats["physicalBonus"].toDouble();
    b.critRate = stats["critRate"].toDouble(5.0);
    b.critDamage = stats["critDamage"].toDouble(50.0);
    preset.setBaseStats(b);

    QJsonArray skillsArr = obj["skills"].toArray();
    QVector<SkillAction> skills;
    for (const auto& val : skillsArr) {
        QJsonObject s = val.toObject();
        SkillAction a;
        a.name = s["name"].toString();
        a.type = static_cast<SkillType>(s["type"].toInt());
        a.multiplier = s["multiplier"].toDouble();
        a.actionTime = s["actionTime"].toDouble();
        skills.append(a);
    }
    preset.setSkills(skills);

    return preset;
}

#ifndef CHARACTERPRESET_HPP
#define CHARACTERPRESET_HPP

#include <QString>
#include <QMap>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

// 技能类型
enum class SkillType {
    NormalAttack,
    ChargedAttack,
    ElementalSkill,
    ElementalBurst,
    Other
};

// 单个技能动作定义
struct SkillAction {
    QString name;
    SkillType type = SkillType::NormalAttack;
    double multiplier = 1.0;
    double actionTime = 1.0;
};

// 角色基础属性
struct CharacterBaseStats {
    double maxHP = 0.0;
    double attack = 0.0;
    double defense = 0.0;
    double elementalMastery = 0.0;
    double energyRecharge = 0.0;
    double hydroBonus  = 0.0;
    double pyroBonus   = 0.0;
    double cryoBonus   = 0.0;
    double electroBonus= 0.0;
    double anemoBonus  = 0.0;
    double geoBonus    = 0.0;
    double dendroBonus  = 0.0;
    double physicalBonus= 0.0;
    double critRate = 5.0;       // 暴击率 (%)
    double critDamage = 50.0;    // 暴击伤害加成 (%)
};

class CharacterPreset {
public:
    CharacterPreset() = default;
    CharacterPreset(const QString& name) : m_name(name) {}

    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    CharacterBaseStats baseStats() const { return m_baseStats; }
    void setBaseStats(const CharacterBaseStats& stats) { m_baseStats = stats; }

    QVector<SkillAction> skills() const { return m_skills; }
    void setSkills(const QVector<SkillAction>& skills) { m_skills = skills; }
    void addSkill(const SkillAction& skill) { m_skills.append(skill); }
    void removeSkill(int index) { if(index>=0 && index<m_skills.size()) m_skills.removeAt(index); }

    QJsonObject toJson() const;
    static CharacterPreset fromJson(const QJsonObject& obj);

private:
    QString m_name;
    CharacterBaseStats m_baseStats;
    QVector<SkillAction> m_skills;
};

#endif

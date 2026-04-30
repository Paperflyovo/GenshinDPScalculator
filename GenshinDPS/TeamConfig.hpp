#ifndef TEAMCONFIG_HPP
#define TEAMCONFIG_HPP

#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

struct TeamBuff {
    QString sourceCharacter;
    QString targetCharacter;
    QString buffType;
    double value = 0.0;
    bool isPercentage = true;
};

struct SkillSequenceItem {
    QString characterName;
    int skillIndex = 0;
    QString reactionTag;
};

class TeamConfig {
public:
    TeamConfig() = default;
    explicit TeamConfig(const QString& name) : m_name(name) {}

    QString name() const { return m_name; }
    void setName(const QString& n) { m_name = n; }

    QVector<QString> members() const { return m_members; }
    void setMembers(const QVector<QString>& members) { m_members = members; }
    void addMember(const QString& name) { if(m_members.size()<4) m_members.append(name); }
    void removeMember(int idx) { if(idx>=0 && idx<m_members.size()) m_members.removeAt(idx); }

    QVector<SkillSequenceItem> skillSequence() const { return m_skillSeq; }
    void setSkillSequence(const QVector<SkillSequenceItem>& seq) { m_skillSeq = seq; }
    void addSkillStep(const SkillSequenceItem& step) { m_skillSeq.append(step); }
    void clearSkillSequence() { m_skillSeq.clear(); }

    QVector<TeamBuff> buffs() const { return m_buffs; }
    void setBuffs(const QVector<TeamBuff>& buffs) { m_buffs = buffs; }
    void addBuff(const TeamBuff& buff) { m_buffs.append(buff); }
    void removeBuff(int idx) { if(idx>=0 && idx<m_buffs.size()) m_buffs.removeAt(idx); }

    QJsonObject toJson() const;
    static TeamConfig fromJson(const QJsonObject& obj);

private:
    QString m_name;
    QVector<QString> m_members;
    QVector<SkillSequenceItem> m_skillSeq;
    QVector<TeamBuff> m_buffs;
};

#endif

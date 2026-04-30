#include "TeamConfig.hpp"

QJsonObject TeamConfig::toJson() const
{
    QJsonObject obj;
    obj["name"] = m_name;

    QJsonArray membersArr;
    for (const auto& m : m_members)
        membersArr.append(m);
    obj["members"] = membersArr;

    QJsonArray seqArr;
    for (const auto& step : m_skillSeq) {
        QJsonObject s;
        s["characterName"] = step.characterName;
        s["skillIndex"] = step.skillIndex;
        s["reactionTag"] = step.reactionTag;
        seqArr.append(s);
    }
    obj["skillSequence"] = seqArr;

    QJsonArray buffsArr;
    for (const auto& b : m_buffs) {
        QJsonObject bo;
        bo["sourceCharacter"] = b.sourceCharacter;
        bo["targetCharacter"] = b.targetCharacter;
        bo["buffType"] = b.buffType;
        bo["value"] = b.value;
        bo["isPercentage"] = b.isPercentage;
        buffsArr.append(bo);
    }
    obj["buffs"] = buffsArr;

    return obj;
}

TeamConfig TeamConfig::fromJson(const QJsonObject& obj)
{
    TeamConfig tc(obj["name"].toString());

    QJsonArray membersArr = obj["members"].toArray();
    QVector<QString> members;
    for (const auto& m : membersArr)
        members.append(m.toString());
    tc.setMembers(members);

    QJsonArray seqArr = obj["skillSequence"].toArray();
    QVector<SkillSequenceItem> seq;
    for (const auto& val : seqArr) {
        QJsonObject s = val.toObject();
        SkillSequenceItem item;
        item.characterName = s["characterName"].toString();
        item.skillIndex = s["skillIndex"].toInt();
        item.reactionTag = s["reactionTag"].toString();
        seq.append(item);
    }
    tc.setSkillSequence(seq);

    QJsonArray buffsArr = obj["buffs"].toArray();
    QVector<TeamBuff> buffs;
    for (const auto& val : buffsArr) {
        QJsonObject b = val.toObject();
        TeamBuff tb;
        tb.sourceCharacter = b["sourceCharacter"].toString();
        tb.targetCharacter = b["targetCharacter"].toString();
        tb.buffType = b["buffType"].toString();
        tb.value = b["value"].toDouble();
        tb.isPercentage = b["isPercentage"].toBool(true);
        buffs.append(tb);
    }
    tc.setBuffs(buffs);

    return tc;
}

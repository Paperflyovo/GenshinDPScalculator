#include "DPSEngine.hpp"
#include <QMap>
#include <QDebug>

DPSResult DPSEngine::calculate(const TeamConfig& team,
                               const QVector<CharacterPreset>& characterPool,
                               const ReactionRuleSet& reactionRules)
{
    DPSResult result;
    double totalDamage = 0.0, currentTime = 0.0;
    QMap<QString, const CharacterPreset*> charMap;
    for (const auto& c : characterPool) charMap[c.name()] = &c;

    for (const SkillSequenceItem& step : team.skillSequence()) {
        if (!charMap.contains(step.characterName)) continue;
        const CharacterPreset* preset = charMap[step.characterName];
        if (step.skillIndex < 0 || step.skillIndex >= preset->skills().size()) continue;

        const SkillAction& skill = preset->skills()[step.skillIndex];
        const CharacterBaseStats& stats = preset->baseStats();

        // 根据主属性取基础数值
        double baseAttr = stats.attack;
        if (skill.baseAttribute == "defense") baseAttr = stats.defense;
        else if (skill.baseAttribute == "maxHP") baseAttr = stats.maxHP;

        double baseDamage = baseAttr * (skill.multiplier / 100.0);

        // 暴击期望
        double critRate = stats.critRate / 100.0;
        double critDamage = stats.critDamage / 100.0;
        double expectedCritMult = 1.0 + critRate * critDamage;
        baseDamage *= expectedCritMult;

        // 配队 Buff (简单处理)
        double buffMultiplier = 1.0;
        for (const TeamBuff& tb : team.buffs()) {
            if (tb.targetCharacter.isEmpty() || tb.targetCharacter == step.characterName) {
                if (tb.buffType == "allDamage") {
                    if (tb.isPercentage) buffMultiplier *= (1.0 + tb.value / 100.0);
                    else buffMultiplier += tb.value;
                }
                // 可扩展其他类型...
            }
        }

        double reactionMult = 1.0;
        if (!step.reactionTag.isEmpty()) {
            const auto& reactions = reactionRules.reactions();
            for (const AmplifyingReaction& r : reactions) {
                if (r.name == step.reactionTag) {
                    reactionMult = r.damageMultiplier;
                    break;
                }
            }
        }

        double finalDamage = baseDamage * reactionMult * buffMultiplier;

        DamageStep ds;
        ds.characterName = step.characterName;
        ds.skillName = skill.name;
        ds.rawDamage = baseDamage;
        ds.reactionMultiplier = reactionMult;
        ds.buffMultiplier = buffMultiplier;
        ds.finalDamage = finalDamage;
        ds.actionTime = skill.actionTime;   // 直接使用技能预设中的动作时间
        result.steps.append(ds);

        totalDamage += finalDamage;
        currentTime += skill.actionTime;
    }

    result.totalDamage = totalDamage;
    result.totalTime = currentTime;
    result.dps = (currentTime > 0) ? (totalDamage / currentTime) : 0.0;
    return result;
}

#include "DPSEngine.hpp"
#include <QMap>
#include <QDebug>
#include <cmath>

DPSResult DPSEngine::calculate(const TeamConfig& team,
                               QVector<CharacterPreset> characterPool,
                               const ReactionRuleSet& reactionRules,
                               int monsterLevel)
{
    DPSResult result;
    double totalDamage = 0.0, currentTime = 0.0;

    // 汇总减防效果
    double totalDefReduction = 0.0;
    for (const TeamBuff& tb : team.buffs()) {
        if (tb.buffType == QStringLiteral("减防%"))
            totalDefReduction += tb.value;
    }
    double defReductionFactor = totalDefReduction / 100.0;
    if (defReductionFactor < 0.0) defReductionFactor = 0.0;
    if (defReductionFactor >= 1.0) defReductionFactor = 0.999;

    // 预先建立名字到索引的快速查找（避免每次遍历）
    QMap<QString, int> nameIndex;
    for (int i = 0; i < characterPool.size(); ++i)
        nameIndex[characterPool[i].name()] = i;

    for (const SkillSequenceItem& step : team.skillSequence()) {
        // 查找角色
        auto it = nameIndex.find(step.characterName);
        if (it == nameIndex.end()) continue;          // 角色不存在
        const CharacterPreset& preset = characterPool[it.value()];
        const auto& skills = preset.skills();
        if (step.skillIndex < 0 || step.skillIndex >= skills.size()) continue; // 技能索引无效

        const SkillAction& skill = skills.at(step.skillIndex);
        const CharacterBaseStats& stats = preset.baseStats();
        int charLevel = (stats.characterLevel > 0) ? stats.characterLevel : 90;

        // 基础属性（攻击/防御/生命）
        QString attr = skill.baseAttribute.isEmpty() ? QStringLiteral("attack") : skill.baseAttribute;
        double baseAttr = stats.attack;
        if (attr == QStringLiteral("defense"))
            baseAttr = stats.defense;
        else if (attr == QStringLiteral("maxHP"))
            baseAttr = stats.maxHP;

        double baseDamage = baseAttr * (skill.multiplier / 100.0);

        // 元素增伤
        double elemBonus = 0.0;
        QString elem = skill.damageElement.isEmpty() ? QStringLiteral("Physical") : skill.damageElement;
        if (elem == QStringLiteral("Hydro")) elemBonus = stats.hydroBonus;
        else if (elem == QStringLiteral("Pyro")) elemBonus = stats.pyroBonus;
        else if (elem == QStringLiteral("Cryo")) elemBonus = stats.cryoBonus;
        else if (elem == QStringLiteral("Electro")) elemBonus = stats.electroBonus;
        else if (elem == QStringLiteral("Anemo")) elemBonus = stats.anemoBonus;
        else if (elem == QStringLiteral("Geo")) elemBonus = stats.geoBonus;
        else if (elem == QStringLiteral("Dendro")) elemBonus = stats.dendroBonus;
        else if (elem == QStringLiteral("Physical")) elemBonus = stats.physicalBonus;
        baseDamage *= (1.0 + elemBonus / 100.0);

        // 暴击期望
        double critRate = stats.critRate / 100.0;
        double critDamage = stats.critDamage / 100.0;
        baseDamage *= (1.0 + critRate * critDamage);

        // 防御乘区
        double defRatio = (charLevel + 100.0) /
                          ((charLevel + monsterLevel + 200.0) * (1.0 - defReductionFactor));
        double ignoreDef = step.ignoreDefensePercent / 100.0;
        if (ignoreDef > 1.0) ignoreDef = 1.0;
        double finalDefMultiplier = 1.0 - (1.0 - defRatio) * (1.0 - ignoreDef);
        baseDamage *= finalDefMultiplier;

        // 配队 Buff（仅示例，可扩展）
        double buffMultiplier = 1.0;
        double bonusFlat = 0.0;
        for (const TeamBuff& tb : team.buffs()) {
            if (tb.targetCharacter.isEmpty() || tb.targetCharacter == step.characterName) {
                const QString& type = tb.buffType;
                if (tb.isPercentage) {
                    if (type == QStringLiteral("全伤害加成%") || type == QStringLiteral("攻击力%") ||
                        type == QStringLiteral("防御力%") || type == QStringLiteral("生命上限%"))
                        buffMultiplier *= (1.0 + tb.value / 100.0);
                } else {
                    if (type == QStringLiteral("小攻击") || type == QStringLiteral("小防御") || type == QStringLiteral("小生命"))
                        bonusFlat += tb.value;
                }
            }
        }
        baseDamage = (baseDamage + bonusFlat) * buffMultiplier;

        // 反应
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

        double finalDamage = baseDamage * reactionMult;

        DamageStep ds;
        ds.characterName = step.characterName;
        ds.skillName = skill.name;
        ds.rawDamage = baseDamage;
        ds.reactionMultiplier = reactionMult;
        ds.buffMultiplier = buffMultiplier;
        ds.finalDamage = finalDamage;
        ds.actionTime = skill.actionTime;
        result.steps.append(ds);

        totalDamage += finalDamage;
        currentTime += skill.actionTime;
    }

    result.totalDamage = totalDamage;
    result.totalTime = currentTime;
    result.dps = (currentTime > 0) ? (totalDamage / currentTime) : 0.0;
    return result;
}

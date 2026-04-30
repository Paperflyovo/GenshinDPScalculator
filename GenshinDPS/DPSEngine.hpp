#ifndef DPSENGINE_HPP
#define DPSENGINE_HPP

#include <QVector>
#include "TeamConfig.hpp"
#include "CharacterPreset.hpp"
#include "ReactionRule.hpp"

struct DamageStep {
    QString characterName;
    QString skillName;
    double rawDamage = 0.0;
    double reactionMultiplier = 1.0;
    double buffMultiplier = 1.0;
    double finalDamage = 0.0;
    double actionTime = 0.0;
};

struct DPSResult {
    double totalDamage = 0.0;
    double totalTime = 0.0;
    double dps = 0.0;
    QVector<DamageStep> steps;
};

class DPSEngine {
public:
    DPSEngine() = default;
    // characterPool 改为按值传递，计算期间不受外部影响
    DPSResult calculate(const TeamConfig& team,
                        QVector<CharacterPreset> characterPool,
                        const ReactionRuleSet& reactionRules,
                        int monsterLevel);
};

#endif

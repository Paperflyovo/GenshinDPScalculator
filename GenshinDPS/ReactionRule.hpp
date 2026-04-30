#ifndef REACTIONRULE_HPP
#define REACTIONRULE_HPP

#include <QString>
#include <QVector>
#include <QPair>

struct AmplifyingReaction {
    QString name;
    QString triggerElement;
    QString auraElement;
    double damageMultiplier = 1.5;
};

class ReactionRuleSet {
public:
    ReactionRuleSet();

    const QVector<AmplifyingReaction>& reactions() const { return m_reactions; }
    double getMultiplier(const QString& trigger, const QString& aura) const;

private:
    QVector<AmplifyingReaction> m_reactions;
};

#endif

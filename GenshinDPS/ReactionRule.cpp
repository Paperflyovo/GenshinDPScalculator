#include "ReactionRule.hpp"

ReactionRuleSet::ReactionRuleSet()
{
    m_reactions.append({"Vaporize_FireOnWater", "Pyro", "Hydro", 2.0});
    m_reactions.append({"Vaporize_WaterOnFire", "Hydro", "Pyro", 1.5});
    m_reactions.append({"Melt_FireOnIce", "Pyro", "Cryo", 2.0});
    m_reactions.append({"Melt_IceOnFire", "Cryo", "Pyro", 1.5});
}

double ReactionRuleSet::getMultiplier(const QString& trigger, const QString& aura) const
{
    for (const auto& r : m_reactions) {
        if (r.triggerElement == trigger && r.auraElement == aura)
            return r.damageMultiplier;
    }
    return 1.0;
}

#ifndef DATAMODEL_HPP
#define DATAMODEL_HPP

#include <QObject>
#include <QVector>
#include <QString>
#include "CharacterPreset.hpp"
#include "TeamConfig.hpp"
#include "ReactionRule.hpp"
#include "Buff.hpp"

class DataModel : public QObject
{
    Q_OBJECT
public:
    static DataModel& instance();

    const QVector<CharacterPreset>& characterPresets() const;
    void setCharacterPresets(const QVector<CharacterPreset>& presets);
    void addCharacterPreset(const CharacterPreset& preset);
    void updateCharacterPreset(int index, const CharacterPreset& preset);
    void removeCharacterPreset(int index);
    CharacterPreset* findCharacterPreset(const QString& name);

    const QVector<TeamConfig>& teams() const;
    void setTeams(const QVector<TeamConfig>& teams);
    void addTeam(const TeamConfig& team);
    void updateTeam(int index, const TeamConfig& team);
    void removeTeam(int index);

    const QVector<Buff>& buffTemplates() const;
    void setBuffTemplates(const QVector<Buff>& buffs);
    void addBuffTemplate(const Buff& buff);
    void removeBuffTemplate(int index);

    const ReactionRuleSet& reactionRules() const;

    int monsterLevel() const;
    void setMonsterLevel(int level);

    void loadFromJson();
    void saveToJson() const;

signals:
    void characterPresetsChanged();
    void teamsChanged();
    void buffsChanged();
    void reactionsChanged();
    void monsterLevelChanged();

private:
    DataModel();
    ~DataModel() override = default;
    DataModel(const DataModel&) = delete;
    DataModel& operator=(const DataModel&) = delete;

    QVector<CharacterPreset> m_characterPresets;
    QVector<TeamConfig> m_teams;
    QVector<Buff> m_buffTemplates;
    ReactionRuleSet m_reactionRules;
    int m_monsterLevel = 110;
};

#endif

#include "DataModel.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

DataModel& DataModel::instance()
{
    static DataModel inst;
    return inst;
}

DataModel::DataModel() : QObject(nullptr)
{
    m_reactionRules = ReactionRuleSet();
    loadFromJson();
}

const QVector<CharacterPreset>& DataModel::characterPresets() const { return m_characterPresets; }
void DataModel::setCharacterPresets(const QVector<CharacterPreset>& presets) { m_characterPresets = presets; emit characterPresetsChanged(); }
void DataModel::addCharacterPreset(const CharacterPreset& preset) { m_characterPresets.append(preset); emit characterPresetsChanged(); }
void DataModel::updateCharacterPreset(int index, const CharacterPreset& preset) { if(index>=0 && index<m_characterPresets.size()) { m_characterPresets[index] = preset; emit characterPresetsChanged(); } }
void DataModel::removeCharacterPreset(int index) { if(index>=0 && index<m_characterPresets.size()) { m_characterPresets.removeAt(index); emit characterPresetsChanged(); } }
CharacterPreset* DataModel::findCharacterPreset(const QString& name) { for(auto& c : m_characterPresets) if(c.name()==name) return &c; return nullptr; }

const QVector<TeamConfig>& DataModel::teams() const { return m_teams; }
void DataModel::setTeams(const QVector<TeamConfig>& teams) { m_teams = teams; emit teamsChanged(); }
void DataModel::addTeam(const TeamConfig& team) { m_teams.append(team); emit teamsChanged(); }
void DataModel::updateTeam(int index, const TeamConfig& team) { if(index>=0 && index<m_teams.size()) { m_teams[index] = team; emit teamsChanged(); } }
void DataModel::removeTeam(int index) { if(index>=0 && index<m_teams.size()) { m_teams.removeAt(index); emit teamsChanged(); } }

const QVector<Buff>& DataModel::buffTemplates() const { return m_buffTemplates; }
void DataModel::setBuffTemplates(const QVector<Buff>& buffs) { m_buffTemplates = buffs; emit buffsChanged(); }
void DataModel::addBuffTemplate(const Buff& buff) { m_buffTemplates.append(buff); emit buffsChanged(); }
void DataModel::removeBuffTemplate(int index) { if(index>=0 && index<m_buffTemplates.size()) { m_buffTemplates.removeAt(index); emit buffsChanged(); } }

const ReactionRuleSet& DataModel::reactionRules() const { return m_reactionRules; }

void DataModel::loadFromJson()
{
    QFile file("data.json");
    if(!file.open(QIODevice::ReadOnly)) { qDebug() << "No data.json found."; return; }
    QByteArray data = file.readAll(); file.close();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject root = doc.object();

    m_characterPresets.clear();
    QJsonArray charArray = root["characters"].toArray();
    for(const auto& val : charArray) m_characterPresets.append(CharacterPreset::fromJson(val.toObject()));

    m_teams.clear();
    QJsonArray teamArray = root["teams"].toArray();
    for(const auto& val : teamArray) m_teams.append(TeamConfig::fromJson(val.toObject()));

    m_buffTemplates.clear();
    QJsonArray buffArray = root["buffs"].toArray();
    for(const auto& val : buffArray) m_buffTemplates.append(Buff::fromJson(val.toObject()));
}

void DataModel::saveToJson() const
{
    QJsonObject root;
    QJsonArray charArray; for(const auto& c : m_characterPresets) charArray.append(c.toJson());
    root["characters"] = charArray;
    QJsonArray teamArray; for(const auto& t : m_teams) teamArray.append(t.toJson());
    root["teams"] = teamArray;
    QJsonArray buffArray; for(const auto& b : m_buffTemplates) buffArray.append(b.toJson());
    root["buffs"] = buffArray;
    QFile file("data.json");
    if(file.open(QIODevice::WriteOnly)) { file.write(QJsonDocument(root).toJson()); file.close(); }
}

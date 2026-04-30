#include "Page_Team.hpp"
#include "DataModel.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QCheckBox>
#include <QAbstractItemModel>

Page_Team::Page_Team(QWidget *parent) : QWidget(parent)
{
    setupUI();
    connect(&DataModel::instance(), &DataModel::teamsChanged, this, &Page_Team::refreshTeamList);
    connect(&DataModel::instance(), &DataModel::characterPresetsChanged, this, &Page_Team::refreshMemberCombo);
    refreshTeamList();
    refreshMemberCombo();
}

void Page_Team::setupUI()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // left
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(new QLabel("配队列表", this));
    m_teamList = new QListWidget(this);
    leftLayout->addWidget(m_teamList);
    m_newBtn = new QPushButton("新建配队", this);
    m_deleteBtn = new QPushButton("删除选中", this);
    leftLayout->addWidget(m_newBtn);
    leftLayout->addWidget(m_deleteBtn);
    mainLayout->addLayout(leftLayout, 1);

    // right
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(new QLabel("配队名称:", this));
    m_teamNameEdit = new QLineEdit(this);
    rightLayout->addWidget(m_teamNameEdit);

    rightLayout->addWidget(new QLabel("队伍成员 (最多4人):", this));
    QHBoxLayout *memberLayout = new QHBoxLayout();
    m_memberCombo = new QComboBox(this);
    m_addMemberBtn = new QPushButton("添加", this);
    m_removeMemberBtn = new QPushButton("移除", this);
    memberLayout->addWidget(m_memberCombo, 1);
    memberLayout->addWidget(m_addMemberBtn);
    memberLayout->addWidget(m_removeMemberBtn);
    rightLayout->addLayout(memberLayout);
    m_memberList = new QListWidget(this);
    m_memberList->setMaximumHeight(80);
    rightLayout->addWidget(m_memberList);

    QAbstractItemModel *memberModel = m_memberList->model();
    connect(memberModel, &QAbstractItemModel::rowsInserted, this, [this]() { refreshMemberCombo(); });
    connect(memberModel, &QAbstractItemModel::rowsRemoved, this, [this]() { refreshMemberCombo(); });

    rightLayout->addWidget(new QLabel("技能序列:", this));
    QHBoxLayout *skillCtrl = new QHBoxLayout();
    m_skillCharCombo = new QComboBox(this);
    m_addSkillBtn = new QPushButton("添加步骤", this);
    m_removeSkillBtn = new QPushButton("删除选中", this);
    skillCtrl->addWidget(new QLabel("角色:", this));
    skillCtrl->addWidget(m_skillCharCombo, 1);
    skillCtrl->addWidget(m_addSkillBtn);
    skillCtrl->addWidget(m_removeSkillBtn);
    rightLayout->addLayout(skillCtrl);

    // 技能序列表格：角色、技能名称、反应标记（移除动作时间列）
    m_skillSeqTable = new QTableWidget(0, 3, this);
    m_skillSeqTable->setHorizontalHeaderLabels({"角色", "技能名称", "反应标记"});
    m_skillSeqTable->horizontalHeader()->setStretchLastSection(true);
    rightLayout->addWidget(m_skillSeqTable);

    // Buff 区域
    rightLayout->addWidget(new QLabel("配队 Buff:", this));
    m_buffTable = new QTableWidget(0, 5, this);
    m_buffTable->setHorizontalHeaderLabels({"来源角色", "目标角色", "增益类型", "数值", "百分比"});
    m_buffTable->horizontalHeader()->setStretchLastSection(true);
    rightLayout->addWidget(m_buffTable);

    QHBoxLayout *buffBtnLayout = new QHBoxLayout();
    m_addBuffBtn = new QPushButton("添加 Buff", this);
    m_removeBuffBtn = new QPushButton("删除 Buff", this);
    buffBtnLayout->addWidget(m_addBuffBtn);
    buffBtnLayout->addWidget(m_removeBuffBtn);
    rightLayout->addLayout(buffBtnLayout);

    m_saveBtn = new QPushButton("保存当前配队", this);
    rightLayout->addWidget(m_saveBtn);
    mainLayout->addLayout(rightLayout, 3);

    // connections
    connect(m_newBtn, &QPushButton::clicked, this, &Page_Team::onNewTeam);
    connect(m_deleteBtn, &QPushButton::clicked, this, &Page_Team::onDeleteTeam);
    connect(m_saveBtn, &QPushButton::clicked, this, &Page_Team::onSaveTeam);
    connect(m_teamList, &QListWidget::currentRowChanged, this, &Page_Team::onTeamListSelectionChanged);
    connect(m_addMemberBtn, &QPushButton::clicked, this, &Page_Team::onAddMember);
    connect(m_removeMemberBtn, &QPushButton::clicked, this, &Page_Team::onRemoveMember);
    connect(m_addSkillBtn, &QPushButton::clicked, this, &Page_Team::onAddSkillStep);
    connect(m_removeSkillBtn, &QPushButton::clicked, this, &Page_Team::onRemoveSkillStep);
    connect(m_addBuffBtn, &QPushButton::clicked, this, &Page_Team::onAddBuff);
    connect(m_removeBuffBtn, &QPushButton::clicked, this, &Page_Team::onRemoveBuff);
}

void Page_Team::refreshTeamList()
{
    m_teamList->clear();
    for (const auto& t : DataModel::instance().teams()) m_teamList->addItem(t.name());
}

void Page_Team::refreshMemberCombo()
{
    m_memberCombo->clear();
    m_skillCharCombo->clear();
    const auto& chars = DataModel::instance().characterPresets();
    for (const auto& c : chars) m_memberCombo->addItem(c.name());
    for (int i = 0; i < m_memberList->count(); ++i)
        m_skillCharCombo->addItem(m_memberList->item(i)->text());
}

void Page_Team::onAddMember()
{
    QString name = m_memberCombo->currentText();
    if (name.isEmpty()) return;
    if (m_memberList->count() >= 4) { QMessageBox::information(this, "提示", "队伍最多4人。"); return; }
    for (int i = 0; i < m_memberList->count(); ++i)
        if (m_memberList->item(i)->text() == name) { QMessageBox::information(this, "提示", "角色已在队伍中。"); return; }
    m_memberList->addItem(name);
}

void Page_Team::onRemoveMember()
{
    int row = m_memberList->currentRow();
    if (row >= 0) delete m_memberList->takeItem(row);
}

void Page_Team::onAddSkillStep()
{
    QString charName = m_skillCharCombo->currentText();
    if (charName.isEmpty()) return;
    int row = m_skillSeqTable->rowCount();
    m_skillSeqTable->insertRow(row);
    m_skillSeqTable->setItem(row, 0, new QTableWidgetItem(charName));

    QComboBox *skillCombo = new QComboBox();
    const CharacterPreset* preset = DataModel::instance().findCharacterPreset(charName);
    if (preset) {
        const auto& skills = preset->skills();
        for (const SkillAction& sa : skills)
            skillCombo->addItem(sa.name);
    }
    m_skillSeqTable->setCellWidget(row, 1, skillCombo);
    m_skillSeqTable->setItem(row, 2, new QTableWidgetItem(""));
}

void Page_Team::onRemoveSkillStep()
{
    int row = m_skillSeqTable->currentRow();
    if (row >= 0) m_skillSeqTable->removeRow(row);
}

void Page_Team::onAddBuff()
{
    int row = m_buffTable->rowCount();
    m_buffTable->insertRow(row);

    // 来源角色下拉
    QComboBox *srcCombo = new QComboBox();
    for (int i = 0; i < m_memberList->count(); ++i)
        srcCombo->addItem(m_memberList->item(i)->text());
    m_buffTable->setCellWidget(row, 0, srcCombo);

    // 目标角色下拉（可包含全体）
    QComboBox *tgtCombo = new QComboBox();
    tgtCombo->addItem("全体");
    for (int i = 0; i < m_memberList->count(); ++i)
        tgtCombo->addItem(m_memberList->item(i)->text());
    m_buffTable->setCellWidget(row, 1, tgtCombo);

    // 增益类型可编辑下拉
    QComboBox *typeCombo = new QComboBox();
    typeCombo->setEditable(true);
    QStringList buffTypes = {"攻击力%", "防御力%", "生命上限%", "元素精通", "充能效率%",
                             "水伤加成%", "火伤加成%", "冰伤加成%", "雷伤加成%",
                             "风伤加成%", "岩伤加成%", "草伤加成%", "物理伤害加成%", "全伤害加成%"};
    typeCombo->addItems(buffTypes);
    typeCombo->setCurrentText("全伤害加成%");
    m_buffTable->setCellWidget(row, 2, typeCombo);

    m_buffTable->setItem(row, 3, new QTableWidgetItem("0"));
    QCheckBox *cb = new QCheckBox();
    cb->setChecked(true);
    m_buffTable->setCellWidget(row, 4, cb);
}

void Page_Team::onRemoveBuff()
{
    int row = m_buffTable->currentRow();
    if (row >= 0) m_buffTable->removeRow(row);
}

void Page_Team::onSaveTeam()
{
    TeamConfig team = collectTeamData();
    if (team.name().isEmpty()) { QMessageBox::warning(this, "错误", "配队名称不能为空。"); return; }
    auto& model = DataModel::instance();
    int sel = m_teamList->currentRow();
    if (sel >= 0 && sel < model.teams().size()) model.updateTeam(sel, team);
    else model.addTeam(team);
}

TeamConfig Page_Team::collectTeamData() const
{
    TeamConfig tc(m_teamNameEdit->text());
    QVector<QString> members;
    for (int i = 0; i < m_memberList->count(); ++i) members.append(m_memberList->item(i)->text());
    tc.setMembers(members);

    QVector<SkillSequenceItem> seq;
    for (int i = 0; i < m_skillSeqTable->rowCount(); ++i) {
        SkillSequenceItem item;
        item.characterName = m_skillSeqTable->item(i, 0) ? m_skillSeqTable->item(i, 0)->text() : "";
        QComboBox* skillCombo = qobject_cast<QComboBox*>(m_skillSeqTable->cellWidget(i, 1));
        item.skillIndex = skillCombo ? skillCombo->currentIndex() : 0;
        item.reactionTag = m_skillSeqTable->item(i, 2) ? m_skillSeqTable->item(i, 2)->text() : "";
        seq.append(item);
    }
    tc.setSkillSequence(seq);

    QVector<TeamBuff> buffs;
    for (int i = 0; i < m_buffTable->rowCount(); ++i) {
        TeamBuff tb;
        QComboBox* src = qobject_cast<QComboBox*>(m_buffTable->cellWidget(i, 0));
        QComboBox* tgt = qobject_cast<QComboBox*>(m_buffTable->cellWidget(i, 1));
        QComboBox* type = qobject_cast<QComboBox*>(m_buffTable->cellWidget(i, 2));
        tb.sourceCharacter = src ? src->currentText() : "";
        QString tgtText = tgt ? tgt->currentText() : "";
        tb.targetCharacter = (tgtText == "全体") ? "" : tgtText;
        tb.buffType = type ? type->currentText() : "";
        tb.value = m_buffTable->item(i, 3) ? m_buffTable->item(i, 3)->text().toDouble() : 0.0;
        QCheckBox *cb = qobject_cast<QCheckBox*>(m_buffTable->cellWidget(i, 4));
        tb.isPercentage = cb ? cb->isChecked() : true;
        buffs.append(tb);
    }
    tc.setBuffs(buffs);
    return tc;
}

void Page_Team::loadTeamToForm(const TeamConfig& team)
{
    m_teamNameEdit->setText(team.name());
    m_memberList->clear();
    m_skillCharCombo->clear();
    for (const auto& m : team.members()) {
        m_memberList->addItem(m);
        m_skillCharCombo->addItem(m);
    }
    m_skillSeqTable->setRowCount(0);
    for (const auto& step : team.skillSequence()) {
        int row = m_skillSeqTable->rowCount();
        m_skillSeqTable->insertRow(row);
        m_skillSeqTable->setItem(row, 0, new QTableWidgetItem(step.characterName));
        QComboBox *skillCombo = new QComboBox();
        const CharacterPreset* preset = DataModel::instance().findCharacterPreset(step.characterName);
        if (preset) {
            const auto& skills = preset->skills();
            for (const SkillAction& sa : skills)
                skillCombo->addItem(sa.name);
            if (step.skillIndex >= 0 && step.skillIndex < skills.size())
                skillCombo->setCurrentIndex(step.skillIndex);
        }
        m_skillSeqTable->setCellWidget(row, 1, skillCombo);
        m_skillSeqTable->setItem(row, 2, new QTableWidgetItem(step.reactionTag));
    }

    m_buffTable->setRowCount(0);
    for (const auto& b : team.buffs()) {
        int row = m_buffTable->rowCount();
        m_buffTable->insertRow(row);

        QComboBox *srcCombo = new QComboBox();
        for (int i = 0; i < m_memberList->count(); ++i)
            srcCombo->addItem(m_memberList->item(i)->text());
        srcCombo->setCurrentText(b.sourceCharacter);
        m_buffTable->setCellWidget(row, 0, srcCombo);

        QComboBox *tgtCombo = new QComboBox();
        tgtCombo->addItem("全体");
        for (int i = 0; i < m_memberList->count(); ++i)
            tgtCombo->addItem(m_memberList->item(i)->text());
        tgtCombo->setCurrentText(b.targetCharacter.isEmpty() ? "全体" : b.targetCharacter);
        m_buffTable->setCellWidget(row, 1, tgtCombo);

        QComboBox *typeCombo = new QComboBox();
        typeCombo->setEditable(true);
        QStringList buffTypes = {"攻击力%", "防御力%", "生命上限%", "元素精通", "充能效率%",
                                 "水伤加成%", "火伤加成%", "冰伤加成%", "雷伤加成%",
                                 "风伤加成%", "岩伤加成%", "草伤加成%", "物理伤害加成%", "全伤害加成%"};
        typeCombo->addItems(buffTypes);
        typeCombo->setCurrentText(b.buffType);
        m_buffTable->setCellWidget(row, 2, typeCombo);

        m_buffTable->setItem(row, 3, new QTableWidgetItem(QString::number(b.value)));
        QCheckBox *cb = new QCheckBox();
        cb->setChecked(b.isPercentage);
        m_buffTable->setCellWidget(row, 4, cb);
    }
}

void Page_Team::clearForm()
{
    m_teamNameEdit->clear();
    m_memberList->clear();
    m_skillCharCombo->clear();
    m_skillSeqTable->setRowCount(0);
    m_buffTable->setRowCount(0);
}

void Page_Team::onNewTeam() { clearForm(); m_teamList->clearSelection(); }
void Page_Team::onDeleteTeam()
{
    int sel = m_teamList->currentRow();
    if (sel < 0) { QMessageBox::information(this, "提示", "请先选择一个配队。"); return; }
    DataModel::instance().removeTeam(sel);
    clearForm();
}
void Page_Team::onTeamListSelectionChanged()
{
    int row = m_teamList->currentRow();
    if (row < 0) return;
    const auto& teams = DataModel::instance().teams();
    if (row < teams.size()) loadTeamToForm(teams[row]);
}

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

    // 当成员列表发生增删时，刷新可供选择技能的角色列表
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
    m_skillSeqTable = new QTableWidget(0, 4, this);
    m_skillSeqTable->setHorizontalHeaderLabels({"角色", "技能名称", "反应标记", "动作时间(秒)"});
    m_skillSeqTable->horizontalHeader()->setStretchLastSection(true);
    rightLayout->addWidget(m_skillSeqTable);

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
    for (const auto& c : chars)
        m_memberCombo->addItem(c.name());
    // 技能角色选择只显示队伍中的成员
    for (int i = 0; i < m_memberList->count(); ++i)
        m_skillCharCombo->addItem(m_memberList->item(i)->text());
}

void Page_Team::onAddMember()
{
    QString name = m_memberCombo->currentText();
    if (name.isEmpty()) return;
    if (m_memberList->count() >= 4) {
        QMessageBox::information(this, "提示", "队伍最多4人。");
        return;
    }
    for (int i = 0; i < m_memberList->count(); ++i) {
        if (m_memberList->item(i)->text() == name) {
            QMessageBox::information(this, "提示", "角色已在队伍中。");
            return;
        }
    }
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
    m_skillSeqTable->setItem(row, 0, new QTableWidgetItem(charName)); // 角色名列

    // 为该角色创建技能下拉框
    QComboBox *skillCombo = new QComboBox();
    const CharacterPreset* preset = DataModel::instance().findCharacterPreset(charName);
    if (preset) {
        const auto& skills = preset->skills();
        for (const SkillAction& sa : skills)
            skillCombo->addItem(sa.name);
    }
    m_skillSeqTable->setCellWidget(row, 1, skillCombo);

    m_skillSeqTable->setItem(row, 2, new QTableWidgetItem(""));   // 反应标记
    m_skillSeqTable->setItem(row, 3, new QTableWidgetItem("1.0"));
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
    m_buffTable->setItem(row, 0, new QTableWidgetItem(""));
    m_buffTable->setItem(row, 1, new QTableWidgetItem(""));
    m_buffTable->setItem(row, 2, new QTableWidgetItem("allDamage"));
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
    if (team.name().isEmpty()) {
        QMessageBox::warning(this, "错误", "配队名称不能为空。");
        return;
    }
    auto& model = DataModel::instance();
    int sel = m_teamList->currentRow();
    if (sel >= 0 && sel < model.teams().size())
        model.updateTeam(sel, team);
    else
        model.addTeam(team);
}

TeamConfig Page_Team::collectTeamData() const
{
    TeamConfig tc(m_teamNameEdit->text());
    QVector<QString> members;
    for (int i = 0; i < m_memberList->count(); ++i)
        members.append(m_memberList->item(i)->text());
    tc.setMembers(members);

    QVector<SkillSequenceItem> seq;
    for (int i = 0; i < m_skillSeqTable->rowCount(); ++i) {
        SkillSequenceItem item;
        item.characterName = m_skillSeqTable->item(i, 0) ? m_skillSeqTable->item(i, 0)->text() : "";
        QComboBox* combo = qobject_cast<QComboBox*>(m_skillSeqTable->cellWidget(i, 1));
        item.skillIndex = combo ? combo->currentIndex() : 0;
        item.reactionTag = m_skillSeqTable->item(i, 2) ? m_skillSeqTable->item(i, 2)->text() : "";
        seq.append(item);
    }
    tc.setSkillSequence(seq);

    QVector<TeamBuff> buffs;
    for (int i = 0; i < m_buffTable->rowCount(); ++i) {
        TeamBuff tb;
        tb.sourceCharacter = m_buffTable->item(i,0) ? m_buffTable->item(i,0)->text() : "";
        tb.targetCharacter = m_buffTable->item(i,1) ? m_buffTable->item(i,1)->text() : "";
        tb.buffType = m_buffTable->item(i,2) ? m_buffTable->item(i,2)->text() : "";
        tb.value = m_buffTable->item(i,3) ? m_buffTable->item(i,3)->text().toDouble() : 0.0;
        QCheckBox *cb = qobject_cast<QCheckBox*>(m_buffTable->cellWidget(i,4));
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

        // 创建技能下拉框
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
        m_skillSeqTable->setItem(row, 3, new QTableWidgetItem("~"));
    }
    m_buffTable->setRowCount(0);
    for (const auto& b : team.buffs()) {
        int row = m_buffTable->rowCount();
        m_buffTable->insertRow(row);
        m_buffTable->setItem(row, 0, new QTableWidgetItem(b.sourceCharacter));
        m_buffTable->setItem(row, 1, new QTableWidgetItem(b.targetCharacter));
        m_buffTable->setItem(row, 2, new QTableWidgetItem(b.buffType));
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
    if (sel < 0) {
        QMessageBox::information(this, "提示", "请先选择一个配队。");
        return;
    }
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

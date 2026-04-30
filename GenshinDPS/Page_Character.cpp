#include "Page_Character.hpp"
#include "DataModel.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>

Page_Character::Page_Character(QWidget *parent) : QWidget(parent)
{
    setupUI();
    connect(&DataModel::instance(), &DataModel::characterPresetsChanged, this, &Page_Character::loadPresetToList);
    loadPresetToList();
}

void Page_Character::setupUI()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 左侧预设列表
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(new QLabel("角色预设列表", this));
    m_presetList = new QListWidget(this);
    leftLayout->addWidget(m_presetList);
    m_newBtn = new QPushButton("新建预设", this);
    m_deleteBtn = new QPushButton("删除选中", this);
    leftLayout->addWidget(m_newBtn);
    leftLayout->addWidget(m_deleteBtn);
    mainLayout->addLayout(leftLayout, 1);

    // 右侧编辑区
    QVBoxLayout *rightLayout = new QVBoxLayout();
    QLabel* editLabel = new QLabel("编辑角色预设", this);
    editLabel->setStyleSheet("font-weight:bold;");
    rightLayout->addWidget(editLabel);

    QFormLayout *form = new QFormLayout();
    m_nameEdit = new QLineEdit(this);
    form->addRow("角色名:", m_nameEdit);

    m_hpEdit = new QDoubleSpinBox(this); m_hpEdit->setRange(0, 999999);
    m_atkEdit = new QDoubleSpinBox(this); m_atkEdit->setRange(0, 99999);
    m_defEdit = new QDoubleSpinBox(this); m_defEdit->setRange(0, 99999);
    m_emEdit = new QDoubleSpinBox(this); m_emEdit->setRange(0, 9999);
    m_erEdit = new QDoubleSpinBox(this); m_erEdit->setRange(0, 9999);
    form->addRow("生命上限:", m_hpEdit);
    form->addRow("攻击力:", m_atkEdit);
    form->addRow("防御力:", m_defEdit);
    form->addRow("元素精通:", m_emEdit);
    form->addRow("充能效率:", m_erEdit);

    // 元素与物理加成 (0~7)
    QStringList bonusNames = {"水伤加成","火伤加成","冰伤加成","雷伤加成","风伤加成","岩伤加成","草伤加成","物理加成"};
    m_bonusEdits.resize(10); // 预分配10个位置 (0~7加成, 8暴击, 9暴伤)
    for (int i = 0; i < 8; ++i) {
        m_bonusEdits[i] = new QDoubleSpinBox(this);
        m_bonusEdits[i]->setRange(0, 9999);
        m_bonusEdits[i]->setSuffix("%");
        form->addRow(bonusNames[i], m_bonusEdits[i]);
    }

    // 双暴
    m_bonusEdits[8] = new QDoubleSpinBox(this);
    m_bonusEdits[8]->setRange(0, 100);
    m_bonusEdits[8]->setSuffix("%");
    form->addRow("暴击率:", m_bonusEdits[8]);

    m_bonusEdits[9] = new QDoubleSpinBox(this);
    m_bonusEdits[9]->setRange(0, 500);
    m_bonusEdits[9]->setSuffix("%");
    form->addRow("暴击伤害:", m_bonusEdits[9]);

    rightLayout->addLayout(form);

    // 技能表格
    rightLayout->addWidget(new QLabel("天赋技能列表", this));
    m_skillTable = new QTableWidget(0, 4, this);
    m_skillTable->setHorizontalHeaderLabels({"技能名", "类型", "倍率(%)", "动作时间(秒)"});
    m_skillTable->horizontalHeader()->setStretchLastSection(true);
    rightLayout->addWidget(m_skillTable);

    QHBoxLayout *skillBtnLayout = new QHBoxLayout();
    m_addSkillBtn = new QPushButton("添加技能", this);
    m_removeSkillBtn = new QPushButton("删除选中", this);
    skillBtnLayout->addWidget(m_addSkillBtn);
    skillBtnLayout->addWidget(m_removeSkillBtn);
    rightLayout->addLayout(skillBtnLayout);

    m_saveBtn = new QPushButton("保存当前预设", this);
    rightLayout->addWidget(m_saveBtn);
    mainLayout->addLayout(rightLayout, 2);

    // 信号连接
    connect(m_newBtn, &QPushButton::clicked, this, &Page_Character::onNewPreset);
    connect(m_deleteBtn, &QPushButton::clicked, this, &Page_Character::onDeletePreset);
    connect(m_saveBtn, &QPushButton::clicked, this, &Page_Character::onSavePreset);
    connect(m_presetList, &QListWidget::currentRowChanged, this, &Page_Character::onPresetListSelectionChanged);
    connect(m_addSkillBtn, &QPushButton::clicked, [this]() {
        int row = m_skillTable->rowCount();
        m_skillTable->insertRow(row);
        m_skillTable->setItem(row, 0, new QTableWidgetItem("新技能"));
        QComboBox *typeCombo = new QComboBox();
        typeCombo->addItems({"普通攻击","重击","元素战技","元素爆发","其他"});
        m_skillTable->setCellWidget(row, 1, typeCombo);
        m_skillTable->setItem(row, 2, new QTableWidgetItem("100"));
        m_skillTable->setItem(row, 3, new QTableWidgetItem("1.0"));
    });
    connect(m_removeSkillBtn, &QPushButton::clicked, [this]() {
        int row = m_skillTable->currentRow();
        if (row >= 0) m_skillTable->removeRow(row);
    });
}

void Page_Character::loadPresetToList()
{
    m_presetList->clear();
    for (const auto& p : DataModel::instance().characterPresets())
        m_presetList->addItem(p.name());
}

void Page_Character::onPresetListSelectionChanged()
{
    int row = m_presetList->currentRow();
    if (row < 0) return;
    const auto& presets = DataModel::instance().characterPresets();
    if (row < presets.size()) loadPresetToForm(presets[row]);
}

void Page_Character::loadPresetToForm(const CharacterPreset& preset)
{
    m_nameEdit->setText(preset.name());
    auto s = preset.baseStats();
    m_hpEdit->setValue(s.maxHP);
    m_atkEdit->setValue(s.attack);
    m_defEdit->setValue(s.defense);
    m_emEdit->setValue(s.elementalMastery);
    m_erEdit->setValue(s.energyRecharge);

    double bonuses[] = {s.hydroBonus, s.pyroBonus, s.cryoBonus, s.electroBonus, s.anemoBonus, s.geoBonus, s.dendroBonus, s.physicalBonus};
    for (int i = 0; i < 8; ++i) m_bonusEdits[i]->setValue(bonuses[i]);

    m_bonusEdits[8]->setValue(s.critRate);
    m_bonusEdits[9]->setValue(s.critDamage);

    m_skillTable->setRowCount(0);
    for (const auto& skill : preset.skills()) {
        int row = m_skillTable->rowCount();
        m_skillTable->insertRow(row);
        m_skillTable->setItem(row, 0, new QTableWidgetItem(skill.name));
        QComboBox *typeCombo = new QComboBox();
        typeCombo->addItems({"普通攻击","重击","元素战技","元素爆发","其他"});
        typeCombo->setCurrentIndex(static_cast<int>(skill.type));
        m_skillTable->setCellWidget(row, 1, typeCombo);
        m_skillTable->setItem(row, 2, new QTableWidgetItem(QString::number(skill.multiplier)));
        m_skillTable->setItem(row, 3, new QTableWidgetItem(QString::number(skill.actionTime)));
    }
}

CharacterPreset Page_Character::collectFormData() const
{
    CharacterPreset preset(m_nameEdit->text());
    CharacterBaseStats stats;
    stats.maxHP = m_hpEdit->value();
    stats.attack = m_atkEdit->value();
    stats.defense = m_defEdit->value();
    stats.elementalMastery = m_emEdit->value();
    stats.energyRecharge = m_erEdit->value();
    stats.hydroBonus = m_bonusEdits[0]->value();
    stats.pyroBonus = m_bonusEdits[1]->value();
    stats.cryoBonus = m_bonusEdits[2]->value();
    stats.electroBonus = m_bonusEdits[3]->value();
    stats.anemoBonus = m_bonusEdits[4]->value();
    stats.geoBonus = m_bonusEdits[5]->value();
    stats.dendroBonus = m_bonusEdits[6]->value();
    stats.physicalBonus = m_bonusEdits[7]->value();
    stats.critRate = m_bonusEdits[8]->value();
    stats.critDamage = m_bonusEdits[9]->value();
    preset.setBaseStats(stats);

    QVector<SkillAction> skills;
    for (int i = 0; i < m_skillTable->rowCount(); ++i) {
        SkillAction sa;
        sa.name = m_skillTable->item(i, 0) ? m_skillTable->item(i, 0)->text() : "";
        QComboBox* combo = qobject_cast<QComboBox*>(m_skillTable->cellWidget(i, 1));
        sa.type = combo ? static_cast<SkillType>(combo->currentIndex()) : SkillType::NormalAttack;
        sa.multiplier = m_skillTable->item(i, 2) ? m_skillTable->item(i, 2)->text().toDouble() : 100.0;
        sa.actionTime = m_skillTable->item(i, 3) ? m_skillTable->item(i, 3)->text().toDouble() : 1.0;
        skills.append(sa);
    }
    preset.setSkills(skills);
    return preset;
}

void Page_Character::clearForm()
{
    m_nameEdit->clear();
    m_hpEdit->setValue(0);
    m_atkEdit->setValue(0);
    m_defEdit->setValue(0);
    m_emEdit->setValue(0);
    m_erEdit->setValue(0);
    for (int i = 0; i < 10; ++i) m_bonusEdits[i]->setValue(0);
    m_skillTable->setRowCount(0);
}

void Page_Character::onNewPreset() { clearForm(); m_presetList->clearSelection(); }

void Page_Character::onSavePreset()
{
    CharacterPreset preset = collectFormData();
    if (preset.name().isEmpty()) { QMessageBox::warning(this, "错误", "角色名不能为空。"); return; }
    auto& model = DataModel::instance();
    int sel = m_presetList->currentRow();
    if (sel >= 0 && sel < model.characterPresets().size())
        model.updateCharacterPreset(sel, preset);
    else
        model.addCharacterPreset(preset);
}

void Page_Character::onDeletePreset()
{
    int sel = m_presetList->currentRow();
    if (sel < 0) { QMessageBox::information(this, "提示", "请先选择一个预设。"); return; }
    DataModel::instance().removeCharacterPreset(sel);
    clearForm();
}

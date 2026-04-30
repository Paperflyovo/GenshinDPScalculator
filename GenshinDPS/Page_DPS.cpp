#include "Page_DPS.hpp"

Page_DPS::Page_DPS(QWidget *parent) : QWidget(parent)
{
    setupUI();
    connect(&DataModel::instance(), &DataModel::teamsChanged, this, &Page_DPS::refreshTeamList);
    connect(&DataModel::instance(), &DataModel::monsterLevelChanged, this, [this]() {
        m_monsterLevelEdit->setValue(DataModel::instance().monsterLevel());
    });
    refreshTeamList();
    m_monsterLevelEdit->setValue(DataModel::instance().monsterLevel());
}

void Page_DPS::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *title = new QLabel("配队选择与DPS计算", this);
    title->setStyleSheet("font-size:16px; font-weight:bold;");
    layout->addWidget(title);

    m_teamCombo = new QComboBox(this);
    layout->addWidget(m_teamCombo);

    QHBoxLayout *monsterLayout = new QHBoxLayout();
    monsterLayout->addWidget(new QLabel("怪物等级:"));
    m_monsterLevelEdit = new QSpinBox(this);
    m_monsterLevelEdit->setRange(1, 200);
    m_monsterLevelEdit->setValue(110);
    monsterLayout->addWidget(m_monsterLevelEdit);
    layout->addLayout(monsterLayout);

    m_calcButton = new QPushButton("开始计算", this);
    layout->addWidget(m_calcButton);

    m_resultView = new QTextEdit(this);
    m_resultView->setReadOnly(true);
    m_resultView->setPlaceholderText("计算结果将显示在此处...");
    layout->addWidget(m_resultView);

    connect(m_calcButton, &QPushButton::clicked, this, &Page_DPS::onCalculate);
    connect(m_monsterLevelEdit, QOverload<int>::of(&QSpinBox::valueChanged), [this](int val) {
        DataModel::instance().setMonsterLevel(val);
    });
}

void Page_DPS::refreshTeamList()
{
    m_teamCombo->clear();
    const auto& teams = DataModel::instance().teams();
    for (const auto& team : teams) m_teamCombo->addItem(team.name());
}

void Page_DPS::onTeamSelected() {}

void Page_DPS::onCalculate()
{
    QString teamName = m_teamCombo->currentText();
    const auto& teams = DataModel::instance().teams();
    const TeamConfig* selected = nullptr;
    for (const auto& t : teams) if (t.name() == teamName) { selected = &t; break; }
    if (!selected) { m_resultView->setText("请选择一个有效的配队。"); return; }

    int monsterLevel = m_monsterLevelEdit->value();
    // 复制一份角色预设数据，保证计算期间不会受外部修改影响
    QVector<CharacterPreset> presetsCopy = DataModel::instance().characterPresets();
    DPSResult result = m_engine.calculate(*selected, presetsCopy,
                                          DataModel::instance().reactionRules(), monsterLevel);
    displayResult(result);
}

void Page_DPS::displayResult(const DPSResult& result)
{
    QString text;
    text += QString("总伤害: %1\n").arg(result.totalDamage, 0, 'f', 2);
    text += QString("总时间: %1 秒\n").arg(result.totalTime, 0, 'f', 2);
    text += QString("DPS: %1\n\n").arg(result.dps, 0, 'f', 2);
    text += "--- 伤害明细 ---\n";
    for (const auto& step : result.steps) {
        text += QString("[%1] %2: 原始伤害 %3, 反应倍率 %4, Buff倍率 %5, 最终 %6 (耗时 %7s)\n")
            .arg(step.characterName, step.skillName)
            .arg(step.rawDamage, 0, 'f', 1)
            .arg(step.reactionMultiplier, 0, 'f', 2)
            .arg(step.buffMultiplier, 0, 'f', 2)
            .arg(step.finalDamage, 0, 'f', 1)
            .arg(step.actionTime, 0, 'f', 2);
    }
    m_resultView->setText(text);
}

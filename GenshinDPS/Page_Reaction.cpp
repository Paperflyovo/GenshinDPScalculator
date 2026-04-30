#include "Page_Reaction.hpp"
#include "DataModel.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

Page_Reaction::Page_Reaction(QWidget *parent) : QWidget(parent)
{
    setupUI();
    loadReactions();
}

void Page_Reaction::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *title = new QLabel("元素增幅反应规则 (硬编码，仅展示)", this);
    title->setStyleSheet("font-size:14px; font-weight:bold;");
    layout->addWidget(title);
    m_reactionTable = new QTableWidget(0, 4, this);
    m_reactionTable->setHorizontalHeaderLabels({"反应名称","触发元素","附着元素","伤害倍率"});
    m_reactionTable->horizontalHeader()->setStretchLastSection(true);
    m_reactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(m_reactionTable);
}

void Page_Reaction::loadReactions()
{
    const auto& rules = DataModel::instance().reactionRules().reactions();
    m_reactionTable->setRowCount(rules.size());
    for (int i = 0; i < rules.size(); ++i) {
        const AmplifyingReaction& r = rules[i];
        m_reactionTable->setItem(i, 0, new QTableWidgetItem(r.name));
        m_reactionTable->setItem(i, 1, new QTableWidgetItem(r.triggerElement));
        m_reactionTable->setItem(i, 2, new QTableWidgetItem(r.auraElement));
        m_reactionTable->setItem(i, 3, new QTableWidgetItem(QString::number(r.damageMultiplier, 'f', 1)));
    }
}

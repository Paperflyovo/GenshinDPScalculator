#include "MainWindow.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("原神DPS计算器");
    resize(1000, 650);
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    central->setLayout(mainLayout);

    QHBoxLayout *navLayout = new QHBoxLayout();
    navLayout->setSpacing(10);
    navLayout->addStretch();
    QStringList navNames = {"DPS计算", "角色", "配队", "元素反应"};
    for (int i = 0; i < navNames.size(); ++i) {
        QPushButton *btn = new QPushButton(navNames[i], this);
        btn->setCheckable(true);
        btn->setFixedHeight(32);
        navLayout->addWidget(btn);
        m_navButtons.append(btn);
        connect(btn, &QPushButton::clicked, [this, i]() { switchPage(i); });
    }
    navLayout->addStretch();
    mainLayout->addLayout(navLayout);

    m_pages = new QStackedWidget(this);
    m_pageDPS = new Page_DPS(this);
    m_pageChar = new Page_Character(this);
    m_pageTeam = new Page_Team(this);
    m_pageReaction = new Page_Reaction(this);

    m_pages->addWidget(m_pageDPS);
    m_pages->addWidget(m_pageChar);
    m_pages->addWidget(m_pageTeam);
    m_pages->addWidget(m_pageReaction);
    mainLayout->addWidget(m_pages);

    setCentralWidget(central);
    switchPage(0);
}

void MainWindow::switchPage(int index)
{
    if (index < 0 || index >= m_pages->count()) return;
    m_pages->setCurrentIndex(index);
    for (int i = 0; i < m_navButtons.size(); ++i)
        m_navButtons[i]->setChecked(i == index);
}

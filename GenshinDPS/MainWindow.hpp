#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include "Page_DPS.hpp"
#include "Page_Character.hpp"
#include "Page_Team.hpp"
#include "Page_Reaction.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void switchPage(int index);

private:
    void setupUI();
    QStackedWidget *m_pages;
    QVector<QPushButton*> m_navButtons;
    Page_DPS *m_pageDPS;
    Page_Character *m_pageChar;
    Page_Team *m_pageTeam;
    Page_Reaction *m_pageReaction;
};

#endif

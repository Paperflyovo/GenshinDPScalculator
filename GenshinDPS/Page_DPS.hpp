#ifndef PAGE_DPS_HPP
#define PAGE_DPS_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QSpinBox>
#include "DPSEngine.hpp"
#include "DataModel.hpp"

class Page_DPS : public QWidget
{
    Q_OBJECT
public:
    explicit Page_DPS(QWidget *parent = nullptr);

private slots:
    void onTeamSelected();
    void onCalculate();

private:
    void setupUI();
    void refreshTeamList();
    void displayResult(const DPSResult& result);

    QComboBox *m_teamCombo;
    QPushButton *m_calcButton;
    QTextEdit *m_resultView;
    QSpinBox *m_monsterLevelEdit;
    DPSEngine m_engine;
};

#endif

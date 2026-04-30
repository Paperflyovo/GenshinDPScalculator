#ifndef PAGE_TEAM_HPP
#define PAGE_TEAM_HPP

#include <QWidget>
#include <QListWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include "TeamConfig.hpp"

class Page_Team : public QWidget
{
    Q_OBJECT
public:
    explicit Page_Team(QWidget *parent = nullptr);

private slots:
    void onAddMember();
    void onRemoveMember();
    void onAddSkillStep();
    void onRemoveSkillStep();
    void onAddBuff();
    void onRemoveBuff();
    void onSaveTeam();
    void onNewTeam();
    void onDeleteTeam();
    void onTeamListSelectionChanged();
    void refreshTeamList();
    void refreshMemberCombo();

private:
    void setupUI();
    void loadTeamToForm(const TeamConfig& team);
    TeamConfig collectTeamData() const;
    void clearForm();

    QListWidget *m_teamList;
    QLineEdit *m_teamNameEdit;
    QListWidget *m_memberList;
    QComboBox *m_memberCombo;
    QPushButton *m_addMemberBtn, *m_removeMemberBtn;
    QTableWidget *m_skillSeqTable;
    QPushButton *m_addSkillBtn, *m_removeSkillBtn;
    QComboBox *m_skillCharCombo;
    QTableWidget *m_buffTable;
    QPushButton *m_addBuffBtn, *m_removeBuffBtn;
    QPushButton *m_newBtn, *m_saveBtn, *m_deleteBtn;
};

#endif

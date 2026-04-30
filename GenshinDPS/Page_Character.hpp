#ifndef PAGE_CHARACTER_HPP
#define PAGE_CHARACTER_HPP

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QTableWidget>
#include <QVector>
#include <QScrollArea>
#include "CharacterPreset.hpp"

class Page_Character : public QWidget
{
    Q_OBJECT
public:
    explicit Page_Character(QWidget *parent = nullptr);

private slots:
    void onNewPreset();
    void onSavePreset();
    void onDeletePreset();
    void onPresetSelectionChanged(int index);

private:
    void setupUI();
    void loadPresetToCombo();
    void loadPresetToForm(const CharacterPreset& preset);
    CharacterPreset collectFormData() const;
    void clearForm();

    QComboBox *m_presetCombo;
    QLineEdit *m_nameEdit;
    QDoubleSpinBox *m_hpEdit, *m_atkEdit, *m_defEdit, *m_emEdit, *m_erEdit;
    QVector<QDoubleSpinBox*> m_bonusEdits;   // 0~7: 元素/物理加成, 8:暴击率, 9:暴击伤害
    QSpinBox *m_levelEdit;
    QTableWidget *m_skillTable;
    QPushButton *m_addSkillBtn, *m_removeSkillBtn;
    QPushButton *m_newBtn, *m_saveBtn, *m_deleteBtn;
};

#endif

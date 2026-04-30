#ifndef PAGE_REACTION_HPP
#define PAGE_REACTION_HPP

#include <QWidget>
#include <QTableWidget>

class Page_Reaction : public QWidget
{
    Q_OBJECT
public:
    explicit Page_Reaction(QWidget *parent = nullptr);
private:
    void setupUI();
    void loadReactions();
    QTableWidget *m_reactionTable;
};

#endif

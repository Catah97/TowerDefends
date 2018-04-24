#ifndef DEVELOP_BOTTOMBAR_H
#define DEVELOP_BOTTOMBAR_H


#include "Item.h"

class Live : public UiItem{
public:
    Live(int x = -1, int y = -1, int width = -1, int height = -1);
    virtual ~Live() = default;

    virtual void draw();
};

class TowerSelectButton : public UiItem{
private:
    const Tower* m_definedTower;

public:
    bool m_selected;

    explicit TowerSelectButton(const Tower& tower);
    virtual ~TowerSelectButton();

    virtual float getLineWidth();
    virtual void draw();
};

class BottomToolbarCommunicator {
public:
    virtual ~BottomToolbarCommunicator() = default;
    virtual void onTowerSelected(int pos) = 0;
    virtual int getSelectedTowerPosition() = 0;
    virtual int getAvailableMoney() = 0;
};

class BottomToolbar : public UiItem{
private:

    static const unsigned int s_liveWidth = 15;
    static const unsigned int s_liveHeight = 20;

    static const unsigned int s_towerBtnWidth = 30;
    static const unsigned int s_towerBtnHeight = 40;

    BottomToolbarCommunicator* m_gameCommunicator;
    std::vector<Live*> m_livesView;
    std::vector<TowerSelectButton*> m_towersButtonsView;

    void deleteLivesViews();
    void deleteDefinedTowersView();

public:
    BottomToolbar(BottomToolbarCommunicator *gameCommunicator);
    ~BottomToolbar();

    virtual void setLives(int lives);
    virtual void setDefinedTowers(const std::vector<Tower *> &definedTowers);
    virtual bool onMouseClick(int x, int y);
    virtual void draw();
};

#endif //DEVELOP_BOTTOMBAR_H

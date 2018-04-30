#ifndef DEVELOP_BOTTOMBAR_H
#define DEVELOP_BOTTOMBAR_H


#include "Item.h"

/**
 * View that draw players live
 */
class LiveView : public UiItem{
public:
    LiveView(int x = -1, int y = -1, int width = -1, int height = -1);
    virtual ~LiveView() = default;

    virtual void draw();
};

/**
 * Button that is specific for tower select
 */
class TowerSelectButton : public UiItem{
private:
    const Tower* m_definedTower;

public:
    /**
     * Detect if is current button selected
     */
    bool m_selected;

    explicit TowerSelectButton(const Tower& tower);
    virtual ~TowerSelectButton();

    virtual float getLineWidth();
    virtual void draw();
};

/**
 * Interface between Game and BottomToolbar
 */
class BottomToolbarCommunicator {
public:
    virtual ~BottomToolbarCommunicator() = default;

    /**
     * Called when on some TowerSelectButton click
     * @param position of selected Tower in tower define list
     */
    virtual void onTowerSelected(int pos) = 0;

    /**
     * Call to get current selected Tower position in tower define list
     * @return position in tower define list
     */
    virtual int getSelectedTowerPosition() = 0;

    /**
     * Get current players money to drow in BottomToolbar
     * @return current money
     */
    virtual int getAvailableMoney() = 0;
};

/**
 * Parrent view that draw all controls view for game and game information.
 * Draw number of live, money, buttons for tower select.
 */
class BottomToolbar : public UiItem{
private:

    static const unsigned int s_liveWidth = 15;
    static const unsigned int s_liveHeight = 20;

    static const unsigned int s_towerBtnWidth = 30;
    static const unsigned int s_towerBtnHeight = 40;

    BottomToolbarCommunicator* m_gameCommunicator;

    /**
     * Link to all LiveView in this view.
     */
    std::vector<LiveView*> m_livesView;

    /**
     * Link to all TowerSelectButton in this view.
     */
    std::vector<TowerSelectButton*> m_towersButtonsView;


    /**
     * delete all LiveView from m_livesView
     */
    void deleteLivesViews();

    /**
     * delete all TowerSelectButton from m_towersButtonsView
     */
    void deleteDefinedTowersView();

public:
    BottomToolbar(BottomToolbarCommunicator *gameCommunicator);
    ~BottomToolbar();

    /**
     * Set number of LiveView in view
     * @param number of lives
     */
    virtual void setLives(int lives);

    /**
     * Set buttons for all define Towers from @param definedTower
     * @param definedTowers definition of all towers
     */
    virtual void setDefinedTowers(const std::vector<Tower *> &definedTowers);

    /**
     * Mouse click in BottomToolbar
     * @param x mouse X position
     * @param y mouse Y position
     * @return TRUE if was clicked on some buttons else FALSE
     */
    virtual bool onMouseClick(int x, int y);

    /**
     * Draw BottomToolbar and all item within.
     */
    virtual void draw();
};

#endif //DEVELOP_BOTTOMBAR_H

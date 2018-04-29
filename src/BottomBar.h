#ifndef DEVELOP_BOTTOMBAR_H
#define DEVELOP_BOTTOMBAR_H


#include "Item.h"

/**
 * View starající vykreslující život
 * View v řadě vedle sebe používám na vykreslení hřáčových životů
 */
class LiveView : public UiItem{
public:
    LiveView(int x = -1, int y = -1, int width = -1, int height = -1);
    virtual ~LiveView() = default;

    virtual void draw();
};

/**
 * View vykrelsující button pro výběr veže
 */
class TowerSelectButton : public UiItem{
private:
    const Tower* m_definedTower;

public:
    /**
     * Označuje jestli je akturální button označený
     */
    bool m_selected;

    explicit TowerSelectButton(const Tower& tower);
    virtual ~TowerSelectButton();

    virtual float getLineWidth();
    virtual void draw();
};

/**
 * Comunikátor, který obsaráva komunikaci mezi Game a BottomToolbar.
 */
class BottomToolbarCommunicator {
public:
    virtual ~BottomToolbarCommunicator() = default;
    /**
     * V případě, že si uživatel vybral vež přes tlačítko
     * @param pos pozice vybrané věže v listu
     */
    virtual void onTowerSelected(int pos) = 0;

    /**
     * Funkce, která ziská pozici vybrané veže pro zvíraznění tlačítka
     * @return pozice vybrané věže
     */
    virtual int getSelectedTowerPosition() = 0;

    /**
     * Vrací dostupné peníze na nákup veží
     * @return dostupné peníze
     */
    virtual int getAvailableMoney() = 0;
};

/**
 * Rodičovské view ve kterém se zobrazují ovládací prvky.
 * Vykresluje počet životů peníze a tlačítka pro výběr věží.
 */
class BottomToolbar : public UiItem{
private:

    /**
     * šířka jednoho LiveView
     */
    static const unsigned int s_liveWidth = 15;

    /**
     * výška jednoho LiveView
     */
    static const unsigned int s_liveHeight = 20;

    /**
     * šířka jednoho TowerSelectButton
     */
    static const unsigned int s_towerBtnWidth = 30;

    /**
     * výška jednoho TowerSelectButton
     */
    static const unsigned int s_towerBtnHeight = 40;

    /**
     * Odka na communicator oznamující, který předává informace o počtu živtů, počtu peněz a vybrené věži
     */
    BottomToolbarCommunicator* m_gameCommunicator;

    /**
     * Reference na všechny LiveView v tomto view
     */
    std::vector<LiveView*> m_livesView;

    /**
     * List, který ukládá všechny reference na TowerSelectButton v tomto view
     */
    std::vector<TowerSelectButton*> m_towersButtonsView;


    /**
     * Odsraní z paměti všechny LiveView
     */
    void deleteLivesViews();

    /**
     * Odstraní z paměti všechny TowerSelectButton
     */
    void deleteDefinedTowersView();

public:
    BottomToolbar(BottomToolbarCommunicator *gameCommunicator);
    ~BottomToolbar();

    /**
     * Nastavuje počet životů toho view, funkce nejpreve smaže všechny stará view a potom vytvoří nové.
     * @param lives počet životů
     */
    virtual void setLives(int lives);

    /**
     * Nastaví všchny tlačítka pro výběr veží
     * @param definedTowers definece všech veží
     */
    virtual void setDefinedTowers(const std::vector<Tower *> &definedTowers);

    /**
     * Kliknuti do BottomToolbar
     * @param x souřadnice kursoru X
     * @param y souřadnice kursoru Y
     * @return TRUE, pokud bylo kliknuto na nějaké tlačítko, jinak FALSE
     */
    virtual bool onMouseClick(int x, int y);

    /**
     * Vykreslí celý BottomToolbar včetně všech view v něm
     */
    virtual void draw();
};

#endif //DEVELOP_BOTTOMBAR_H

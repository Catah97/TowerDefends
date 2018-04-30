#ifndef TOWER_DEFENDS_UIITEMS_H
#define TOWER_DEFENDS_UIITEMS_H

#include <fstream>
#include <iostream>
#include <vector>

#ifdef __OPENGL__

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <fstream>

#else
#include <GL/glut.h>
#endif

#endif //__OPENGL__

/**
 * Global varaible that save window width and height
 */
extern int m_WindowWidth, m_WindowHeight;

/**
 * Basic view, holds coordinates X and Y in the Cartesian system and display size. These coordinates are convert into vectors in openGl.
 * Contains an abstract function for draw all subview
 */
class UiItem {
private:

    /**
     * Convert standard coordinates into openGl vector coordinates
     * @param position to convert
     * @param size of window
     * @return vector coordinates for openGl
     */
    static float getGlFloat(int position, const int& size);

protected:

    int m_viewX, m_viewY;
    int m_width, m_height;

    /**
     * Position of item in OpenGl
     */
    float m_vectorTop, m_vectorLeft, m_vectorRight, m_vectorBottom;

    /**
     * Draw rectangle with same size as view
     * @param filled TRUE if rectangle should be filled(default is true)
     */
    void drawAsSquare(bool filled = true);

    /**
     * Draw rectangle by params
     * @param left left of rectangle
     * @param top top of rectangle
     * @param right right of rectangle
     * @param bottom bottom of rectangle
     * @param filled TRUE if rectangle should be filled(default is true)
     */
    void drawAsSquare(const float& left, const float& top, const float& right, const float& bottom, bool filled = true);

    /**
     * Get line width for draw
     * @return line width
     */
    virtual float getLineWidth();

public:

    explicit UiItem(int x = -1, int y = -1, int width = -1, int height = -1);

    /**
     * Convert X position to opengl vector
     * @param position X position in window
     * @return vektor vector in opengl
     */
    static float getGlFloatX(const int& position);

    /**
     * Convert Y position to opengl vector
     * @param position Y position in window
     * @return vektor vector in opengl
     */
    static float getGlFloatY(int position);

    /**
     * Detect if coordinates are in view or not
     * @param x
     * @param y
     * @return TRUE if yes, else FALSE
     */
    bool contains(int x, int y) const;

    /**
     * Detect if coordinates are in view or not
     * @param x
     * @param y
     * @return TRUE if yes, else FALSE
     */
    bool contains(float x, float y) const;

    /**
    * Set item position and get vectors for openGl
    * @param x X position in window
    * @param y Y position in window
    * @param width width of view
    * @param height height of view
    */
    virtual void setPosition(int x, int y, int width, int height);

    /**
     * Convert position to opengl vectors
     */
    void calPosition();

    /**
     * Abstract function to draw view
     */
    virtual void draw() = 0;
};

/**
 * View které reprezentuje jednu položku na mapě. Obsahuje souřednice na mapě,
 * poznámku o tom jestli je políčko průchodné či nikoliv a znak políčka.
 */
class MapItem : public UiItem{
protected:

    /**
     * Vykreslí kolem políčka rámeček označující vybraní pozice kursorem
     */
    void drawSelectedSquare();

    /**
     * Funkce vyhodnocující jeslti políčko může být vybráno pro stavbu věže(rámeček je zelený) či nikoliv
     * @return TRUE pokud může být vybráno pro stavbu věže.
     */
    virtual bool canBeSelected();

public:

    /**
     * Proměná označující jestli přes políčko může vést cesta či nikoliv
     */
    bool m_isFree;

    /**
     * Pokud true potom je item vybrán kursorem
     */
    bool m_isSelected;
    char m_mapItemChar;
    int m_mapPositionX, m_mapPositionY;

    explicit MapItem(bool isFree);
    explicit MapItem(const MapItem& mapItem);
    virtual ~MapItem() = default;

    /**
     * Funkce vracící aktuální typ objektu, slouží pro debug
     * @return
     */
    virtual std::string getMapItemType();

    /**
     * Nastaví pozici view a navíc nastaví i souřadnice na mapě
     * @param x souřadnice na ose x
     * @param y souřadnice na ose y
     */
    virtual void setPosition(int x, int y);

    /**
     * Nastaví pozici view a navíc nastaví i souřadnice na mapě
     * @param x souřadnice na ose x
     * @param y souřadnice na ose y
     * @param width šířka objektu
     * @param height výška objektu
     */
    virtual void setPosition(int x, int y, int width, int height);

    /**
     * Příznak oznamující, že je políčko zablokováno, tudíž mohlo být použito pro výpočet cesty, ale
     * neni možné zde momentálně umístit například věž nebo nepřítele.
     * @return TRUE pokud je blokováno
     */
    virtual bool isBlock();

    /**
     * Getter pro m_isFree
     * @return vrací hodnotu m_isFree
     */
    virtual bool isFree() const;

    /**
     * Porovnání dvou MapItem na základě jejich souřadnice
     * @param mapItem
     * @return TRUE, pokud se souřadnice schodují jinak FALSE
     */
    virtual bool operator == (const MapItem& mapItem){
        return m_mapPositionX == mapItem.m_mapPositionX && m_mapPositionY == mapItem.m_mapPositionY;
    }
};


class MapNode{
public:
    int m_x, m_y;
    int m_startDistance, m_endDistance;
    MapNode* m_next,* m_before;

    MapNode(const MapNode& mapNode);
    MapNode(int x, int y, int startDistance, int endDistance, MapNode* m_parent);
    int getEndDistance();
    int getMetrict();

    MapNode& operator = (const MapNode& mapNode);
    bool operator == (const MapNode& mapNode);
    bool operator == (const MapItem& mapItem);

    static void deletePath(MapNode* start);
};

/**
 * Třida označující prázdné políčko na mapě
 */
class FreePlace : public MapItem{
public:
    explicit FreePlace();
    explicit FreePlace(const MapItem &mapItem);


    virtual void draw();
    virtual std::string getMapItemType();

};

/**
 * Třída označující políčko se zdí
 */
class Wall : public MapItem{
public:
    explicit Wall();

    virtual void draw();
    virtual std::string getMapItemType();

};

/**
 * Třída reprezentující nepřátele
 */
class Enemy : public MapItem{
private:

    /**
     * Začátek cesty nepřítele
     */
    MapNode* m_enemyPath;

    /**
     * Odkaz na další MapNode, který reprezentuje budoucí políčko cesty Enemy
     */
    MapNode* m_nextPosition;

    /**
     * Počet skoků nutný k tomu, aby Enemy dosáhl cíle. Používá se pro ražení nepřátel
     */
    int m_mapNodesToEnd;

    /**
     * Vrácí výšku pro vykreslení čtverce ukazující životy nepřítele
     * @return
     */
    int getHpHeight();

protected:
    virtual bool canBeSelected();

public:
    int m_hp;
    int m_maxHp;

    explicit Enemy(const int& maxHp = -1);
    explicit Enemy(const Enemy& enemy);
    ~Enemy();

    /**
     * Funkce volaná v případě, že nepřítele dostane zásah od veže
     * @param hit veliost zásahu
     * @return vrací hodnotu isDead()
     */
    bool getHit(const int& hit);

    /**
     * Vrací oznámení o tom jeslti je nepřítele živí
     * @return TRUE pokud je živí
     */
    bool isAlive();

    /**
     * Vrací oznámení o tom jeslti je nepřítele mrtví
     * @return TRUE pokud je mrtví
     */
    bool isDead();

    /**
     * Porovná vzdálenosti nepřátel od cíle a vrací oznamení o tom, který je blíž
     * @param e porovnávaný nepřítel
     * @return TRUE pokud je aktuální nepřítel blíže než porovnávaný nepřítel, jinak FALSE
     */
    bool compareDistanceToEnd(const Enemy &e) const;

    /**
     * Nastaví cestu nepřítele do cíle
     * @param startNode startovní MapNode
     */
    void setPath(const MapNode& startNode);
    void printDistanceToEnd();
    MapNode* getNextPosition();

    /**
     * Pohnutí nepřítele
     */
    void enemyMove();

    /**
     * Zapíše nepřítele do souboru jako definici nepřítele
     * @param ofstream ofstram zapisovaného souboru
     * @param delimiter oddělovač mezi atributy
     */
    virtual void writeToFile(std::ofstream &ofstream, char delimiter);

    /**
     * Zapíše nepřítele do souboru jako, jako nepřítele v mapě, čili o zapíše jeho aktuální zdravý a pozici na mapě
     * @param ofstream ofstram zapisovaného souboru
     * @param delimiter oddělovač mezi atributy
     */
    virtual void writeToFileAsEnemyInMap(std::ofstream &ofstream, char delimiter);
    virtual void draw();
    virtual std::string getMapItemType();
    virtual bool isBlock();

};

/**
 * Třída reprezentující vež na mapě
 * Stará se o útočení věží na nepřátele. Atributem každé veže je její cena, útok a dosah na mapě
 */
class Tower : public MapItem{
private:
    int m_price, m_attack, m_range;

    /**
     * Ověřuje jestli je nepřítel v dosahu veže
     * @param enemy ověřovaný nepřítel
     * @return TRUE poku je nepřítel v dosahu jinak FALSE
     */
    bool isInRange(const Enemy& enemy);

public:

    explicit Tower(const Tower& tower);
    explicit Tower(const int& price = -1, const int& attack = -1,const int& range = -1);

    /**
     * Útok veže na prvního nepřítele v listu který je v dosahu veže
     * @param enemiesInMap list všech nepřátel na mapě
     */
    void attack(std::vector<Enemy*>& enemiesInMap);
    int getPrice() const;

    /**
     * Zapíše definice veže do souboru
     * @param ofstream ofstram zapisovaného souboru
     * @param delimiter oddělovač mezi atributy
     */
    virtual void writeToFile(std::ofstream &ofstream, char delimiter);
    virtual void draw();
    virtual std::string getMapItemType();


};

#endif //TOWER_DEFENDS_UIITEMS_H

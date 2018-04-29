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
 * Globální proměné napříč baličky uchovávající velikost okna
 */
extern int m_WindowWidth, m_WindowHeight;

/**
 * Základní view, uchováva souřadnice X a Y v kartézské soustavě a velikost view. Tyto věci potom převádí do vektorů v openGl.
 * Obsahule abstraktní funkci pro vykreslení všech poděděných view
 */
class UiItem {
private:
    /**
     * Převede souřadnici do souřednici ve vektorovém prostoru
     * @param position souřadnice k převedení
     * @param size velikost okna
     * @return souřadnice ve vektrovém prostoru openGl
     */
    static float getGlFloat(int position, const int& size);

protected:

    /**
     * Souřadnice view
     */
    int m_viewX, m_viewY;
    int m_width, m_height;

    /**
     * Souřadnice bodů v openGl
     */
    float m_vectorTop, m_vectorLeft, m_vectorRight, m_vectorBottom;

    /**
     * Nakreslí čtverec, který rozěry odpovída view
     * @param filled pokud má být čtverec vyplněný tak true jinak false
     */
    void drawAsSquare(bool filled = true);

    /**
     * Nakrasí čtverec odpovídající zadaným souřednicím
     * @param left levá strana čtverce ve vektoru
     * @param top horní strana čtverce ve vektoru
     * @param right pravá strana čtverce ve vektoru
     * @param bottom spodní strana čtverce ve vektoru
     * @param filled pokud má být čtverec vyplněný tak true jinak false
     */
    void drawAsSquare(const float& left, const float& top, const float& right, const float& bottom, bool filled = true);

    /**
     * Funkce, která získá šířku čáry v případě, že se čtverec kreslí nevyplněný
     * @return šířku čáry
     */
    virtual float getLineWidth();

public:

    explicit UiItem(int x = -1, int y = -1, int width = -1, int height = -1);

    /**
     * Převede souřadnici X do vektoru v opengl
     * @param position souřadnice X pozice
     * @return vektor souřadnice v openGL
     */
    static float getGlFloatX(const int& position);

    /**
     * Převede souřadnici Y do vektoru v opengl
     * @param position souřadnice Y pozice
     * @return vektor souřadnice v openGL
     */
    static float getGlFloatY(int position);

    /**
     * Indikace jestli zadané souřadnice jsou součástí view
     * @param x
     * @param y
     * @return TRUE pokud ano jinak FALSE
     */
    bool contains(int x, int y) const;

    /**
     * Indikace jestli zadané souřadnice jsou součástí view
     * @param x
     * @param y
     * @return TRUE pokud ano jinak FALSE
     */
    bool contains(float x, float y) const;

    /**
    * Nastaví polohu prvku a vypočítá i hodnoty pro OpenGl
    * @param x souřadnice na ose x
    * @param y souřadnice na ose y
    * @param width šířka objektu
    * @param height výška objektu
    */
    virtual void setPosition(int x, int y, int width, int height);

    /**
    * Funkce, která přepočítává souřednice x a y do reprezentace opengl
    */
    void calPosition();

    /**
     * Abstraktní funkce, která je voláná při vykreslování view
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

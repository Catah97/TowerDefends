//
// Created by Martin Beran on 11.04.18.
//

#ifndef TOWER_DEFENDS_UIITEMS_H
#define TOWER_DEFENDS_UIITEMS_H

extern int m_WindowWidth, m_WindowHeight;


class UiItems {
private:
    float getGlFloat(int position, const int& size);

protected:
    int m_x, m_y;
    int m_width, m_height;

    float getTopLeft();
    float getBottomLeft();
    float getTopRight();
    float getBottomRight();
    float getGlFloatX(const int& position);
    float getGlFloatY(const int& position);
public:

    UiItems(int x, int y, int width, int height);


    virtual void draw() = 0;
};

class Button : public UiItems{
public:


    Button(int x, int y, int width, int height);

    virtual void draw();
};

#endif //TOWER_DEFENDS_UIITEMS_H

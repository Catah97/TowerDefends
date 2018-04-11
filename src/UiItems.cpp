//
// Created by Martin Beran on 11.04.18.
//

#include <iostream>
#include "UiItems.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


UiItems::UiItems(int x, int y, int width, int height) : m_x(x),
                                                        m_y(y),
                                                        m_width(width),
                                                        m_height(height){}

float UiItems::getGlFloatX(const int& position) {
    return getGlFloat(position, m_WindowWidth);
}

float UiItems::getGlFloatY(const int& position) {
    return getGlFloat(position, m_WindowHeight);
}

float UiItems::getGlFloat(int position, const int& size) {
    std::cout << "position " << position << std::endl;
    float middle = size / 2;
    position -= middle;
    float result = position / middle;
    std::cout << "result " << result << std::endl;
    return result;
}



//-------------Button------------------------------------------------

Button::Button(int x, int y, int width, int height) : UiItems(x, y, width, height) {}


void Button::draw() {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex2f( getGlFloatX(m_x), getGlFloatX(m_y));
    glVertex2f( getGlFloatX(m_x + m_width), getGlFloatX(m_y));
    glVertex2f( getGlFloatX(m_x + m_width), getGlFloatX(m_y + m_height));
    glVertex2f( getGlFloatX(m_x), getGlFloatX(m_y + m_height));
}


//
// Created by Martin Beran on 20.04.18.
//

#include "BottomBar.h"


//-------------BottomBar-------

void BottomToolbar::draw() {
#ifdef __OPENGL__
    glColor3f(1.0f, 1.0f, 1.0f);
    drawAsSquare();
    for (auto live : m_livesView){
        live->draw();
    }
    for (auto towerBtn : m_towersButtonsView){
        towerBtn->draw();
    }

    //DrawMoney

    glColor3f(0.0f, 0.0f, 0.0f);
    glDisable(GL_LIGHTING);

    std::string price = std::to_string(m_gameCommunicator->getAvailableMoney());
    glRasterPos2d( getGlFloatX( m_width - price.size() * 12 ) , getGlFloatY( m_viewY + m_height) ) ;
    for (size_t i = 0; i < price.size(); ++i) {
        glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, price.at(i) );
    }
#endif
}


void BottomToolbar::setLives(int lives) {
    deleteLivesViews();
    for (auto i = 1; i <= lives; ++i) {
        int liveX = m_width - s_liveWidth * i;
        auto uiItem = new LiveView(liveX, m_viewY, s_liveWidth, s_liveHeight);
        m_livesView.push_back(uiItem);
    }
}

void BottomToolbar::deleteLivesViews() {
    for (auto liveView : m_livesView){
        delete liveView;
    }
    m_livesView.clear();
}

void BottomToolbar::setDefinedTowers(const std::vector<Tower *> &definedTowers) {
    deleteDefinedTowersView();
    int padding = 8;
    for (size_t i = 0; i < definedTowers.size(); ++i) {
        auto towerBtn = new TowerSelectButton(*definedTowers[i]);
        towerBtn->setPosition( i * (padding + s_towerBtnWidth) , padding + m_viewY, s_towerBtnWidth, s_towerBtnHeight);
        m_towersButtonsView.push_back(towerBtn);
    }

    int selectedPos = 0;
    m_towersButtonsView[selectedPos]->m_selected = true;
    m_gameCommunicator->onTowerSelected(selectedPos);
}

void BottomToolbar::deleteDefinedTowersView() {
    for (auto liveView : m_towersButtonsView){
        delete liveView;
    }
    m_towersButtonsView.clear();
}

BottomToolbar::~BottomToolbar() {
    deleteLivesViews();
    deleteDefinedTowersView();
}

BottomToolbar::BottomToolbar(BottomToolbarCommunicator *gameCommunicator) : m_gameCommunicator(gameCommunicator) {}

bool BottomToolbar::onMouseClick(int x, int y) {
    for (size_t i = 0; i < m_towersButtonsView.size(); i++){
        auto towerView = m_towersButtonsView[i];
        if (towerView->contains(x, y)){
            m_towersButtonsView[m_gameCommunicator->getSelectedTowerPosition()]->m_selected = false;
            m_towersButtonsView[i]->m_selected = true;
            m_gameCommunicator->onTowerSelected(i);
            return true;
        }
    }
    return false;
}



//----------------Live--------------------

LiveView::LiveView(int x, int y, int width, int height) : UiItem(x, y, width, height) {

}

void LiveView::draw() {
#ifdef __OPENGL__

    glColor3f(1.0f, 0.0f, 0.0f);
    drawAsSquare();
    glColor3f(0.0f, 0.0f, 0.0f);
    drawAsSquare(false);

#endif
}

//---------------TowerSelectButton--------------

TowerSelectButton::TowerSelectButton(const Tower &tower) : m_definedTower(new Tower(tower)), m_selected(false) {}

void TowerSelectButton::draw() {
#ifdef __OPENGL__

    glColor3f(0.0f, 1.0f, 0.0f);
    drawAsSquare();
    if (m_selected) {
        glColor3f(0.0f, 0.0f, 0.0f);
        drawAsSquare(false);
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    glDisable(GL_LIGHTING);

    glRasterPos2d( m_vectorLeft , m_vectorBottom ) ;
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, m_definedTower->m_mapItemChar );

    glColor3f(0.0f, 0.0f, 0.0f);
    glDisable(GL_LIGHTING);

    glRasterPos2d( m_vectorLeft , getGlFloatY( m_viewY + m_height / 3) ) ;
    std::string price = std::to_string(m_definedTower->getPrice());
    for (size_t i = 0; i < price.size(); ++i) {
        glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_10, price.at(i) );
    }

#endif
}

float TowerSelectButton::getLineWidth() {
    return 2.0f;
}

TowerSelectButton::~TowerSelectButton() {
    delete m_definedTower;
}

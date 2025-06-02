#include "../header/Menu.hpp"
#include "../header/SceneHandler.hpp"
void Menu::updateScene() {
    Button::updateButtons<RectButton>(RectButtons);

}

void Menu::displayScene() {
    Button::drawButtons(RectButtons);
}

void Menu::init() {
    initButtons();
}

void Menu::initButtons() {
    RectButton::insertHeadButton(RectButtons, new TextBox("Play", UI::screenWidth / 2, UI::screenHeight / 2, WHITE, BROWN, BLACK));
    RectButtons[0]->onClick = [this]() {
        scenehandler->changeScene(GAME);
    };
    RectButton::insertHeadButton(RectButtons, new TextBox("Settings"));
}
#include "Menu.hpp"
#include "Program.hpp"


void Menu::updateScene() {
    Button::updateButtons<RectButton>(RectButtons);
}

void Menu::displayScene() {
    UI::drawBackground();
    UI::drawLogo();
    Button::drawButtons(RectButtons);
}
 
void Menu::init() {
    RectButton::insertHeadButton(RectButtons, new TextBox("Play", UI::screenWidth / 2, UI::screenHeight / 2, WHITE, BROWN, BLACK));
    RectButtons[0]->onClick = [this]() {
        scenehandler->changeScene(GAME);
        };
    RectButton::insertHeadButton(RectButtons, new TextBox("Settings"));
}


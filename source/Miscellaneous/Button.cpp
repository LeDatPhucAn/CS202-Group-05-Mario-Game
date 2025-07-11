#include "Button.hpp"
#include "Program.hpp"
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

bool Button::isCollision = false;
bool Button::isClicking = false;
const int RectButton::padding = UI::fontSize;

// ### Button Utility Functions

Vector2 Button::getMousePos() const {
    return GetMousePosition();
}

void Button::hover() {
    if (!isHovered) {
        UI::darkenColor(FillColor, 30);
        UI::darkenColor(TextColor, 30);
        isHovered = true;
    }
}

void Button::unhover() {
    if (isHovered) {
        UI::lightenColor(FillColor, 30);
        UI::lightenColor(TextColor, 30);
        isHovered = false;
    }
}

void Button::click() {
    if (!isClicked) {
        UI::lightenColor(FillColor, 30);
        UI::lightenColor(TextColor, 30);
        isClicked = true;
    }
}

void Button::unclick() {
    if (isClicked) {
        UI::darkenColor(FillColor, 30);
        UI::darkenColor(TextColor, 30);
        isClicked = false;
    }
}
void Button::update() {
    if (useless) return;
    if (checkCollision()) {
        hover();
        setCursor();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            click();
            Button::isClicking = true;
        }
        else {
            Button::isClicking = false;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            isActivated = !isActivated;
            if (onClick) onClick();
            unclick();
        }
        Button::isCollision = true;
    }
    else {
        unhover();
    }
    //if (animation && !animation->isCompleted()) animation->update(GetFrameTime());
}
// ### RectButton Methods
void RectButton::resetSubAni() {
    RectButton* cur = this->next;
    while (cur) {
        //if (cur->animation) cur->animation->reset();
        cur = cur->next;
    }
}

void RectButton::setPosition(float x, float y) {
    rect.x = x;
    rect.y = y;
}

void RectButton::setSubPosition() {
    RectButton* prev = this;
    RectButton* cur = this->next;
    while (cur) {
        cur->setPosition(prev->rect.x + prev->rect.width + padding / 2, prev->rect.y);
        //if (cur->animation) cur->animation->handleReposition();
        prev = cur;
        cur = cur->next;
    }
}

void RectButton::setHeadPosition(std::vector<RectButton*>& buttons, float x, float y) {
    if (buttons.empty()) {
        std::cout << "Your Button is missing\n";
        return;
    }
    buttons[0]->setPosition(x, y);
    buttons[0]->setSubPosition();
    //if (buttons[0]->animation) buttons[0]->animation->handleReposition();
    for (size_t i = 1; i < buttons.size(); ++i) {
        RectButton* prevHead = buttons[i - 1];
        RectButton* curHead = buttons[i];
        curHead->setPosition(prevHead->rect.x, prevHead->rect.y + prevHead->rect.height);
        //if (curHead->animation) curHead->animation->handleReposition();
        curHead->setSubPosition();
    }
}

void RectButton::setCodeBlockPosition(std::vector<RectButton*>& CodeBlocks, float x, float y) {
    if (CodeBlocks.empty()) {
        std::cout << "Your Button is missing\n";
        return;
    }
    if (CodeBlocks.size() == 1) {
        CodeBlocks[0]->setPosition(x, y);
        return;
    }
    CodeBlocks[0]->setPosition(x, y);
    CodeBlocks[1]->setPosition(CodeBlocks[0]->rect.x - CodeBlocks[1]->rect.width, CodeBlocks[0]->rect.y);
    for (size_t i = 2; i < CodeBlocks.size(); ++i) {
        RectButton* curHead = CodeBlocks[i];
        RectButton* prevHead = CodeBlocks[i - 1];
        curHead->setPosition(prevHead->rect.x, prevHead->rect.y + prevHead->rect.height);
    }
}

void RectButton::insertHeadButton(std::vector<RectButton*>& buttons, RectButton* button) {
    if (buttons.empty()) {
        buttons.push_back(button);
        return;
    }
    RectButton* prev = buttons.back();
    buttons.push_back(button);
    if (button->rect.width <= prev->rect.width) {
        button->rect.width = prev->rect.width;
    }
    else {
        for (size_t i = 0; i < buttons.size() - 1; ++i) {
            buttons[i]->rect.width = button->rect.width;
        }
    }
    button->rect.x = prev->rect.x;
    button->rect.y = prev->rect.y + prev->rect.height;
}

void RectButton::insertSubButton(RectButton* button) {
    RectButton* cur = this;
    while (cur->next) cur = cur->next;
    cur->next = button;
    button->head = this;
    button->rect = { cur->rect.x + cur->rect.width + padding / 2, cur->rect.y, button->rect.width, button->rect.height };
    //button->animation = new RectMoveXAnim(button, this->rect.x, 0.3);
}

void RectButton::insertSubButton(RectButton* button, std::function<void()> function) {
    insertSubButton(button);
    button->onClick = function;
}

void RectButton::insertCodeBlock(std::vector<RectButton*>& CodeBlocks, RectButton* codeblock) {
    if (CodeBlocks.empty()) {
        CodeBlocks.push_back(codeblock);
        return;
    }
    RectButton* prev = CodeBlocks.back();
    CodeBlocks.push_back(codeblock);
    if (codeblock->rect.width <= prev->rect.width) {
        codeblock->rect.width = prev->rect.width;
        codeblock->rect.x = prev->rect.x;
    }
    else {
        for (size_t i = 1; i < CodeBlocks.size(); ++i) {
            CodeBlocks[i]->rect.width = codeblock->rect.width;
            CodeBlocks[i]->rect.x = CodeBlocks[0]->rect.x - codeblock->rect.width;
        }
    }
    codeblock->head = CodeBlocks[0];
    codeblock->rect.y = prev->rect.y + prev->rect.height;
    CodeBlocks[0]->rect.height += codeblock->rect.height;
}

void RectButton::insertPseudoCode(std::vector<RectButton*>& CodeBlocks, std::string pseudocode) {
    if (CodeBlocks.size() > 1) {
        RectButton* head = CodeBlocks[0];
        for (size_t i = 1; i < CodeBlocks.size(); ++i) {
            delete CodeBlocks[i];
        }
        CodeBlocks.clear();
        CodeBlocks.push_back(head);
        head->rect.height = 0;
    }
    else if (CodeBlocks.empty()) {
        return;
    }
    std::stringstream ss(pseudocode);
    std::string line;
    while (std::getline(ss, line, '\n')) {
        insertCodeBlock(CodeBlocks, new CodeBlock(line.c_str()));
    }
}
bool RectButton::checkCollision() {
    return CheckCollisionPointRec(getMousePos(), rect);
}
void RectButton::update() {
    if (!head || head->isActivated) {
        Button::update();
        if (checkCollision()) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (!head && isActivated) resetSubAni();
            }
        }
    }
    if (next) next->update();
}

void RectButton::draw() {
    if (noDraw)return;
    if (!head || head->isActivated) {
        DrawRectangleRec(rect, FillColor);
        DrawRectangleLines((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, OutLineColor);
    }
    if (next) next->draw();
}

void ScrollyAndButton::update() {
    if (isActivated && Button::isClicking) {
        MoveableButton.x = getMousePos().x - MoveableButton.width / 2;
        clamp();
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        isActivated = false;
    }
    if (checkCollision()) {
        hover();
        setCursor();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            click();
            isActivated = true;
            Button::isClicking = true;
            MoveableButton.x = getMousePos().x - MoveableButton.width/2;
            clamp();
        }
        else {
            Button::isClicking = false;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (onClick) onClick();
            unclick();
        }
        Button::isCollision = true;
    }
    else {
        unhover();
    }
    //if (animation && !animation->isCompleted()) animation->update(GetFrameTime());

    // update the value represented in the scrolly
    float x = (MoveableButton.x - rect.x + MoveableButton.width/2) / rect.width;

    value = (valueMax - valueMin) * x + valueMin;
}

void ScrollyAndButton::draw() {
    
    // draw scrolly
    DrawRectangleRec(rect, FillColor);
    DrawRectangleLines((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, OutLineColor);

    // draw button
    DrawRectangleRounded(MoveableButton, 0.7f, 100, TextColor);

    UI::drawtext2("Speed: " + to_string(value) + "x", rect.x + rect.width / 2, MoveableButton.y - MoveableButton.height / 2, BLACK);
}
// ### InputBox Methods
void InputBox::hover() {
    if (!isHovered) {
        UI::darkenColor(FillColor, 30);
        UI::darkenColor(TextColor, 30);
        isHovered = true;
    }
    //OutLineColor = RED;
}

void InputBox::unhover() {
    if (isHovered) {
        UI::lightenColor(FillColor, 30);
        UI::lightenColor(TextColor, 30);
        isHovered = false;
    }
    //OutLineColor = DARKGRAY;
}

void InputBox::update() {
    if (GetGestureDetected() == GESTURE_TAP) {
        inputHandler->setTexting(false);
    }
    if (!head || head->isActivated) {
        Button::update();
        if (checkCollision()) {
            if (GetGestureDetected() == GESTURE_TAP) inputHandler->setTexting(true);
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (!head && isActivated) resetSubAni();
            }
        }
        inputHandler->update();
        if (inputHandler->isTexting() && IsKeyPressed(KEY_ENTER) && onClick) onClick();
    }
    if (next) next->update();
}

void InputBox::draw() {
	if (noDraw) return;
    if (!head || head->isActivated) {
        DrawRectangleRec(rect, FillColor);
        if (inputHandler->isTexting() && (inputHandler->getFramesCounter() / 20) % 2 == 0) {
            std::string underscore = inputHandler->getText() + "_";
            Vector2 textSize = MeasureTextEx(UI::font, inputHandler->getText().c_str(), UI::fontSize, UI::spacing);
            DrawText(underscore.c_str(), rect.x + rect.width / 2 - textSize.x / 2,
                rect.y + rect.height / 2 - UI::fontSize / 2, UI::fontSize, TextColor);
        }
        else UI::drawtext2(inputHandler->getText(), rect.x + rect.width / 2, rect.y + rect.height / 2, TextColor);

        DrawRectangleLines((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, OutLineColor);
    }
    if (next) next->draw();
}


void DelayInputBox::update() {
    if (GetGestureDetected() == GESTURE_TAP) {
        inputHandler->temporaryText = inputHandler->getText();
        inputHandler->setTexting(false);
    }
    if (!head || head->isActivated) {
        Button::update();
        if (checkCollision()) {
            if (GetGestureDetected() == GESTURE_TAP) inputHandler->setTexting(true);
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (!head && isActivated) resetSubAni();
            }
        }
        inputHandler->update();
        if (inputHandler->isTexting() && IsKeyPressed(KEY_ENTER) && onClick) onClick();
    }
    if (next) next->update();
}

void DelayInputBox::draw() {
    if (noDraw) return;
    if (!head || head->isActivated) {
        DrawRectangleRec(rect, FillColor);
        string displayText;
        if (inputHandler->isTexting()) {
            displayText = inputHandler->temporaryText;

            if ((inputHandler->getFramesCounter() / 20) % 2 == 0) {
                displayText += "_";
            }
        }
        else {
            displayText = inputHandler->getText();
        }

        Vector2 textSize = MeasureTextEx(UI::font, displayText.c_str(), UI::fontSize, UI::spacing);
        DrawText(displayText.c_str(), rect.x + rect.width / 2 - textSize.x / 2,
            rect.y + rect.height / 2 - UI::fontSize / 2, UI::fontSize, TextColor);

        DrawRectangleLines((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, OutLineColor);
    }
}

void DelayInputBox::hover() {
    if (!isHovered) {
        UI::darkenColor(FillColor, 30);
        UI::darkenColor(TextColor, 30);
        isHovered = true;
    }
    //OutLineColor = RED;
}

void DelayInputBox::unhover() {
    if (isHovered) {
        UI::lightenColor(FillColor, 30);
        UI::lightenColor(TextColor, 30);
        isHovered = false;
    }
    //OutLineColor = DARKGRAY;
}

// NumberInputBox::draw and update and clear inherits from InputBox

// ### TextBox Methods

// inherit update from RectBox
void TextBox::draw() {
	if (noDraw) return;
    if (!head || head->isActivated) {
        DrawRectangleRec(rect, FillColor);
        DrawRectangleLines((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, OutLineColor);
        DrawTextEx(UI::font, Text.c_str(), { rect.x + padding / 2, rect.y + rect.height / 2 - padding / 2 },
            UI::fontSize, UI::spacing, TextColor);
    }
    if (next) next->draw();
}

void CodeBlock::highlight() {
   isHighlight = true;
   TextColor = WHITE;
   FillColor = RED;
}
void CodeBlock::unhighlight() {
    isHighlight = false;
    TextColor = DARKGRAY;
    FillColor = CodeColor;
}
void CodeBlock::hover() {
    if (isHighlight)return;
    Button::hover();
}
// ### CircleButton Methods
bool CircleButton::checkCollision() {
    return CheckCollisionPointCircle(getMousePos(), center, radius);
}
void CircleButton::hover() {
    OutLineColor = RED;
    TextColor = RED;
}

void CircleButton::unhover() {
    OutLineColor = OgOutLineColor;
    TextColor = OgTextColor;
}
void CircleButton::click() {
    if (!isClicked) {
        UI::darkenColor(FillColor, 30);
        UI::darkenColor(TextColor, 30);
        isClicked = true;
    }
}
void CircleButton::unclick() {
    if (isClicked) {
        UI::lightenColor(FillColor, 30);
        UI::lightenColor(TextColor, 30);
        isClicked = false;
    }
}
CircleButton::CircleButton(Vector2 cent = { 0, 0 }, float r = 50.0f,
    Color tc = BLUE, Color fc = RAYWHITE, Color rc = BLUE)
    : Button(tc, fc, rc), center(cent), radius(r) {
    //animation = new CircleInitializeAnim(this, 1);
}
void CircleButton::update() {
    Button::update();
}
void TextCircle::unhover() {
    OutLineColor = OgOutLineColor;
    FillColor = OgFillColor;
}

void TextCircle::hover() {
    OutLineColor = DARKGREEN;
    FillColor = GREEN;
}
void TextureCircle::unhover() {
    OutLineColor = OgOutLineColor;
    FillColor = OgFillColor;
}

void TextureCircle::hover() {
    OutLineColor = DARKGREEN;
    FillColor = GREEN;
}
void TextCircle::draw() {
	if (noDraw) return;
    DrawCircleV(center, radius * 4 / 5 +1, FillColor);
    DrawRing(center, radius * 4 / 5, radius, 0, 360, 100, OutLineColor);
    UI::drawtext2(Text, center.x, center.y, TextColor);
}

void TextureCircle::draw() {
	if (noDraw) return;
    float RingRadius = radius * 4 / 5;
    DrawCircleV(center, RingRadius + 1, FillColor);
    DrawRing(center, RingRadius, radius, 0, 360, 100, OutLineColor);

    DrawTexturePro(Texture,
        { 0,0,(float)Texture.width,(float)Texture.height },
        { center.x - RingRadius,center.y - RingRadius,RingRadius*2,RingRadius*2},
        { 0,0 }, 0, TextColor
    );
}
void TextureCircle::changeTexture(Texture2D texture) {
    Texture = texture;
}
void PlayButton::update() {
    if (scenehandler->getCurrentScene() == MENU) {
        return;
    }
    //if (scenehandler->currentSceneObject->animManager.isPaused()) {
        //changeTexture(UI::Icons[5]);
    //}
    //else changeTexture(UI::Icons[4]);
    Button::update();
}
// ### InputCircle Methods
void InputCircle::update() {
    if (GetGestureDetected() == GESTURE_TAP) inputHandler->setTexting(false);
    Button::update();
    if (checkCollision()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) inputHandler->setTexting(true);
    }
    inputHandler->update();
    if (inputHandler->isTexting() && IsKeyPressed(KEY_ENTER) && onClick) onClick();
}
void InputCircle::draw() {
	if (noDraw) return;
    DrawCircleV(center, radius * 4 / 5, FillColor);
    DrawRing(center, radius * 4 / 5, radius, 0, 360, 100, OutLineColor);
    UI::drawtext2(inputHandler->getText(), center.x, center.y, TextColor);
    if (inputHandler->isTexting() && (inputHandler->getFramesCounter() / 20) % 2 == 0) {
        std::string underscore = inputHandler->getText() + "_";
        Vector2 textSize = MeasureTextEx(UI::font, inputHandler->getText().c_str(), UI::fontSize, UI::spacing);
        DrawText(underscore.c_str(), center.x - textSize.x / 2, center.y - UI::fontSize / 2,
            UI::fontSize, TextColor);
    }
}

Vector2 NumberInputBoxInCamera::getMousePos() const { return Program::mouseWorldPos; }
Vector2 NumberInputCircleInCamera::getMousePos() const { return Program::mouseWorldPos; }
Vector2 TextBoxInCamera::getMousePos() const { return Program::mouseWorldPos; }
Vector2 DelayNumberInputBoxInCamera::getMousePos() const { return Program::mouseWorldPos; }

void NumberInputCircleInCamera::Indicate(string Text){
    UI::drawtext2(Text, center.x, center.y + radius + 20, RED);
}
void NumberInputCircleInCamera::draw() {
    if (indicateNode !="") Indicate(indicateNode);
    InputCircle::draw();
}
void TextBoxInCamera::draw() {
    if (noDraw) return;
    if (!head || head->isActivated) {
        DrawRectangleRec(rect, FillColor);
        DrawRectangleLines((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, OutLineColor);
        DrawTextEx(UI::font, Text.c_str(), { rect.x + padding / 2, rect.y + rect.height / 2 - padding / 2 },
            UI::fontSize, UI::spacing, TextColor);
    }
    if (next) next->draw();
}

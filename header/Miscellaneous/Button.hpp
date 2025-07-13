#pragma once
#include <functional>
#include "raylib.h"
#include "UI.hpp"
#include <sstream>
#include <memory>
// ### InputHandler Class
// used for inputText
class InputHandler {
protected:
    std::string inputText;
    bool texting;
    double lastDeletedTime;
    int maxChars;
    int framesCounter;

public:
    InputHandler(int maxCh)
        : inputText(""), texting(false), lastDeletedTime(0), maxChars(maxCh), framesCounter(0) {
    }

    virtual ~InputHandler() = default;

    virtual bool isAllowedChar(char c) const { return c >= 32 && c <= 125; } // Printable ASCII

    virtual void update() {
        framesCounter++;
        if (!texting) return;

        int key = GetCharPressed();
        while (key > 0) {
            if (isAllowedChar(static_cast<char>(key)) && inputText.size() < maxChars) {
                inputText += static_cast<char>(key);
            }
            key = GetCharPressed();
        }

        double currentTime = GetTime();
        if (IsKeyPressed(KEY_BACKSPACE) && currentTime - lastDeletedTime >= 0.1 && !inputText.empty()) {
            inputText.pop_back();
            lastDeletedTime = currentTime;
        }
    }
    void setText(string s) { inputText = s; }
    const std::string& getText() const { return inputText; }
    bool isTexting() const { return texting; }
    void setTexting(bool state) { texting = state; }
    virtual void clear() { inputText.clear(); }
    int getFramesCounter() const { return framesCounter; }
};

class DelayInputHandler {  
protected:  
   std::string inputText;  
   bool texting;  
   double lastDeletedTime;  
   int maxChars;  
   int framesCounter;  

public:  
   std::string temporaryText;  
   DelayInputHandler(int maxCh)  
       : inputText(""), temporaryText(""), texting(false), lastDeletedTime(0), maxChars(maxCh), framesCounter(0) {  
   }  

   virtual ~DelayInputHandler() {} 

   virtual bool isAllowedChar(char c) const { return c >= 32 && c <= 125; } // Printable ASCII  

   virtual void update() {  
       framesCounter++;  
       if (!texting) return;  

       int key = GetCharPressed();  
       while (key > 0) {  
           if (isAllowedChar(static_cast<char>(key)) && temporaryText.size() < maxChars) {  
               temporaryText += static_cast<char>(key);  
           }  
           key = GetCharPressed();  
       }  

       double currentTime = GetTime();  
       if (IsKeyPressed(KEY_BACKSPACE) && currentTime - lastDeletedTime >= 0.1 && !temporaryText.empty()) {  
           temporaryText.pop_back();  
           lastDeletedTime = currentTime;  
       }  

       if (IsKeyPressed(KEY_ENTER)) {  
           finalizeInput();  
       }  
   }  
   void setText(string s) {  
       inputText = s;  
       temporaryText = s;  
   }  
   const std::string& getText() const {  
       return inputText;  
   }  
   bool isTexting() const {  
       return texting;  
   }  
   void setTexting(bool state) {  
       texting = state;  
       if (state)  temporaryText = inputText;  
   }  
   void finalizeInput() {  
       inputText = temporaryText;  
       texting = false;  
   }  
   virtual void clear() {  
       inputText.clear();  
       temporaryText.clear();  
   }  
   int getFramesCounter() const { return framesCounter; }  
};

// ### NumericInputHandler Subclass
// used for inputNumber
class NumericInputHandler : public InputHandler {
public:
    NumericInputHandler(int maxCh) : InputHandler(maxCh) {}
    bool isAllowedChar(char c) const override { return c >= '0' && c <= '9'; }

    int getNumber() const { 
        return getText().empty() ? 0 : std::stoi(getText()); }
};

class DelayNumericInputHandler : public DelayInputHandler {
public:
    DelayNumericInputHandler(int maxCh) : DelayInputHandler(maxCh) {}
    bool isAllowedChar(char c) const override { return c >= '0' && c <= '9'; }

    int getNumber() const {
        return getText().empty() ? 0 : std::stoi(getText());
    }
};

class Animation;
class Button {
public:
    Color OgTextColor;
    Color OgFillColor;
    Color OgOutLineColor;
    Color TextColor;
    Color FillColor;
    Color OutLineColor;
    Animation* animation;
    bool isHovered;
    bool isClicked;
    bool isActivated;
    bool useless;
    bool noDraw;
    static bool isCollision;
    static bool isClicking;
    std::function<void()> onClick;
    Button(Color tc = WHITE, Color fc = BLUE, Color olc = DARKGRAY)
        : TextColor(tc), FillColor(fc), OutLineColor(olc),
        OgTextColor(tc), OgFillColor(fc), OgOutLineColor(olc),
        animation(nullptr), isHovered(false), isClicked(false), isActivated(false), useless(false), noDraw(false) {
    }
    virtual Vector2 getMousePos() const;
    /**
     * Deletes all buttons in the provided vector and clears the vector.
     * @tparam T Type of the button, must be derived from Button.
     * @param buttons Vector of pointers to buttons to be deleted.
     */
    template <typename T>
    static void deleteButtons(std::vector<T*>& Buttons) {
        static_assert(std::is_base_of<Button, T>::value, "T must be derived from Button");
        for (auto btn : Buttons) {
            delete btn;
        }
        Buttons.clear();
    }

    /**
     * Draws all buttons in the provided vector.
     * @tparam T Type of the button, must be derived from Button.
     * @param buttons Vector of pointers to buttons to be drawn.
     */
    template <typename T>
    static void drawButtons(const std::vector<T*>& Buttons) {
        static_assert(std::is_base_of<Button, T>::value, "T must be derived from Button");
        for (auto btn : Buttons) {
            btn->draw();
        }
    }

    /**
     * Updates all buttons in the provided vector.
     * @tparam T Type of the button, must be derived from Button.
     * @param buttons Vector of pointers to buttons to be updated.
     */
    template <typename T>
    static void updateButtons(std::vector<T*>& Buttons) {
        static_assert(std::is_base_of<Button, T>::value, "T must be derived from Button");
        for (auto btn : Buttons) {
            btn->update();
        }
    }
    /**
     * Reset all animations in buttons in the provided vector.
     * @tparam T Type of the button, must be derived from Button.
     * @param buttons Vector of pointers to buttons to be updated.
     */
    template <typename T>
    static void resetButtonsAnimations(std::vector<T*>& Buttons) {
        static_assert(std::is_base_of<Button, T>::value, "T must be derived from Button");
        //for (auto btn : Buttons) {
        //    if(btn->animation)btn->animation->reset();
        //}
    }
    /**
     * Reset all animations in buttons in the provided vector.
     * @tparam T Type of the button, must be derived from Button.
     * @param buttons Vector of pointers to buttons to be updated.
     */
    template <typename T>
    static void handleButtonsAnimReposition(std::vector<T*>& Buttons) {
        static_assert(std::is_base_of<Button, T>::value, "T must be derived from Button");
        for (auto btn : Buttons) {
            if(btn->animation)btn->animation->handleReposition();
        }
    }
    virtual void highlight() {};
    virtual void unhighlight() {};
    virtual void draw() = 0;
    virtual void update();
    virtual void hover();
    virtual void unhover();
    virtual void click();
    virtual void unclick();
    virtual bool checkCollision() = 0;
    virtual void setCursor() {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
};

// ### RectButton Class
// Uses Rectangle in Raylib to create a button
class RectButton : public Button {
public:
    static const int padding;
    Rectangle rect;

    RectButton* head;
    RectButton* next; // Pointer to the next button

    RectButton(float x = 0, float y = 0, float w = 0, float h = 0,
        Color tc = WHITE, Color fc = BLUE, Color olc = DARKGRAY)
        : Button(tc, fc, olc), rect{ x, y, w, h }
        , head(nullptr), next(nullptr) {
    }

    virtual ~RectButton() {
        if (animation)delete animation;
        if(next) delete next;
    }
    void update() override;
    void draw() override;
    bool checkCollision() override;
    virtual int getNumber() const { return 0; }
    virtual void setPosition(float x, float y);
    virtual void setSubPosition();

    static void setHeadPosition(vector<RectButton*>& Buttons, float x, float y);
    static void setCodeBlockPosition(vector<RectButton*>& CodeBlocks, float x, float y);
    static void insertHeadButton(vector<RectButton*>& Buttons, RectButton* button);
    static void insertCodeBlock(vector<RectButton*>& Buttons, RectButton* button);
    static void insertPseudoCode(vector<RectButton*>& CodeBlocks, string pseudocode);
    

    virtual void resetSubAni();
    virtual void insertSubButton(RectButton* button);
    virtual void insertSubButton(RectButton* button, std::function<void()> function);
};


class ScrollyAndButton: public RectButton {
protected:
    float value;
    float valueMin, valueMax;
    Rectangle MoveableButton;
public:
    ScrollyAndButton(float x = 0, float y = 0, int scrollyLength=300, int scrollyThickness=20, float buttonSize=50,
        Color ButtonColor = BLUE, Color ScrollyColor = WHITE, Color ScrollyLineColor = DARKGRAY, 
        float val = 1, float valMin = 0.5f, float valMax = 2)
        : RectButton(x, y, scrollyLength, scrollyThickness, ButtonColor, ScrollyColor, ScrollyLineColor),
        MoveableButton({ x - buttonSize/2, y - (buttonSize - scrollyThickness)/2,buttonSize,buttonSize }),
        value(val), valueMin(valMin), valueMax(valMax){
    }

    void update() override;
    void draw() override;
    float getValue() {
        return value;
    }
    void clamp() {

        int minclamp = rect.x - MoveableButton.width / 2;
        int maxclamp = rect.width + rect.x - MoveableButton.width / 2;

        if (MoveableButton.x < minclamp)MoveableButton.x = minclamp;

        else if (MoveableButton.x > maxclamp) MoveableButton.x = maxclamp;
    }

    bool checkCollision() override {
        return CheckCollisionPointRec(getMousePos(), MoveableButton);
    }
    void setPosition(float x, float y) override {

        MoveableButton.x += x - rect.x;
        MoveableButton.y = y - (MoveableButton.height-rect.height)/2;
        rect.x = x;
        rect.y = y;

    }
};

// Uses RectButton and Input Handler to create a button with text
class InputBox : public RectButton {
public:
    std::unique_ptr<InputHandler> inputHandler;
    InputBox(int maxCh, float x = 0, float y = 0,
        Color tc = MAROON, Color fc = LIGHTGRAY, Color olc = DARKGRAY)
        : RectButton(x, y, 0, 0, tc, fc, olc), 
        inputHandler(std::make_unique<InputHandler>(maxCh)) {
        Vector2 textSize = UI::getMaxTextSize(maxCh - 1);
        rect.width = textSize.x + padding;
        rect.height = textSize.y + padding;
    }

    void update() override;
    void draw() override;
    void hover() override;
    void unhover() override;
    virtual void clear() {
        inputHandler->clear();
    }
    void setCursor() override {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
};

class DelayInputBox : public RectButton {
public:
    std::unique_ptr<DelayInputHandler> inputHandler;
    DelayInputBox(int maxCh, float x = 0, float y = 0,
        Color tc = MAROON, Color fc = LIGHTGRAY, Color olc = DARKGRAY)
        : RectButton(x, y, 0, 0, tc, fc, olc),
        inputHandler(std::make_unique<DelayInputHandler>(maxCh)) {
        Vector2 textSize = UI::getMaxTextSize(maxCh - 1);
        rect.width = textSize.x + padding;
        rect.height = textSize.y + padding;
    }

    void update() override;
    void draw() override;
    void hover() override;
    void unhover() override;
    virtual void clear() {
        inputHandler->clear();
    }
    void setCursor() override {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
};

// Uses RectButton and Number Input Handler to create a button with text
class NumberInputBox : public InputBox {
public:
    NumberInputBox(int maxCh, float x = 0, float y = 0,
        Color tc = MAROON, Color fc = LIGHTGRAY, Color olc = DARKGRAY)
        : InputBox(maxCh, x, y, tc, fc, olc) {
        inputHandler = std::make_unique<NumericInputHandler>(maxCh);
        Vector2 textSize = UI::getMaxTextSize(maxCh+2);
        rect.width = textSize.x + padding;
        rect.height = textSize.y + padding;
    }

    int getNumber() const { return dynamic_cast<NumericInputHandler*>(inputHandler.get())->getNumber(); }
    void setNumber(int x) {
        inputHandler->setText(std::to_string(x));
    }
    const int getWidth() const {
        return rect.width;
    }
    const int getHeight() const {
        return rect.height;
    }
};

class DelayNumberInputBox : public DelayInputBox {
public:
    DelayNumberInputBox(int maxCh, float x = 0, float y = 0,
        Color tc = MAROON, Color fc = LIGHTGRAY, Color olc = DARKGRAY)
        : DelayInputBox(maxCh, x, y, tc, fc, olc) {
        inputHandler = std::make_unique<DelayNumericInputHandler>(maxCh);
        Vector2 textSize = UI::getMaxTextSize(maxCh + 2);
        rect.width = textSize.x + padding;
        rect.height = textSize.y + padding;
    }

    int getNumber() const { return dynamic_cast<DelayNumericInputHandler*>(inputHandler.get())->getNumber(); }
    void setNumber(int x) {
        inputHandler->setText(std::to_string(x));
    }
    const int getWidth() const {
        return rect.width;
    }
    const int getHeight() const {
        return rect.height;
    }
};

// NumberInputBoxInCamera is NumberInputBox but uses the camera position to get the mouse position
class NumberInputBoxInCamera : public NumberInputBox {
public:
    NumberInputBoxInCamera(int maxCh, float x = 0, float y = 0,
        Color tc = MAROON, Color fc = LIGHTGRAY, Color olc = DARKGRAY)
        : NumberInputBox(maxCh, x, y, tc, fc, olc) {
    }

    Vector2 getMousePos() const override;
};

class DelayNumberInputBoxInCamera : public DelayNumberInputBox {
public:
    DelayNumberInputBoxInCamera(int maxCh, float x = 0, float y = 0,
        Color tc = MAROON, Color fc = LIGHTGRAY, Color olc = DARKGRAY)
        : DelayNumberInputBox(maxCh, x, y, tc, fc, olc) {
    }

    Vector2 getMousePos() const override;
};

// RectButton with text
class TextBox : public RectButton {
public:
    string Text;
    TextBox(string t, float x = 0, float y = 0,
        Color tc = WHITE, Color fc = BLUE, Color olc = DARKGRAY)
        : RectButton(x, y, 0, 0, tc, fc, olc), Text(t){
        Vector2 tsize = MeasureTextEx(UI::font, t.c_str(), UI::fontSize, UI::spacing);
        rect.width = tsize.x + padding;
        rect.height = tsize.y + padding;
    }
    void draw() override;
};
class TextBoxInCamera : public RectButton {
public:
    string Text;
    TextBoxInCamera(string t, float x = 0, float y = 0,
        Color tc = WHITE, Color fc = BLUE, Color olc = DARKGRAY)
        : RectButton(x, y, 0, 0, tc, fc, olc), Text(t){
        Vector2 tsize = MeasureTextEx(UI::font, t.c_str(), UI::fontSize, UI::spacing);
        rect.width = tsize.x + padding;
        rect.height = tsize.y + padding;
    }
    Vector2 getMousePos() const override;
    void draw() override;
};

// RectButton but for displaying code blocks
class CodeBlock : public TextBox {
private:
    static constexpr Color CodeColor = { 232,232,232,180 };
    bool isHighlight;
public:
    CodeBlock(string t, float x = 0, float y = 0,
        Color tc = DARKGRAY, Color fc = CodeColor, Color olc = CodeColor)
        : TextBox(t,x,y, tc, fc, olc), isHighlight(false) {
        Vector2 tsize = MeasureTextEx(UI::font, t.c_str(), UI::fontSize, UI::spacing);
        rect.width = tsize.x + padding;
        rect.height = tsize.y + padding;
    }
    void highlight() override;
    void unhighlight() override;
    void hover() override;
};

// ### CircleButton Class
// Uses Vector2 center and Radius in Raylib to create a button
class CircleButton : public Button {
protected:
    Vector2 center;
    float radius;
public:

    // default color
    CircleButton(Vector2 cent, float r,
        Color tc, Color fc, Color rc);
    virtual ~CircleButton(){
        if (animation)delete animation;
    }
    virtual void setRadius(int r) {
        radius = r;
    }
    virtual float getRadius() const{
        return radius;
    }
    virtual Vector2 getCenter() const {
        return center;
    }
    virtual void setCenter(float x, float y) {
        center.x = x;
        center.y = y;
    }
    virtual void setCenterX(float x) {
        center.x = x;
    }
    virtual float getCenterX() const {
        return center.x;
    }

    virtual void setCenterY(float y) {
        center.y = y;
    }
    virtual float getCenterY() const {
        return center.y;
    }
    bool checkCollision() override;
    virtual void update();
    virtual void draw() = 0;
    void hover() override;
    void unhover() override;
    void click() override;
    void unclick() override;
};

// CircleButton with text
class TextCircle : public CircleButton {
public:
    string Text;
    TextCircle(string t, Vector2 cent = { 0, 0 }, float r = 50.0f,
        Color tc = RAYWHITE, Color fc = ORANGE, Color rc = RED)
        : CircleButton(cent, r, tc, fc, rc), Text(t) {
    }
    void draw() override;
    void hover() override;
    void unhover() override;
};

// CircleButton with texture
class TextureCircle : public CircleButton {
protected:
    Texture2D Texture;
public:
    TextureCircle(Texture2D t, Vector2 cent = { 0, 0 }, float r = 50.0f,
        Color tc = RAYWHITE, Color fc = ORANGE, Color rc = RED)
        : CircleButton(cent, r, tc, fc, rc), Texture(t) {
    }
    void draw() override;
    void hover() override;
    void unhover() override;
    void changeTexture(Texture2D texture);
};

// CircleButton with texture
class Program;
class PlayButton : public TextureCircle {
private:
    Program* scenehandler;
public:
    PlayButton(Program* obj, Texture2D t, Vector2 cent = { 0, 0 }, float r = 50.0f,
        Color tc = RAYWHITE, Color fc = ORANGE, Color rc = RED)
        : TextureCircle(t,cent, r, tc, fc, rc), scenehandler(obj){
    }
    void update() override;
};

// Uses CircleButton and Input Handler to create a button with text
class InputCircle : public CircleButton {
public:
    std::unique_ptr<InputHandler> inputHandler;

    InputCircle(int maxCh, Vector2 cent = { 0, 0 }, float r = 50.0f,
        Color tc = BLUE, Color fc = RAYWHITE, Color rc = BLUE)
        : CircleButton(cent, r, tc, fc, rc),
        inputHandler(std::make_unique<InputHandler>(maxCh)) {
    }

    void update() override;

    void draw() override;

    virtual void clear() {
        inputHandler->clear();
    }
    void setCursor() override {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
};

// Uses CircleButton and Number Input Handler to create a button with text
class NumberInputCircle : public InputCircle {
public:

    // Constructor with minimal parameters
    NumberInputCircle(int maxCh)
        : InputCircle(maxCh) {
        inputHandler = std::make_unique<NumericInputHandler>(maxCh);
    }
    NumberInputCircle(int maxCh, Color tc, Color fc, Color rc)
        : InputCircle(maxCh, { 0, 0 }, 50.0f,  tc, fc, rc) {
        inputHandler = std::make_unique<NumericInputHandler>(maxCh);
    }

    // Constructor with position and radius, no colors
    NumberInputCircle(Vector2 cent, float r, int input, int maxCh)
        : InputCircle(maxCh, cent, r, BLUE, RAYWHITE, BLUE) {
        inputHandler = std::make_unique<NumericInputHandler>(maxCh);
        setNumber(input);
    }

    // Full constructor with all parameters
    NumberInputCircle(Vector2 cent, float r, int input, int maxCh, Color tc, Color fc, Color rc)
        : InputCircle(maxCh, cent, r, tc, fc, rc) {
        inputHandler = std::make_unique<NumericInputHandler>(maxCh);
        setNumber(input);
    }
    virtual void setNumber(int x) {
        inputHandler->setText(std::to_string(x));
    }
    virtual int getNumber() const { return dynamic_cast<NumericInputHandler*>(inputHandler.get())->getNumber(); }
    void clear() override {
        inputHandler->clear();
        
    }
};

// NumberInputCircleInCamera is NumberInputCircle but uses the camera position to get the mouse position
class NumberInputCircleInCamera : public NumberInputCircle {
private:
    void Indicate(string text);
public:
    string indicateNode; 
    // default Number Input Box
    NumberInputCircleInCamera(int maxCh)
        : NumberInputCircle(maxCh), indicateNode("") {
    }
    NumberInputCircleInCamera(int maxCh, Color tc, Color fc, Color rc)
        : NumberInputCircle(maxCh, tc, fc, rc), indicateNode("") {
    }
    NumberInputCircleInCamera(Vector2 cent, float r, int input, int maxCh)
        : NumberInputCircle(cent, r, input, maxCh), indicateNode("") {
    }
    NumberInputCircleInCamera(Vector2 cent, float r, int input, int maxCh, Color tc, Color fc, Color rc)
        : NumberInputCircle(cent, r, input, maxCh, tc, fc, rc), indicateNode("") {
    }
    Vector2 getMousePos() const override;
    void draw() override;
};



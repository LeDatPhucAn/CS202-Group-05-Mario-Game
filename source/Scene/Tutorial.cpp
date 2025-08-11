#include "Tutorial.hpp"
#include "raylib.h"
#include <cmath>
#include <vector>
#include <string>
#include "SoundController.hpp"

Tutorial::Tutorial(SceneManager* _manager) : Scene(_manager)
{
    // Load textures (reuse from other scenes)
    buttonTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButton.png");
    buttonHoverTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButtonHovered.png");
    tutorialBoard = LoadTexture("assets/Backgrounds/SettingsBoard.png");
    
    if (buttonTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load tutorial button texture");
    }
    if (buttonHoverTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load tutorial button hover texture");
    }
    if (tutorialBoard.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load tutorial board texture");
    }
    
    
    initializeActionList();
}

Tutorial::~Tutorial()
{
    if (buttonTexture.id > 0)
    {
        UnloadTexture(buttonTexture);
    }
    if (buttonHoverTexture.id > 0)
    {
        UnloadTexture(buttonHoverTexture);
    }
    if (tutorialBoard.id > 0)
    {
        UnloadTexture(tutorialBoard);
    }
    
    for (auto button : buttons)
    {
        delete button;
    }
    buttons.clear();
    
}

void Tutorial::initializeActionList()
{
    actionList = {
        {Action::BUTTON_LEFT, "Move Left"},
        {Action::BUTTON_RIGHT, "Move Right"},
        {Action::BUTTON_UP, "Jump/Look Up"},
        {Action::BUTTON_DOWN, "Crouch/Down"},
        {Action::BUTTON_RUN, "Run"},
        {Action::BUTTON_THROW_FIREBALL, "Throw Fireball"}
    };
}

void Tutorial::updateScene()
{
    // Update swap message timer
    if (showSwapMessage)
    {
        swapMessageTimer -= GetFrameTime();
        if (swapMessageTimer <= 0.0f)
        {
            showSwapMessage = false;
        }
    }
    
    // Handle key binding editing
    if (isEditing)
    {
        // Use GetKeyPressed() to get the key that was pressed
        int newKey = GetKeyPressed();

        if (newKey != 0 && newKey != KEY_BACKSPACE)
        {
            updateKeyBinding(editingAction, selectedColumn, newKey);
            isEditing = false;
            editingAction = -1;
        }
        else if (IsKeyPressed(KEY_BACKSPACE))
        {
            isEditing = false;
            editingAction = -1;
        }
        return;
    }
    
    // Navigation
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        if (selectedButton == 0) // In table
        {
            selectedRow = (selectedRow - 1 + actionList.size()) % actionList.size();
        }
        else
        {
            selectedButton = 0;
        }
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        if (selectedButton == 0) // In table
        {
            selectedRow = (selectedRow + 1) % actionList.size();
        }
        else
        {
            selectedButton = 0;
        }
    }
    
    // Column navigation (Mario vs Luigi)
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
    {
        if (selectedButton == 0) // In table
        {
            selectedColumn = 0; // Mario
        }
        else
        {
            selectedButton = (selectedButton - 1 + 2) % 2;
        }
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
    {
        if (selectedButton == 0) // In table
        {
            selectedColumn = 1; // Luigi
        }
        else
        {
            selectedButton = (selectedButton + 1) % 2;
        }
    }
    
    // Switch between table and buttons
    if (IsKeyPressed(KEY_TAB))
    {
        selectedButton = (selectedButton == 0) ? 1 : 0;
    }
    
    // Edit key binding
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        if (selectedButton == 0) // In table
        {
            isEditing = true;
            editingAction = selectedRow;
        }
        else if (selectedButton == 1) // Back button
        {
            manager->goBack();
        }
    }
    
    // Mouse interaction
    Vector2 mousePos = GetMousePosition();
    float boardWidth = 800;
    float boardHeight = 650;
    float boardX = UI::screenWidth / 2 - boardWidth / 2;
    float boardY = UI::screenHeight / 2 - 325;
    
    // Check table clicks
    float tableStartY = boardY + 120;
    float rowHeight = 60;
    float col1X = boardX + 50;   // Action
    float col2X = boardX + 250;  // Mario Key
    float col3X = boardX + 500;  // Luigi Key
    
    for (int i = 0; i < actionList.size(); i++)
    {
        float rowY = tableStartY + 40 + (i * rowHeight);
        
        // Mario key column
        Rectangle marioKeyRect = {col2X, rowY, 200, 40};
        if (CheckCollisionPointRec(mousePos, marioKeyRect))
        {
            selectedButton = 0;
            selectedRow = i;
            selectedColumn = 0;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                isEditing = true;
                editingAction = i;
            }
        }
        
        // Luigi key column
        Rectangle luigiKeyRect = {col3X, rowY, 200, 40};
        if (CheckCollisionPointRec(mousePos, luigiKeyRect))
        {
            selectedButton = 0;
            selectedRow = i;
            selectedColumn = 1;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                isEditing = true;
                editingAction = i;
            }
        }
    }
    
    // Back button
    float buttonWidth = 200;
    float buttonHeight = 50;
    Rectangle backButton = {boardX + boardWidth - buttonWidth - 30, boardY + boardHeight - 70, buttonWidth, buttonHeight};
    if (CheckCollisionPointRec(mousePos, backButton))
    {
        selectedButton = 1;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {

            manager->goBack();
        }
    }

    // BACKSPACE to go back
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        manager->goBack();
    }
}

void Tutorial::displayScene()
{
    // Draw semi-transparent background
    DrawRectangle(0, 0, UI::screenWidth, UI::screenHeight, Color{0, 0, 0, 80});
    
    // Calculate tutorial board dimensions and position
    float boardWidth = 800;
    float boardHeight = 650;
    float boardX = UI::screenWidth / 2 - boardWidth / 2;
    float boardY = UI::screenHeight / 2 - 325;
    Rectangle boardRect = {boardX, boardY, boardWidth, boardHeight};
    
    // Draw the tutorial board
    if (tutorialBoard.id > 0)
    {
        DrawTexturePro(tutorialBoard, 
                      {0, 0, (float)tutorialBoard.width, (float)tutorialBoard.height},
                      boardRect, {0, 0}, 0.0f, WHITE);
    }
    else
    {
        DrawRectangleRounded(boardRect, 0.1f, 8, Color{40, 40, 40, 240});
        DrawRectangleRoundedLines(boardRect, 0.1f, 8, ORANGE);
    }
    
    // Draw title
    std::string titleText = "CONTROLS";
    float titleSpacing = 5.0f;
    float titleFontSize = 36.0f;
    Vector2 titleSize = MeasureTextEx(UI::boldFont, titleText.c_str(), titleFontSize, titleSpacing);
    float titleX = (UI::screenWidth - titleSize.x) / 2.0f;
    float titleY = boardY + 20;
    
    DrawTextEx(UI::boldFont, titleText.c_str(),
               {titleX + 2, titleY + 2}, titleFontSize, titleSpacing, Color{255, 200, 0, 255});
    DrawTextEx(UI::boldFont, titleText.c_str(),
               {titleX, titleY}, titleFontSize, titleSpacing, Color{255, 255, 255, 255});
    
    // Draw subtitle
    std::string subtitleText = "Mario & Luigi Key Bindings (Click to edit)";
    float subtitleFontSize = 18.0f;
    Vector2 subtitleSize = MeasureTextEx(UI::font, subtitleText.c_str(), subtitleFontSize, 2);
    float subtitleX = (UI::screenWidth - subtitleSize.x) / 2.0f;
    float subtitleY = boardY + 70;
    
    DrawTextEx(UI::font, subtitleText.c_str(),
               {subtitleX + 1, subtitleY + 1}, subtitleFontSize, 2, DARKGRAY);
    DrawTextEx(UI::font, subtitleText.c_str(),
               {subtitleX, subtitleY}, subtitleFontSize, 2, LIGHTGRAY);
    
    // Draw key bindings table
    drawKeyBindingTable();
    
    // Draw Back button
    float buttonWidth = 200;
    float buttonHeight = 50;
    Rectangle backButtonRect = {boardX + boardWidth - buttonWidth - 30, boardY + boardHeight - 70, buttonWidth, buttonHeight};
    Color backButtonColor = (selectedButton == 1) ? ORANGE : Color{100, 100, 100, 255};
    DrawRectangleRounded(backButtonRect, 0.2f, 8, backButtonColor);
    DrawRectangleRoundedLines(backButtonRect, 0.2f, 8, BLACK);
    
    std::string backText = "BACK";
    Vector2 backTextSize = MeasureTextEx(UI::font, backText.c_str(), 20, 2);
    float backTextX = backButtonRect.x + (backButtonRect.width - backTextSize.x) / 2;
    float backTextY = backButtonRect.y + (backButtonRect.height - backTextSize.y) / 2;
    DrawTextEx(UI::font, backText.c_str(), {backTextX, backTextY}, 20, 2, WHITE);
    
    // Draw instructions
    drawInstructions();
    
    // Draw editing prompt if in edit mode
    if (isEditing)
    {
        std::string editText = "Press any key to bind (BACKSPACE to cancel)";
        float editFontSize = 24.0f;
        Vector2 editTextSize = MeasureTextEx(UI::boldFont, editText.c_str(), editFontSize, 3);
        float editX = (UI::screenWidth - editTextSize.x) / 2.0f;
        float editY = UI::screenHeight - 100;
        
        // Background
        Rectangle editBg = {editX - 20, editY - 10, editTextSize.x + 40, editTextSize.y + 20};
        DrawRectangleRounded(editBg, 0.3f, 8, Color{255, 0, 0, 200});
        DrawRectangleRoundedLines(editBg, 0.3f, 8, WHITE);
        
        DrawTextEx(UI::boldFont, editText.c_str(),
                   {editX + 2, editY + 2}, editFontSize, 3, BLACK);
        DrawTextEx(UI::boldFont, editText.c_str(),
                   {editX, editY}, editFontSize, 3, WHITE);
    }
    
    // Draw key swap notification if active
    if (showSwapMessage)
    {
        float swapFontSize = 20.0f;
        Vector2 swapTextSize = MeasureTextEx(UI::font, swapMessage.c_str(), swapFontSize, 2);
        float swapX = (UI::screenWidth - swapTextSize.x) / 2.0f;
        float swapY = UI::screenHeight - 140;
        
        // Calculate alpha based on remaining time
        float alpha = (swapMessageTimer > 1.0f) ? 255 : (swapMessageTimer * 255);
        
        // Background
        Rectangle swapBg = {swapX - 15, swapY - 8, swapTextSize.x + 30, swapTextSize.y + 16};
        DrawRectangleRounded(swapBg, 0.3f, 8, Color{0, 200, 0, (unsigned char)(alpha * 0.8f)});
        DrawRectangleRoundedLines(swapBg, 0.3f, 8, Color{255, 255, 255, (unsigned char)alpha});
        
        DrawTextEx(UI::font, swapMessage.c_str(),
                   {swapX + 1, swapY + 1}, swapFontSize, 2, Color{0, 0, 0, (unsigned char)alpha});
        DrawTextEx(UI::font, swapMessage.c_str(),
                   {swapX, swapY}, swapFontSize, 2, Color{255, 255, 255, (unsigned char)alpha});
    }
}


void Tutorial::drawKeyBindingTable()
{
    float boardWidth = 800;
    float boardX = UI::screenWidth / 2 - boardWidth / 2;
    float boardY = UI::screenHeight / 2 - 325;
    
    float tableStartY = boardY + 120;
    float rowHeight = 60;
    float col1X = boardX + 50;   // Action
    float col2X = boardX + 250;  // Mario Key
    float col3X = boardX + 500;  // Luigi Key
    
    // Draw table headers
    DrawTextEx(UI::boldFont, "ACTION", {col1X, tableStartY}, 22, 2, YELLOW);
    DrawTextEx(UI::boldFont, "MARIO", {col2X + 60, tableStartY}, 22, 2, Color{255, 100, 100, 255});
    DrawTextEx(UI::boldFont, "LUIGI", {col3X + 60, tableStartY}, 22, 2, Color{100, 255, 100, 255});
    
    // Draw separator line
    DrawLineEx({boardX + 20, tableStartY + 30}, {boardX + boardWidth - 20, tableStartY + 30}, 3, YELLOW);
    
    // Draw key bindings
    for (int i = 0; i < actionList.size(); i++)
    {
        float rowY = tableStartY + 40 + (i * rowHeight);
        
        // Highlight selected row
        if (selectedButton == 0 && selectedRow == i)
        {
            Rectangle rowBg = {boardX + 20, rowY - 8, boardWidth - 40, rowHeight - 5};
            DrawRectangleRounded(rowBg, 0.1f, 4, Color{255, 255, 255, 30});
            DrawRectangleRoundedLines(rowBg, 0.1f, 4, ORANGE);
        }
        else if (i % 2 == 0)
        {
            Rectangle rowBg = {boardX + 20, rowY - 8, boardWidth - 40, rowHeight - 5};
            DrawRectangleRounded(rowBg, 0.1f, 4, Color{255, 255, 255, 10});
        }
        
        // Draw action name
        DrawTextEx(UI::font, actionList[i].second.c_str(), {col1X, rowY + 10}, 18, 1, BLACK);

        // Draw Mario key
        int marioKeyCode = getCurrentKeyForAction(actionList[i].first, 0);
        std::string marioKeyText = getKeyName(marioKeyCode);
        Rectangle marioKeyBox = {col2X, rowY, 200, 40};
        Color marioBoxColor = (selectedButton == 0 && selectedRow == i && selectedColumn == 0) ? 
                              Color{255, 100, 100, 150} : Color{80, 80, 80, 200};
        Color marioBorderColor = (selectedButton == 0 && selectedRow == i && selectedColumn == 0) ? 
                                 Color{255, 100, 100, 255} : Color{150, 150, 150, 255};
        
        DrawRectangleRounded(marioKeyBox, 0.3f, 4, marioBoxColor);
        DrawRectangleRoundedLines(marioKeyBox, 0.3f, 4, marioBorderColor);
        
        Vector2 marioTextSize = MeasureTextEx(UI::font, marioKeyText.c_str(), 16, 1);
        float marioTextX = marioKeyBox.x + (marioKeyBox.width - marioTextSize.x) / 2;
        float marioTextY = marioKeyBox.y + (marioKeyBox.height - marioTextSize.y) / 2;
        DrawTextEx(UI::font, marioKeyText.c_str(), {marioTextX, marioTextY}, 16, 1, Color{255, 200, 200, 255});
        
        // Draw Luigi key
        int luigiKeyCode = getCurrentKeyForAction(actionList[i].first, 1);
        std::string luigiKeyText = getKeyName(luigiKeyCode);
        Rectangle luigiKeyBox = {col3X, rowY, 200, 40};
        Color luigiBoxColor = (selectedButton == 0 && selectedRow == i && selectedColumn == 1) ? 
                              Color{100, 255, 100, 150} : Color{80, 80, 80, 200};
        Color luigiBorderColor = (selectedButton == 0 && selectedRow == i && selectedColumn == 1) ? 
                                 Color{100, 255, 100, 255} : Color{150, 150, 150, 255};
        
        DrawRectangleRounded(luigiKeyBox, 0.3f, 4, luigiBoxColor);
        DrawRectangleRoundedLines(luigiKeyBox, 0.3f, 4, luigiBorderColor);
        
        Vector2 luigiTextSize = MeasureTextEx(UI::font, luigiKeyText.c_str(), 16, 1);
        float luigiTextX = luigiKeyBox.x + (luigiKeyBox.width - luigiTextSize.x) / 2;
        float luigiTextY = luigiKeyBox.y + (luigiKeyBox.height - luigiTextSize.y) / 2;
        DrawTextEx(UI::font, luigiKeyText.c_str(), {luigiTextX, luigiTextY}, 16, 1, Color{200, 255, 200, 255});
    }
}

void Tutorial::drawInstructions()
{
    std::string instructions;
    if (isEditing)
    {
        instructions = "Press the key you want to assign (backspace to cancel)";
    }
    else
    {
        instructions = "ARROWS: Navigate . ENTER: Edit key . TAB: Finish . BACKSPACE: Cancel";
    }
    
    float instrX = UI::screenWidth / 2 - 350;
    float instrY = UI::screenHeight - 50;
    
    DrawTextEx(UI::font, instructions.c_str(),
               {instrX + 1, instrY + 1}, 16, 1, DARKGRAY);
    DrawTextEx(UI::font, instructions.c_str(),
               {instrX, instrY}, 16, 1, WHITE);
}

void Tutorial::updateKeyBinding(int actionIndex, int player, int newKey)
{
    if (actionIndex < 0 || actionIndex >= actionList.size()) return;
    
    Action action = actionList[actionIndex].first;
    std::string playerName = (player == 0) ? "Mario" : "Luigi";
    
    // Check if this key is already bound to another action for this player
    Action conflictingAction = static_cast<Action>(-1);
    std::string conflictingActionName = "";
    
    // Find all actions for this player to check for conflicts
    for (const auto& actionPair : actionList)
    {
        if (KeyBindingManager::getKeyBinding(playerName, actionPair.first) == newKey && actionPair.first != action)
        {
            conflictingAction = actionPair.first;
            conflictingActionName = actionPair.second;
            break;
        }
    }
    
    if (player == 0) // Mario
    {
        KeyBindingManager::setKeyBinding("Mario", action, newKey);
    }
    else // Luigi
    {
        KeyBindingManager::setKeyBinding("Luigi", action, newKey);
    }
    
    // Show swap message if there was a conflict
    if (conflictingAction != static_cast<Action>(-1))
    {
        std::string currentActionName = actionList[actionIndex].second;
        swapMessage = "Swapped keys: " + currentActionName + " <-> " + conflictingActionName;
        showSwapMessage = true;
        swapMessageTimer = 3.0f; // Show for 3 seconds
    }
}


int Tutorial::getCurrentKeyForAction(Action action, int player)
{
    if (player == 0) // Mario
    {
        return KeyBindingManager::getKeyBinding("Mario", action);
    }
    else // Luigi
    {
        return KeyBindingManager::getKeyBinding("Luigi", action);
    }
}

std::string Tutorial::getKeyName(int keyCode)
{
    switch (keyCode)
    {
        case KEY_A: return "A";
        case KEY_B: return "B";
        case KEY_C: return "C";
        case KEY_D: return "D";
        case KEY_E: return "E";
        case KEY_F: return "F";
        case KEY_G: return "G";
        case KEY_H: return "H";
        case KEY_I: return "I";
        case KEY_J: return "J";
        case KEY_K: return "K";
        case KEY_L: return "L";
        case KEY_M: return "M";
        case KEY_N: return "N";
        case KEY_O: return "O";
        case KEY_P: return "P";
        case KEY_Q: return "Q";
        case KEY_R: return "R";
        case KEY_S: return "S";
        case KEY_T: return "T";
        case KEY_U: return "U";
        case KEY_V: return "V";
        case KEY_W: return "W";
        case KEY_X: return "X";
        case KEY_Y: return "Y";
        case KEY_Z: return "Z";
        case KEY_SPACE: return "Space";
        case KEY_ENTER: return "Enter";
        case KEY_TAB: return "Tab";
        case KEY_BACKSPACE: return "Backspace";
        case KEY_LEFT_SHIFT: return "Left Shift";
        case KEY_RIGHT_SHIFT: return "Right Shift";
        case KEY_LEFT_CONTROL: return "Left Ctrl";
        case KEY_RIGHT_CONTROL: return "Right Ctrl";
        case KEY_LEFT_ALT: return "Left Alt";
        case KEY_RIGHT_ALT: return "Right Alt";
        case KEY_LEFT: return "Left Arrow";
        case KEY_RIGHT: return "Right Arrow";
        case KEY_UP: return "Up Arrow";
        case KEY_DOWN: return "Down Arrow";
        case KEY_SLASH: return "/ (Slash)";
        case KEY_PERIOD: return ". (Period)";
        case KEY_COMMA: return ", (Comma)";
        case KEY_SEMICOLON: return "; (Semicolon)";
        case KEY_APOSTROPHE: return "' (Apostrophe)";
        case KEY_LEFT_BRACKET: return "[ (Left Bracket)";
        case KEY_RIGHT_BRACKET: return "] (Right Bracket)";
        case KEY_BACKSLASH: return "\\ (Backslash)";
        case KEY_MINUS: return "- (Minus)";
        case KEY_EQUAL: return "= (Equal)";
        case KEY_GRAVE: return "` (Grave)";
        case KEY_ESCAPE: return "Escape";
        case KEY_F1: return "F1";
        case KEY_F2: return "F2";
        case KEY_F3: return "F3";
        case KEY_F4: return "F4";
        case KEY_F5: return "F5";
        case KEY_F6: return "F6";
        case KEY_F7: return "F7";
        case KEY_F8: return "F8";
        case KEY_F9: return "F9";
        case KEY_F10: return "F10";
        case KEY_F11: return "F11";
        case KEY_F12: return "F12";
        case KEY_ZERO: return "0";
        case KEY_ONE: return "1";
        case KEY_TWO: return "2";
        case KEY_THREE: return "3";
        case KEY_FOUR: return "4";
        case KEY_FIVE: return "5";
        case KEY_SIX: return "6";
        case KEY_SEVEN: return "7";
        case KEY_EIGHT: return "8";
        case KEY_NINE: return "9";
        default: return "Unknown (" + std::to_string(keyCode) + ")";
    }
}


#include "PreGame.hpp"
#include "UI.hpp"
#include "GameInfo.hpp"
#include "Structs.hpp"
#include <raylib.h>
#include <string>

PreGame::PreGame(SceneManager* m) : Scene(m) {}

void PreGame::updateScene() {
    // Determine max selection rows depending on mode
    // SINGLE mode rows: 0 MODE, 1 CHARACTER, 2 DIFFICULTY, 3 START, 4 BACK
    // DUAL mode rows:   0 MODE, 1 DIFFICULTY, 2 START, 3 BACK
    int maxIndex = (mode==Mode::SINGLE)?4:3;

    if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) selectionIndex = (selectionIndex+1) % (maxIndex+1);
    if(IsKeyPressed(KEY_UP)   || IsKeyPressed(KEY_W)) selectionIndex = (selectionIndex-1 + (maxIndex+1)) % (maxIndex+1);

    auto toggleDifficulty = [&](){ 
        if(difficulty == Difficulty::EASY) difficulty = Difficulty::HARD;
        else if(difficulty == Difficulty::HARD) difficulty = Difficulty::HARDCORE;
        else difficulty = Difficulty::EASY;
    };
    auto toggleMode = [&](){ 
        mode = (mode==Mode::SINGLE)?Mode::DUAL:Mode::SINGLE; 
        if(mode==Mode::DUAL && selectionIndex==1) { 
            chooseMario = !chooseMario;
        } 
    };    
   

    if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        if(selectionIndex==0) toggleMode();
        else if(mode==Mode::SINGLE && selectionIndex==1) chooseMario = !chooseMario; // character
        else if( (mode==Mode::SINGLE && selectionIndex==2) || (mode==Mode::DUAL && selectionIndex==1) ) toggleDifficulty();
    }
    if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        if(selectionIndex==0) toggleMode();
        else if(mode==Mode::SINGLE && selectionIndex==1) chooseMario = !chooseMario;
        else if( (mode==Mode::SINGLE && selectionIndex==2) || (mode==Mode::DUAL && selectionIndex==1) ) toggleDifficulty();
    }

    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        int startRow = (mode==Mode::SINGLE)?3:2;
        int backRow  = (mode==Mode::SINGLE)?4:3;
        if(selectionIndex==startRow) {
            // Store settings in GameInfo singleton
            GameInfo* gameInfo = GameInfo::getInstance();
            gameInfo->setMode(mode==Mode::SINGLE ? Mode::SINGLE : Mode::DUAL);
            gameInfo->setCharacter(chooseMario);
            gameInfo->setDifficulty(difficulty);
            
            manager->changeScene(sceneType::GAME);
            return;
        }
        if(selectionIndex==backRow) {
            manager->goBack();
            return;
        }
        if(selectionIndex==0) toggleMode();
        else if(mode==Mode::SINGLE && selectionIndex==1) chooseMario = !chooseMario;
        else if( (mode==Mode::SINGLE && selectionIndex==2) || (mode==Mode::DUAL && selectionIndex==1) ) toggleDifficulty();
    }
    if(IsKeyPressed(KEY_ESCAPE)) manager->goBack();
}

void PreGame::displayScene() {
	ClearBackground(Color{25,25,40,255});

    Rectangle gradientRect = {0, 0, (float)UI::screenWidth, (float)UI::screenHeight};
    DrawRectangleGradientV(0, 0, UI::screenWidth, UI::screenHeight, 
                           Color{25,25,40,255}, Color{15,15,30,255});
                
	std::string title = "GAME SETUP";
	Vector2 tSize = MeasureTextEx(UI::boldFont, title.c_str(), 46, 4);
	DrawTextEx(UI::boldFont, title.c_str(), {(float)(UI::screenWidth - tSize.x)/2, 70}, 46, 4, WHITE);

	int baseY = 170;
	int rowH = 55;
	int idx=0;
	auto drawRow=[&](const std::string& label,const std::string& value,int row){
		std::string line = label + (value.empty()?"":" : ") + value;
		Color col = (selectionIndex==row)?YELLOW:WHITE;
		Vector2 sz = MeasureTextEx(UI::font, line.c_str(), 26, 2);
		DrawTextEx(UI::font, line.c_str(), {(float)UI::screenWidth/2 - sz.x/2, (float)baseY + row*rowH}, 26, 2, col);
	};

	drawRow("MODE", (mode==Mode::SINGLE?"SINGLE":"DUAL"), idx++);
	if(mode==Mode::SINGLE) drawRow("CHARACTER", chooseMario?"MARIO":"LUIGI", idx++);

	std::string difficultyText;
    if(difficulty==Difficulty::EASY) difficultyText = "EASY (5 Lives)";
    else if(difficulty==Difficulty::HARD) difficultyText = "HARD (3 Lives)";
    else difficultyText = "HARDCORE (1 Life)";
	drawRow("DIFFICULTY", difficultyText, idx++);

	drawRow("START", "", idx++);
	drawRow("BACK", "", idx++);

	std::string hint = "UP/DOWN select, LEFT/RIGHT toggle, ENTER confirm, ESC back";
	Vector2 hSz = MeasureTextEx(UI::font, hint.c_str(), 18, 2);
	DrawTextEx(UI::font, hint.c_str(), {(float)(UI::screenWidth - hSz.x)/2, (float)UI::screenHeight - 70}, 18, 2, GRAY);
}

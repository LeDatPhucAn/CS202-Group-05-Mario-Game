#include "SoundController.hpp"

SoundController::SoundController()
    : marioStateSFX(generalPath + "Motion/"),
      blockStateSFX(generalPath + "Block/"),
      sceneMusic(generalPath + "Scene/")
{
    loadBlockStateSFX();
    loadMarioStateSFX();
    loadSceneMusic();
}

SoundController &SoundController::getInstance()
{
    static SoundController instance;
    return instance;
}

void SoundController::loadMarioStateSFX()
{
    unordered_map<marioStateType, string> sfxFileMap = {
        {marioStateType::JUMP, "jump.wav"},
        {marioStateType::CROUCH, "crouch.wav"},
        {marioStateType::GROW, "change_big.wav"},
        {marioStateType::UNGROW, "change_small.wav"},
        {marioStateType::DEAD, "death.wav"},
        {marioStateType::THROWFB, "throw_fireball.wav"}};

    marioStateSFX.load(sfxFileMap);
}

void SoundController::loadBlockStateSFX()
{
    unordered_map<blockStateType, string> sfxFileMap = {
        {blockStateType::BOUNCE, "bounce_block.wav"},
        {blockStateType::BREAK, "break_block.wav"},
        {blockStateType::SPAWNITEM, "spawn_item.wav"}};

    blockStateSFX.load(sfxFileMap);
}
void SoundController::loadSceneMusic()
{
    unordered_map<sceneType, pair<string, bool>> sceneMusicMap = {
        {sceneType::MENU, {"Menu.mp3", true}},
        {sceneType::GAME, {"level_theme_1.mp3", true}},
        {sceneType::PAUSE, {"Pause.wav", false}}
        // Add more sceneType mappings as needed
    };

    sceneMusic.load(sceneMusicMap);
}
void SoundController::playMarioStateSFX(marioStateType type)
{
    marioStateSFX.play(type);
}

void SoundController::playBlockStateSFX(blockStateType type)
{
    blockStateSFX.play(type);
}

void SoundController::clearAll()
{

    marioStateSFX.clear();
    blockStateSFX.clear();
    sceneMusic.clear();
}

void SoundController::playSceneMusic(sceneType type)
{
    sceneMusic.play(type);
}

void SoundController::updateSceneMusic()
{
    sceneMusic.update();
}
void SoundController::pauseSceneMusic()
{
    sceneMusic.pause();
}

void SoundController::resumeSceneMusic()
{
    sceneMusic.resume();
}

void SoundController::stopSceneMusic()
{
    sceneMusic.stop();
}
#include "SoundController.hpp"

SoundController::SoundController()
    : marioStateSFX(generalPath + "Motion/"),
      blockStateSFX(generalPath + "Block/"),
      sceneMusic(generalPath + "Scene/"),
      marioVoiceSFX(generalPath + "Mario/"),
      sceneSFX(generalPath + "Scene/")
{
    loadBlockStateSFX();
    loadMarioStateSFX();
    loadSceneSFX();
    loadSceneMusic();
    loadMarioVoice();
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
        {marioStateType::SMALLJUMP, "jumpsmall.wav"},
        {marioStateType::CROUCH, "crouch.wav"},
        {marioStateType::GROW, "change_big.wav"},
        {marioStateType::UNGROW, "change_small.wav"},
        {marioStateType::DEAD, "death.wav"},
        {marioStateType::THROWFB, "throw_fireball.wav"},
        {marioStateType::SQUISH_ENEMY, "stomp.wav"},
        {marioStateType::KICK_SHELL, "kick.wav"}};

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
void SoundController::loadMarioVoice()
{
    unordered_map<marioVoice, string> marioVoiceMap = {
        {marioVoice::LETSGO, "here_we_go.wav"},
        {marioVoice::JUMP, "hoo.wav"},
        {marioVoice::HURT, "owowowow.wav"}
        // Add more sceneType mappings as needed
    };

    marioVoiceSFX.load(marioVoiceMap);
}
void SoundController::loadSceneMusic()
{
    unordered_map<sceneType, pair<string, bool>> sceneMusicMap = {
        {sceneType::MENU, {"Menu.mp3", true}},
        {sceneType::GAME, {"level_1(3).mp3", true}},
        {sceneType::GAMEOVER, {"GameOver.mp3", true}}
        // Add more sceneType mappings as needed
    };

    sceneMusic.load(sceneMusicMap);
}
void SoundController::loadSceneSFX()
{
    unordered_map<sceneType, string> sceneSFXMap = {
        {sceneType::PAUSE, "Pause.wav"}};

    sceneSFX.load(sceneSFXMap);
}
void SoundController::playMarioStateSFX(marioStateType type)
{
    marioStateSFX.play(type);
}

void SoundController::playBlockStateSFX(blockStateType type)
{
    blockStateSFX.play(type);
}
void SoundController::playMarioVoiceSFX(marioVoice type)
{
    marioVoiceSFX.play(type);
}
void SoundController::playSceneSFX(sceneType type)
{
    sceneSFX.play(type);
}
void SoundController::clearAll()
{
    marioVoiceSFX.clear();
    marioStateSFX.clear();
    blockStateSFX.clear();
    sceneMusic.clear();
    sceneSFX.clear();
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
void SoundController::setMasterSFXVolume(float vol)
{
    marioStateSFX.setVolume(vol);
    blockStateSFX.setVolume(vol);
    marioVoiceSFX.setVolume(vol);
    sceneSFX.setVolume(vol);
}

void SoundController::setMasterMusicVolume(float vol)
{
    sceneMusic.setVolume(vol);
}
#include "SoundController.hpp"

SoundController::SoundController()
    : playerStateSFX(generalPath + "Motion/"),
      blockStateSFX(generalPath + "Block/"),
      sceneMusic(generalPath + "Scene/"),
      marioVoiceSFX(generalPath + "Player/"),
      sceneSFX(generalPath + "Scene/")
{
}
SoundController::~SoundController()
{
    clearAll();
}
SoundController &SoundController::getInstance()
{
    static SoundController instance;
    return instance;
}

void SoundController::playPlayerStateSFX(playerStateType type)
{
    if (loadedPlayerStates.count(type) == 0 && playerStatePaths.count(type) != 0)
    {
        playerStateSFX.loadSingle(type, playerStatePaths.at(type));
        loadedPlayerStates.insert(type);
    }
    playerStateSFX.play(type);
}

void SoundController::playBlockStateSFX(blockStateType type)
{
    if (loadedBlockStates.count(type) == 0 && blockStatePaths.count(type) != 0)
    {
        blockStateSFX.loadSingle(type, blockStatePaths.at(type));
        loadedBlockStates.insert(type);
    }
    blockStateSFX.play(type);
}

void SoundController::playPlayerVoiceSFX(marioVoice type)
{
    if (loadedPlayerVoices.count(type) == 0 && marioVoiceMap.count(type) != 0)
    {
        marioVoiceSFX.loadSingle(type, marioVoiceMap.at(type));
        loadedPlayerVoices.insert(type);
    }
    marioVoiceSFX.play(type);
}

void SoundController::playSceneSFX(sceneType type)
{
    if (loadedSceneSFX.count(type) == 0 && sceneSFXMap.count(type) != 0)
    {
        sceneSFX.loadSingle(type, sceneSFXMap.at(type));
        loadedSceneSFX.insert(type);
    }
    sceneSFX.play(type);
}

void SoundController::playSceneMusic(sceneType type)
{
    if (loadedSceneMusic.count(type) == 0 && sceneMusicMap.count(type) != 0)
    {
        auto [filePath, loop] = sceneMusicMap.at(type);
        sceneMusic.loadSingle(type, filePath, loop);
        loadedSceneMusic.insert(type);
    }
    sceneMusic.play(type);
}

void SoundController::playSceneMusicFromStart(sceneType type)
{
    if (loadedSceneMusic.count(type) == 0 && sceneMusicMap.count(type) != 0)
    {
        auto [filePath, loop] = sceneMusicMap.at(type);
        sceneMusic.loadSingle(type, filePath, loop);
        loadedSceneMusic.insert(type);
    }
    sceneMusic.playFromStart(type);
}

void SoundController::playTemporarySceneMusic(sceneType temp)
{
    if (loadedSceneMusic.count(temp) == 0 && sceneMusicMap.count(temp) != 0)
    {
        auto [filePath, loop] = sceneMusicMap.at(temp);
        sceneMusic.loadSingle(temp, filePath, loop);
        loadedSceneMusic.insert(temp);
    }
    sceneMusic.playTemporary(temp);
}
void SoundController::clearAll()
{
    marioVoiceSFX.clear();
    playerStateSFX.clear();
    blockStateSFX.clear();
    sceneMusic.clear();
    sceneSFX.clear();
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
    playerStateSFX.setVolume(vol);
    blockStateSFX.setVolume(vol);
    marioVoiceSFX.setVolume(vol);
    sceneSFX.setVolume(vol);
}

void SoundController::setMasterMusicVolume(float vol)
{
    sceneMusic.setVolume(vol);
}
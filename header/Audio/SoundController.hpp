#pragma once
#include "raylib.h"
#include "Structs.hpp"
#include "AudioGroup.hpp"
using namespace std;

class SoundController
{
private:
    std::string generalPath = "assets/Audio/";

    SFXGroup<marioStateType> marioStateSFX;
    SFXGroup<blockStateType> blockStateSFX;
    SFXGroup<marioVoice> marioVoiceSFX;
    SFXGroup<sceneType> sceneSFX;
    MusicGroup<sceneType> sceneMusic;
    SoundController();
    ~SoundController();
    void loadMarioStateSFX();
    void loadBlockStateSFX();
    void loadSceneSFX();
    void loadSceneMusic();
    void loadMarioVoice();
    void clearAll();

public:
    static SoundController &getInstance();

    SoundController(const SoundController &) = delete;
    SoundController &operator=(const SoundController &) = delete;

    // chơi sound effect
    void playMarioStateSFX(marioStateType type);
    void playBlockStateSFX(blockStateType type);
    void playMarioVoiceSFX(marioVoice type);
    void playSceneSFX(sceneType type);

    // đổi scene nên đổi nhạc
    void playSceneMusic(sceneType type);
    void playSceneMusicFromStart(sceneType type);
    // Play a temporary scene music (pause current, play temp, auto-restore when done)
    void playTemporarySceneMusic(sceneType temp);

    // tác động nhạc của scene hiện tại
    void updateSceneMusic();
    void pauseSceneMusic();
    void resumeSceneMusic();
    void stopSceneMusic();

    // chỉnh volume
    void setMasterSFXVolume(float vol);
    void setMasterMusicVolume(float vol);
};
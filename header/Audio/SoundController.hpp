#pragma once
#include "raylib.h"
#include "Structs.hpp"
#include "AudioGroup.hpp"
#include "unordered_set"
using namespace std;

class SoundController
{
private:
    string generalPath = "assets/Audio/";

    SFXGroup<marioStateType> marioStateSFX;
    SFXGroup<blockStateType> blockStateSFX;
    SFXGroup<marioVoice> marioVoiceSFX;
    SFXGroup<sceneType> sceneSFX;
    MusicGroup<sceneType> sceneMusic;
    SoundController();
    ~SoundController();

    void clearAll();
    const unordered_map<marioStateType, string> marioStatePaths = {
        {marioStateType::JUMP, "jump.wav"},
        {marioStateType::SMALLJUMP, "jumpsmall.wav"},
        {marioStateType::CROUCH, "crouch.wav"},
        {marioStateType::GROW, "change_big.wav"},
        {marioStateType::UNGROW, "change_small.wav"},
        {marioStateType::DEAD, "death.wav"},
        {marioStateType::THROWFB, "throw_fireball.wav"},
        {marioStateType::SQUISH_ENEMY, "stomp.wav"},
        {marioStateType::KICK_SHELL, "kick.wav"}};

    const unordered_map<blockStateType, string> blockStatePaths = {
        {blockStateType::BOUNCE, "bounce_block.wav"},
        {blockStateType::BREAK, "break_block.wav"},
        {blockStateType::SPAWNITEM, "spawn_item.wav"}};

    const unordered_map<marioVoice, string> marioVoiceMap = {
        {marioVoice::LETSGO, "here_we_go.wav"},
        {marioVoice::JUMP, "hoo.wav"},
        {marioVoice::HURT, "owowowow.wav"}
        // Add more sceneType mappings as needed
    };

    const unordered_map<sceneType, pair<string, bool>> sceneMusicMap = {
        {sceneType::MENU, {"Menu.mp3", true}},
        {sceneType::GAME, {"level_1(3).mp3", true}},
        {sceneType::GAMEOVER, {"GameOver.mp3", true}},
        {sceneType::CHOOSE_LEVEL, {"ChooseLevel.mp3", true}},
        {sceneType::STAR, {"star_man.mp3", false}}};

    const unordered_map<sceneType, string> sceneSFXMap = {
        {sceneType::PAUSE, "Pause.wav"}};
    // same for marioVoice and sceneMusic
    unordered_set<marioStateType> loadedMarioStates;
    unordered_set<blockStateType> loadedBlockStates;
    unordered_set<marioVoice> loadedMarioVoices;
    unordered_set<sceneType> loadedSceneSFX;
    unordered_set<sceneType> loadedSceneMusic;

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
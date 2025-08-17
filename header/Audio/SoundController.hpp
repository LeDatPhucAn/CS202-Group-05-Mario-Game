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

    SFXGroup<playerStateType> playerStateSFX;
    SFXGroup<blockStateType> blockStateSFX;
    SFXGroup<marioVoice> marioVoiceSFX;
    SFXGroup<sceneType> sceneSFX;
    MusicGroup<sceneType> sceneMusic;
    SoundController();
    ~SoundController();

    void clearAll();
    const unordered_map<playerStateType, string> playerStatePaths = {
        {playerStateType::JUMP, "jump.wav"},
        {playerStateType::SMALLJUMP, "jumpsmall.wav"},
        {playerStateType::CROUCH, "crouch.wav"},
        {playerStateType::GROW, "change_big.wav"},
        {playerStateType::UNGROW, "change_small.wav"},
        {playerStateType::DEAD, "death.wav"},
        {playerStateType::THROWFB, "throw_fireball.wav"},
        {playerStateType::SQUISH_ENEMY, "stomp.wav"},
        {playerStateType::KICK_SHELL, "kick.wav"}};

    const unordered_map<blockStateType, string> blockStatePaths = {
        {blockStateType::BOUNCE, "bounce_block.wav"},
        {blockStateType::BREAK, "break_block.wav"},
        {blockStateType::SPAWNITEM, "spawn_item.wav"},
        {blockStateType::GETCOIN, "coin_get.wav"}};

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
        {sceneType::STAR, {"star_man.mp3", false}},
        {sceneType::CREDITS, {"Credits.mp3", true}}};

    const unordered_map<sceneType, string> sceneSFXMap = {
        {sceneType::PAUSE, "Pause.wav"},
        {sceneType::WIN_LEVEL, "level_clear.mp3"}};
    // same for marioVoice and sceneMusic
    unordered_set<playerStateType> loadedPlayerStates;
    unordered_set<blockStateType> loadedBlockStates;
    unordered_set<marioVoice> loadedPlayerVoices;
    unordered_set<sceneType> loadedSceneSFX;
    unordered_set<sceneType> loadedSceneMusic;

public:
    static SoundController &getInstance();

    SoundController(const SoundController &) = delete;
    SoundController &operator=(const SoundController &) = delete;

    // chơi sound effect
    void playPlayerStateSFX(playerStateType type);
    void playBlockStateSFX(blockStateType type);
    void playPlayerVoiceSFX(marioVoice type);
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
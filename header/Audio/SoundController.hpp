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
    MusicGroup<sceneType> sceneMusic;

    SoundController();
    void loadMarioStateSFX();
    void loadBlockStateSFX();
    void loadSceneMusic();

public:
    static SoundController &getInstance();

    SoundController(const SoundController &) = delete;
    SoundController &operator=(const SoundController &) = delete;

    // chơi sound effect
    void playMarioStateSFX(marioStateType type);
    void playBlockStateSFX(blockStateType type);

    // đổi scene nên đổi nhạc
    void playSceneMusic(sceneType type);

    // tác động nhạc của scene hiện tại
    void updateSceneMusic();
    void pauseSceneMusic();
    void resumeSceneMusic();
    void stopSceneMusic();
    void clearAll();
};
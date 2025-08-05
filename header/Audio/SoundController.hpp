#include "raylib.h"
#include "Structs.hpp"
#include <unordered_map>
#include <string>
using namespace std;

class SoundController
{
private:
    unordered_map<marioStateType, Sound> marioStateSFX;

    // Private constructor
    SoundController()
    {
        loadMarioStateSFX();
    }

    // Delete Copy Constructor and Assignment Operator
    SoundController(const SoundController &) = delete;
    SoundController &operator=(const SoundController &) = delete;
    void loadSoundTrack();
    void loadMarioVoice();
    void loadMarioStateSFX();

public:
    // Singleton Accessor
    static SoundController &getInstance()
    {
        static SoundController instance;
        return instance;
    }

    void playMarioStateSFX(marioStateType type);
    void playMarioVoice();
    void playSoundTrack();
    void pause();
    void resume();
    void clearAll();
};

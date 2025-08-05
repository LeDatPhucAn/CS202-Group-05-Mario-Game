#include "raylib.h"
#include "Structs.hpp"
#include <unordered_map>
#include <string>
using namespace std;

class SoundController
{
private:
    unordered_map<marioStateType, Sound> marioStateSFX;
    unordered_map<blockStateType, Sound> blockStateSFX;

    // Private constructor
    SoundController()
    {
        loadMarioStateSFX();
        loadBlockStateSFX();
    }
    string generalPath = "assets/Audio/";
    // Delete Copy Constructor and Assignment Operator
    SoundController(const SoundController &) = delete;
    SoundController &operator=(const SoundController &) = delete;
    void loadSoundTrack();
    void loadMarioVoice();
    void loadMarioStateSFX();
    void loadBlockStateSFX();

private:
    template <typename StateType>
    void loadStateSFX(unordered_map<StateType, Sound> &sfxMap, const string &path, const unordered_map<StateType, string> &sfxFileMap)
    {
        for (const auto &pair : sfxFileMap)
        {
            string fullPath = path + pair.second;
            sfxMap[pair.first] = LoadSound(fullPath.c_str());
        }
    }
    template <typename StateType>
    void playStateSFX(unordered_map<StateType, Sound> &sfxMap, StateType type, float minPitch = 0.95f, float maxPitch = 1.05f)
    {
        if (sfxMap.count(type))
        {
            float randomPitch = minPitch + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxPitch - minPitch)));
            SetSoundPitch(sfxMap[type], randomPitch);
            PlaySound(sfxMap[type]);
        }
    }

public:
    // Singleton Accessor
    static SoundController &getInstance()
    {
        static SoundController instance;
        return instance;
    }

    void playBlockStateSFX(blockStateType type);
    void playMarioStateSFX(marioStateType type);
    void playMarioVoice();
    void playSoundTrack();
    void pause();
    void resume();
    void clearAll();
};

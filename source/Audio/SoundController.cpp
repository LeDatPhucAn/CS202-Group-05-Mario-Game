#include "SoundController.hpp"

void SoundController::loadMarioStateSFX()
{
    string path = "assets/Audio/motion/";

    unordered_map<marioStateType, string> sfxFileMap = {
        {marioStateType::JUMP, "jump.wav"},
        {marioStateType::SKID, "Skid.wav"},
        {marioStateType::CROUCH, "crouch.wav"},
        {marioStateType::GROW, "change_big.wav"},
        {marioStateType::UNGROW, "change_small.wav"},
        {marioStateType::DEAD, "death.wav"},
        {marioStateType::THROWFB, "throw_fireball.wav"}};

    for (const auto &pair : sfxFileMap)
    {
        string fullPath = path + pair.second;
        marioStateSFX[pair.first] = LoadSound(fullPath.c_str());
    }
}

void SoundController::playMarioStateSFX(marioStateType type)
{
    if (marioStateSFX.count(type))
    {
        float randomPitch = 0.9f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.1f - 1.0f)));
        SetSoundPitch(marioStateSFX[type], randomPitch);
        PlaySound(marioStateSFX[type]);
    }
}

void SoundController::clearAll()
{
    for (auto &pair : marioStateSFX)
    {
        UnloadSound(pair.second);
    }
    marioStateSFX.clear();
}

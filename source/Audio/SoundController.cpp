#include "SoundController.hpp"

void SoundController::loadMarioStateSFX()
{
    string path = generalPath + "Motion/";
    unordered_map<marioStateType, string> sfxFileMap = {
        {marioStateType::JUMP, "jump.wav"},
        {marioStateType::SKID, "Skid.wav"},
        {marioStateType::CROUCH, "crouch.wav"},
        {marioStateType::GROW, "change_big.wav"},
        {marioStateType::UNGROW, "change_small.wav"},
        {marioStateType::DEAD, "death.wav"},
        {marioStateType::THROWFB, "throw_fireball.wav"}};
    loadStateSFX(marioStateSFX, path, sfxFileMap);
}

void SoundController::playMarioStateSFX(marioStateType type)
{
    playStateSFX(marioStateSFX, type);
}
void SoundController::loadBlockStateSFX()
{
    string path = generalPath + "Block/";

    unordered_map<blockStateType, string> sfxFileMap = {
        {blockStateType::BOUNCE, "bounce_block.wav"},
        {blockStateType::BREAK, "break_block.wav"},
        {blockStateType::SPAWNITEM, "spawn_item.wav"}};

    loadStateSFX(blockStateSFX, path, sfxFileMap);
}
void SoundController::playBlockStateSFX(blockStateType type)
{
    playStateSFX(blockStateSFX, type);
}

void SoundController::clearAll()
{
    auto unloadAll = [](auto &sfxMap)
    {
        for (auto &pair : sfxMap)
        {
            UnloadSound(pair.second);
        }
        sfxMap.clear();
    };

    unloadAll(marioStateSFX);
    unloadAll(blockStateSFX);
}

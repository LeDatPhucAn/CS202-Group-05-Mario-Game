#include <unordered_map>
#include "Scene.hpp"
#include <string>
#include <optional>
#include <iostream>
template <typename EnumType>
class SFXGroup
{
private:
    unordered_map<EnumType, Sound> stateSFX;
    string path;
    float volume = 0.5f;

public:
    SFXGroup(const string &basePath) : path(basePath) {}

    void load(const unordered_map<EnumType, string> &sfxFileMap)
    {
        for (const auto &pair : sfxFileMap)
        {
            string fullPath = path + pair.second;
            stateSFX[pair.first] = LoadSound(fullPath.c_str());
        }
    }

    void play(EnumType type, float minPitch = 0.95f, float maxPitch = 1.05f)
    {
        if (stateSFX.count(type))
        {
            float randomPitch = minPitch + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxPitch - minPitch)));
            SetSoundPitch(stateSFX[type], randomPitch);
            SetSoundVolume(stateSFX[type], volume);
            PlaySound(stateSFX[type]);
        }
    }

    void clear()
    {
        for (auto &pair : stateSFX)
        {
            UnloadSound(pair.second);
        }
        stateSFX.clear();
    }
    void setVolume(float newVolume)
    {
        volume = newVolume;
    }
};
template <typename EnumType>
class MusicGroup
{
private:
    struct MusicData
    {
        Music track;
        bool loop;
    };
    float volume = 0.5f;

    unordered_map<EnumType, MusicData> bgmTracks;
    string path;
    optional<EnumType> currentlyPlaying; // Keep track of which music is playing

public:
    MusicGroup(const string &basePath) : path(basePath) {}

    void load(const unordered_map<EnumType, pair<string, bool>> &musicFileMap)
    {
        for (const auto &pair : musicFileMap)
        {
            string fullPath = path + pair.second.first;
            Music music = LoadMusicStream(fullPath.c_str());
            bgmTracks[pair.first] = {music, pair.second.second};
        }
    }

    void play(EnumType type)
    {
        if (bgmTracks.count(type))
        {
            StopMusicStream(bgmTracks[type].track);       // This ensures it stops and resets position
            SeekMusicStream(bgmTracks[type].track, 0.0f); // Ensure seek to 0 even after stop (extra safety)
            SetMusicVolume(bgmTracks[type].track, volume);
            PlayMusicStream(bgmTracks[type].track);
            currentlyPlaying = type;
        }
    }

    void stop()
    {
        if (currentlyPlaying.has_value() && bgmTracks.count(currentlyPlaying.value()))
        {
            StopMusicStream(bgmTracks[currentlyPlaying.value()].track);
            currentlyPlaying.reset();
        }
    }

    void pause()
    {
        if (currentlyPlaying.has_value() && bgmTracks.count(currentlyPlaying.value()))
        {
            PauseMusicStream(bgmTracks[currentlyPlaying.value()].track);
        }
    }

    void resume()
    {
        if (currentlyPlaying.has_value() && bgmTracks.count(currentlyPlaying.value()))
        {
            ResumeMusicStream(bgmTracks[currentlyPlaying.value()].track);
        }
    }

    void update()
    {
        if (currentlyPlaying.has_value())
        {
            auto &data = bgmTracks[currentlyPlaying.value()];
            UpdateMusicStream(data.track);

            if (!data.loop)
            {
                float timePlayed = GetMusicTimePlayed(data.track);
                float timeLength = GetMusicTimeLength(data.track);

                if (fabs(timePlayed - timeLength) < 0.01f)
                {
                    StopMusicStream(data.track);
                    currentlyPlaying.reset(); // No longer playing
                }
            }
        }
    }

    void clear()
    {
        for (auto &pair : bgmTracks)
        {
            UnloadMusicStream(pair.second.track);
        }
        bgmTracks.clear();
    }

    void setVolume(float newVolume)
    {
        volume = newVolume;
        if (currentlyPlaying.has_value())
        {
            SetMusicVolume(bgmTracks[currentlyPlaying.value()].track, volume);
        }
    }
};

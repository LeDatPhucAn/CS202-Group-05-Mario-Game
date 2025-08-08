#include <unordered_map>
#include "Scene.hpp"
#include <string>
#include <optional>
#include <iostream>
#include <cmath>
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
        float elapsed = 0.0f; // accumulated play time (seconds)
        bool paused = false;  // whether this track is paused
    };
    float volume = 0.5f;

    unordered_map<EnumType, MusicData> bgmTracks;
    string path;
    std::optional<EnumType> currentlyPlaying; // active track
    std::optional<EnumType> previousMusic;    // track to return to after temporary one

public:
    MusicGroup(const string &basePath) : path(basePath) {}

    void load(const unordered_map<EnumType, pair<string, bool>> &musicFileMap)
    {
        for (const auto &pair : musicFileMap)
        {
            string fullPath = path + pair.second.first;
            Music music = LoadMusicStream(fullPath.c_str());
            bgmTracks[pair.first] = {music, pair.second.second, 0.0f, false};
        }
    }

    // Play from start (reset elapsed)
    void playFromStart(EnumType type)
    {
        auto it = bgmTracks.find(type);
        if (it != bgmTracks.end())
        {
            // stop & reset internal elapsed
            StopMusicStream(it->second.track);
            SeekMusicStream(it->second.track, 0.0f);
            it->second.elapsed = 0.0f;
            it->second.paused = false;

            SetMusicVolume(it->second.track, volume);
            PlayMusicStream(it->second.track);
            currentlyPlaying = type;
        }
    }

    // Play (resume or start — we sync elapsed with raylib's current position)
    void play(EnumType type)
    {
        auto it = bgmTracks.find(type);
        if (it != bgmTracks.end())
        {
            // try to sync elapsed to current stream position (useful if resumed externally)
            it->second.elapsed = GetMusicTimePlayed(it->second.track);
            it->second.paused = false;

            SetMusicVolume(it->second.track, volume);
            PlayMusicStream(it->second.track);
            currentlyPlaying = type;
        }
    }

    // Play temporary (remember previous)
    void playTemporary(EnumType type)
    {
        if (currentlyPlaying.has_value())
        {
            previousMusic = currentlyPlaying; // remember old track
        }
        playFromStart(type);
    }

    // Stop currently playing
    void stop()
    {
        if (currentlyPlaying.has_value())
        {
            auto it = bgmTracks.find(currentlyPlaying.value());
            if (it != bgmTracks.end())
            {
                StopMusicStream(it->second.track);
                it->second.paused = false;
                it->second.elapsed = 0.0f;
            }
            currentlyPlaying.reset();
        }
    }

    // Pause the currently playing track (keep elapsed)
    void pause()
    {
        if (currentlyPlaying.has_value())
        {
            auto it = bgmTracks.find(currentlyPlaying.value());
            if (it != bgmTracks.end())
            {
                PauseMusicStream(it->second.track);
                it->second.paused = true;
            }
        }
    }

    // Pause a specific track (helper, doesn't change currentlyPlaying)
    void pauseTrack(EnumType type)
    {
        auto it = bgmTracks.find(type);
        if (it != bgmTracks.end())
        {
            PauseMusicStream(it->second.track);
            it->second.paused = true;
        }
    }

    // Resume the currentlyPlaying track
    void resume()
    {
        if (currentlyPlaying.has_value())
        {
            auto it = bgmTracks.find(currentlyPlaying.value());
            if (it != bgmTracks.end())
            {
                ResumeMusicStream(it->second.track);
                it->second.paused = false;
            }
        }
    }

    // Resume a specific paused track and mark it currentlyPlaying
    void resumeTrack(EnumType type)
    {
        auto it = bgmTracks.find(type);
        if (it != bgmTracks.end())
        {
            ResumeMusicStream(it->second.track);
            it->second.paused = false;
            currentlyPlaying = type;
            // keep elapsed as-is (we paused earlier)
            SetMusicVolume(it->second.track, volume);
        }
    }

    // Must be called every frame
    void update()
    {
        if (currentlyPlaying.has_value())
        {
            auto &data = bgmTracks[currentlyPlaying.value()];

            // Stream the audio buffers
            UpdateMusicStream(data.track);

            // Only advance elapsed if not paused
            if (!data.paused)
            {
                data.elapsed += GetFrameTime();
            }

            if (!data.loop)
            {
                // get track length (stable)
                float timeLength = GetMusicTimeLength(data.track);

                // if elapsed reaches or exceeds length -> stop and restore previous
                if (data.elapsed + 1e-3f >= timeLength) // small epsilon
                {
                    StopMusicStream(data.track);
                    data.paused = false;
                    data.elapsed = 0.0f;
                    currentlyPlaying.reset();

                    // restore previous (resume) if present
                    if (previousMusic.has_value())
                    {
                        EnumType prev = previousMusic.value();
                        previousMusic.reset();
                        // resumeTrack will ResumeMusicStream and set currentlyPlaying
                        resumeTrack(prev);
                    }
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
        currentlyPlaying.reset();
        previousMusic.reset();
    }

    void setVolume(float newVolume)
    {
        volume = newVolume;
        if (currentlyPlaying.has_value())
        {
            SetMusicVolume(bgmTracks[currentlyPlaying.value()].track, volume);
        }
    }

    std::optional<EnumType> getCurrentlyPlaying() const
    {
        return currentlyPlaying;
    }
};
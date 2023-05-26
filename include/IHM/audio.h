#pragma once
#include <AL/al.h>
#include <AL/alc.h>

#include <vector>
#include <string>

struct AudioContext;
struct AudioMedia;

static std::vector<std::string> device_list;
void updateDeviceList();

struct AudioContext {
    ALCdevice* device;
    ALCcontext* context;

    AudioContext(const char* device_name = nullptr);
    ~AudioContext();

    bool makeCurrent();
};

struct AudioMedia {
    ALuint source;
    ALuint buffer;
    ALint status;

    AudioMedia(const char* file);
    ~AudioMedia();

    void play();
    void pause();
    void stop();
    void rewind();

    void update();
    bool isPlaying();
};

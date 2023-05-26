/** Inspiration: https://loulou.developpez.com/tutoriels/openal/premiers-pas/#LII */

#include "IHM/audio.h"
#include <sndfile.h>
#include <cstring>
#include <iostream>
#include <iomanip>

void updateDeviceList() {
    device_list.clear();
    const char* device_list_alc = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
    if(device_list_alc) {
        while(strlen(device_list_alc) > 0) {
            device_list.push_back(device_list_alc);
            device_list_alc += strlen(device_list_alc) + 1;
        }
    }

}

AudioContext::AudioContext(const char* device_name) {
    device = alcOpenDevice(device_name);
    if(!device) {
        throw std::runtime_error("Could not open audio device");
    }
    context = alcCreateContext(device, nullptr);
    if(!context) {
        throw std::runtime_error("Could not create audio context");
    }
    if(!makeCurrent()) {
        throw std::runtime_error("Could not use audio context");
    }
}

AudioContext::~AudioContext() {
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool AudioContext::makeCurrent() {
    return alcMakeContextCurrent(context);
}


AudioMedia::AudioMedia(const char* file_path) {
    SF_INFO file_infos;
    SNDFILE* file = sf_open(file_path, SFM_READ, &file_infos);
    if(!file) {
        throw std::runtime_error(std::string(file_path) + " not found");
    }

    ALsizei nb_samples  = static_cast<ALsizei>(file_infos.channels * file_infos.frames);
    ALsizei sample_rate = static_cast<ALsizei>(file_infos.samplerate);
    std::vector<ALshort> samples(nb_samples);
    if (sf_read_short(file, &samples[0], nb_samples) < nb_samples) {
        throw std::runtime_error(std::string("Could not read ") + file_path);
    }
    sf_close(file);

    ALenum format;
    switch (file_infos.channels) {
        case 1: format = AL_FORMAT_MONO16;   break;
        case 2: format = AL_FORMAT_STEREO16; break;
        default: {
            throw std::runtime_error(std::string(file_path) + " must be either mono or stereo");
        }
    }

    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, &samples[0], nb_samples * sizeof(ALushort), sample_rate);
    if(alGetError() != AL_NO_ERROR) {
        throw std::runtime_error(std::string("Something went wrong while loading ") + file_path);
    }

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
}

AudioMedia::~AudioMedia() {
    alDeleteBuffers(1, &buffer);
    alSourcei(source, AL_BUFFER, 0);
    alDeleteSources(1, &source);
    alListener3f(AL_POSITION, 0, 0, 0);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    ALfloat orientation[] = {0,0,1,0,1,0};
    alListenerfv(AL_ORIENTATION, orientation);
}

void AudioMedia::play() {
    alSourcePlay(source);
}
void AudioMedia::pause() {
    alSourcePause(source);
}
void AudioMedia::stop() {
    alSourceStop(source);
}
void AudioMedia::rewind() {
    alSourceRewind(source);
}

void AudioMedia::update() {
    alGetSourcei(source, AL_SOURCE_STATE, &status);
}

bool AudioMedia::isPlaying() {
    return status == AL_PLAYING;
}

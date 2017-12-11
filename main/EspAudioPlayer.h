#pragma once
#include "AudioPlayer.h"

class EspAudioPlayer :
    public AudioPlayer
{
    public:
        EspAudioPlayer();
        virtual void startAudio() override;
        virtual void stopAudio() override;
};

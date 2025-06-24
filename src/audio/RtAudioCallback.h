#ifndef RT_AUDIO_CALLBACK
#define RT_AUDIO_CALLBACK

#include "AudioDefinitions.h"
#include "RtAudio.h"

static int audioCallback(void* output_buffer, void* input_buffer, unsigned int number_of_frames,
    double stream_time, RtAudioStreamStatus status, void* user_data)
{
    auto queue = reinterpret_cast<AudioQueue*>(user_data);
    auto output = static_cast<float*>(output_buffer);

    for (int i = 0; i < number_of_frames; i++) {
        float sample = 0.0f;
        if (queue->pop(sample)) {
            output[i] = sample;
        } else {
            output[i] = 0.0f;
        }
    }

    return 0;
}

#endif // RT_AUDIO_CALLBACK

#ifndef RT_AUDIO_CALLBACK
#define RT_AUDIO_CALLBACK

#include "RtAudio.h"

static int audioCallback(void* output_buffer, void* input_buffer, unsigned int number_of_frames,
    double stream_time, RtAudioStreamStatus status, void* user_data)
{
    auto* buffer = reinterpret_cast<boost::lockfree::queue<float>*>(user_data);
    float* output = static_cast<float*>(output_buffer);

    for (unsigned int i = 0; i < number_of_frames; ++i) {
        float sample = 0.0f;
        if (!buffer->pop(sample)) {
            // Buffer underflow: just output silence
            sample = 0.0f;
        }
        output[i] = sample;
    }

    return 0;
}

#endif // RT_AUDIO_CALLBACK

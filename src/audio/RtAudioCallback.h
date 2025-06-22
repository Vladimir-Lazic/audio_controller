#ifndef RT_AUDIO_CALLBACK
#define RT_AUDIO_CALLBACK

#include "RtAudio.h"
#include "AudioDefinitions.h"

static int audioCallback(void* output_buffer, void* input_buffer, unsigned int number_of_frames,
    double stream_time, RtAudioStreamStatus status, void* user_data)
{
    auto* playback_buffer = static_cast<WaveformBuffer*>(user_data);
    auto output = static_cast<float*>(output_buffer);

    size_t available = std::min(static_cast<size_t>(number_of_frames), playback_buffer->size());

    std::copy(playback_buffer->begin(), playback_buffer->begin() + available, output);

    return 0;
}

#endif // RT_AUDIO_CALLBACK

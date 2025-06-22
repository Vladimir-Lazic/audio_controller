#ifndef RT_AUDIO_CALLBACK
#define RT_AUDIO_CALLBACK

#include "RtAudio.h"

static int audioCallback(void* output_buffer, void* input_buffer, unsigned int number_of_frames,
    double stream_time, RtAudioStreamStatus status, void* user_data)
{
    auto* controller = reinterpret_cast<AudioController*>(user_data);
    float* output = static_cast<float*>(output_buffer);

    const auto& buffer = controller->getPlaybackBuffer();
    size_t available = std::min(static_cast<size_t>(number_of_frames), buffer.size());

    std::copy(buffer.begin(), buffer.begin() + available, output);

    return 0;
}

#endif // RT_AUDIO_CALLBACK

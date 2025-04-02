//
// Created by MattFor on 31.03.2025.
//

#include <thread>
#include <windows.h>
#include <algorithm>

#include "sound.hpp"

void playTone(const int value, const int duration_ms)
{
    if (duration_ms < 0)
    {
        return;
    }

    std::thread([value, duration_ms] {
        constexpr int minFrequency = 1;
        constexpr int maxFrequency = 5000;

        int frequency = minFrequency + value * (maxFrequency - minFrequency) / 120;
        frequency = std::clamp(frequency, minFrequency, maxFrequency);

        Beep(frequency, std::max(duration_ms, 10));
    }).detach();
}

void playTone(const float value, const int duration_ms)
{
    playTone(static_cast<int>(value), duration_ms);
}


//
// Created by MattFor on 31.03.2025.
//

// #include <cmath>
// #include <vector>
// #include <thread>
// #include <SDL3/SDL.h>
//
// // Helper: Generate a sine wave buffer given a frequency and duration.
// std::vector<float> generateSineWave(const int frequency, const int duration_ms, const int sample_rate = 44100)
// {
//     const int samples = sample_rate * duration_ms / 1000;
//     std::vector<float> buffer(samples);
//     const double twoPiF = 2.0 * M_PI * frequency;
//     for (int i = 0; i < samples; ++i)
//     {
//         constexpr float amplitude = 0.9f; // Normalized for float samples
//         const double t = static_cast<double>(i) / sample_rate;
//         buffer[i] = amplitude * std::sin(twoPiF * t);
//     }
//     return buffer;
// }
//
// void playTone(float value, int duration_ms)
// {
//     if (duration_ms <= 0)
//     {
//         return;
//     }
//
//     std::thread([value, duration_ms]() {
//         // Map value to frequency
//         const int frequency = 100 + static_cast<int>(value);
//
//         // Generate the sine wave buffer
//         const auto buffer = generateSineWave(frequency, duration_ms);
//
//         // Initialize SDL
//         if (SDL_Init(SDL_INIT_AUDIO) < 0)
//         {
//             SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
//             return;
//         }
//
//         // Set up the desired audio specification
//         SDL_AudioSpec spec;
//         SDL_zero(spec);
//         spec.freq = 44100;
//         spec.format = SDL_AUDIO_F32;
//         spec.channels = 1;
//
//         // Open the default audio device for playback
//         const SDL_AudioDeviceID dev = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
//         if (dev == 0)
//         {
//             SDL_Log("Failed to open audio device: %s", SDL_GetError());
//             SDL_Quit();
//             return;
//         }
//
//         // Create an audio stream to handle audio data
//         SDL_AudioStream *stream = SDL_CreateAudioStream(&spec, &spec);
//         if (!stream)
//         {
//             SDL_Log("Failed to create audio stream: %s", SDL_GetError());
//             SDL_CloseAudioDevice(dev);
//             SDL_Quit();
//             return;
//         }
//
//         // Put the generated audio data into the stream
//         if (SDL_PutAudioStreamData(stream, buffer.data(), buffer.size() * sizeof(float)) < 0)
//         {
//             SDL_Log("Failed to put audio data into stream: %s", SDL_GetError());
//             SDL_DestroyAudioStream(stream);
//             SDL_CloseAudioDevice(dev);
//             SDL_Quit();
//             return;
//         }
//
//         // Start playback
//         SDL_ResumeAudioDevice(dev);
//
//         // Wait for the tone to finish
//         SDL_Delay(duration_ms + 50);
//
//         // Clean up
//         SDL_DestroyAudioStream(stream);
//         SDL_CloseAudioDevice(dev);
//         SDL_Quit();
//     }).detach();
// }
//
// void playTone(int value, int duration_ms)
// {
//     if (duration_ms <= 0)
//     {
//         return;
//     }
//
//     std::thread([value, duration_ms]() {
//         // Map value to frequency
//         const int frequency = 100 + static_cast<int>(value);
//
//         // Generate the sine wave buffer
//         const auto buffer = generateSineWave(frequency, duration_ms);
//
//         // Initialize SDL
//         if (SDL_Init(SDL_INIT_AUDIO) < 0)
//         {
//             SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
//             return;
//         }
//
//         // Set up the desired audio specification
//         SDL_AudioSpec spec;
//         SDL_zero(spec);
//         spec.freq = 44100;
//         spec.format = SDL_AUDIO_F32;
//         spec.channels = 1;
//
//         // Open the default audio device for playback
//         const SDL_AudioDeviceID dev = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
//         if (dev == 0)
//         {
//             SDL_Log("Failed to open audio device: %s", SDL_GetError());
//             SDL_Quit();
//             return;
//         }
//
//         // Create an audio stream to handle audio data
//         SDL_AudioStream *stream = SDL_CreateAudioStream(&spec, &spec);
//         if (!stream)
//         {
//             SDL_Log("Failed to create audio stream: %s", SDL_GetError());
//             SDL_CloseAudioDevice(dev);
//             SDL_Quit();
//             return;
//         }
//
//         // Put the generated audio data into the stream
//         if (SDL_PutAudioStreamData(stream, buffer.data(), buffer.size() * sizeof(float)) < 0)
//         {
//             SDL_Log("Failed to put audio data into stream: %s", SDL_GetError());
//             SDL_DestroyAudioStream(stream);
//             SDL_CloseAudioDevice(dev);
//             SDL_Quit();
//             return;
//         }
//
//         // Start playback
//         SDL_ResumeAudioDevice(dev);
//
//         // Wait for the tone to finish
//         SDL_Delay(duration_ms + 50);
//
//         // Clean up
//         SDL_DestroyAudioStream(stream);
//         SDL_CloseAudioDevice(dev);
//         SDL_Quit();
//     }).detach();
// }

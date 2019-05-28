#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>
#include <cmath>
#include <chrono>
#include <unordered_map>

#include <alure2.h>
#include <phonon.h>
#include "Externals/soxr-code/src/soxr.h"

std::unordered_map<alure::String, alure::Vector<float>> audioCache;

std::vector<float> load_input_audio(const std::string filename)
{
  std::ifstream file(filename.c_str(), std::ios::binary);

  file.seekg(0, std::ios::end);
  auto filesize = file.tellg();
  auto numsamples = static_cast<int>(filesize / sizeof(float));

  std::vector<float> inputaudio(numsamples);
  file.seekg(0, std::ios::beg);
  file.read(reinterpret_cast<char*>(inputaudio.data()), filesize);

  return inputaudio;
}

void save_output_audio(const std::string filename, std::vector<float> outputaudio)
{
  std::ofstream file(filename.c_str(), std::ios::binary);
  file.write(reinterpret_cast<char*>(outputaudio.data()), outputaudio.size() * sizeof(float));
}

IPLVector3 rotate_clockwise_over_zero(double radius, std::chrono::milliseconds run_duration, double angularVelocity)
{
  return IPLVector3
  {
    (IPLfloat32)(std::sin(run_duration.count() * angularVelocity / 1000) * radius),
    0,
    (IPLfloat32)(std::cos(run_duration.count() * angularVelocity / 1000) * radius)
  };
}

IPLVector3 rotate_clockwise_over_zero(double radius, uint64_t run_duration, double angularVelocity)
{
  return IPLVector3
  {
    (IPLfloat32)(std::sin(run_duration * angularVelocity / 1000) * radius),
    0,
    (IPLfloat32)(std::cos(run_duration * angularVelocity / 1000) * radius)
  };
}

constexpr int selectedRate = 48000;
constexpr int selectedFramesize = 512;

int main(int argc, char** argv)
{

  // Init OpenAL
  auto al_dev_manager = alure::DeviceManager::getInstance();
  auto al_device = al_dev_manager.openPlayback();
  auto al_context = al_device.createContext();
  alure::Context::MakeCurrent(al_context);

  // Decode and resample single audio file
  auto dec = al_context.createDecoder("piec.wav");
  std::vector<int16_t> audioData(dec->getLength());
  dec->read(audioData.data(), dec->getLength());

  std::vector<float> resampledAudio(dec->getLength() * selectedRate / dec->getFrequency());
  auto audioSpec = soxr_io_spec(SOXR_INT16_I, SOXR_FLOAT32_I);
  auto qualityResampleQuality = soxr_quality_spec(SOXR_VHQ, 0);
  size_t odone = 0;
  auto soxError = soxr_oneshot(dec->getFrequency(), selectedRate, 1, audioData.data(), audioData.size(), nullptr, resampledAudio.data(), resampledAudio.size(), &odone, &audioSpec, &qualityResampleQuality, nullptr);

  // Add to cache

  // Create Alure Source

  // Create SteamAudio decoder

  // Link decoder to source

  // Play source

  // Move source (by moving decoder)

  // Stop source

  // Close source

  // Close software
  auto inputaudio = resampledAudio;

  IPLhandle context{ nullptr };
  iplCreateContext(nullptr, nullptr, nullptr, &context);

  auto const samplingrate = selectedRate;
  auto const framesize = 1024;
  IPLRenderingSettings settings{ samplingrate, framesize };

  IPLhandle renderer{ nullptr };
  IPLHrtfParams hrtfParams{ IPL_HRTFDATABASETYPE_DEFAULT, nullptr, 0 };
  iplCreateBinauralRenderer(context, settings, hrtfParams, &renderer);

  IPLAudioFormat mono;
  mono.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
  mono.channelLayout = IPL_CHANNELLAYOUT_MONO;
  mono.channelOrder = IPL_CHANNELORDER_INTERLEAVED;

  IPLAudioFormat stereo;
  stereo.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
  stereo.channelLayout = IPL_CHANNELLAYOUT_STEREO;
  stereo.channelOrder = IPL_CHANNELORDER_INTERLEAVED;

  IPLhandle effect{ nullptr };
  iplCreateBinauralEffect(renderer, mono, stereo, &effect);

  std::vector<float> outputaudioframe(2 * framesize);
  std::vector<float> outputaudio;

  IPLAudioBuffer inbuffer{ mono, framesize, inputaudio.data() };
  IPLAudioBuffer outbuffer{ stereo, framesize, outputaudioframe.data() };

  auto clock = std::chrono::steady_clock();
  auto start_time = clock.now();
  auto numframes = static_cast<int>(inputaudio.size() / framesize);
  uint64_t time = 0;
  for (auto i = 0; i < numframes; ++i)
  {
    auto pos = rotate_clockwise_over_zero(1, time, 2);
    iplApplyBinauralEffect(effect, renderer, inbuffer, pos, IPL_HRTFINTERPOLATION_NEAREST, outbuffer);
    std::copy(std::begin(outputaudioframe), std::end(outputaudioframe), std::back_inserter(outputaudio));
    inbuffer.interleavedBuffer += framesize;
    time += 16;
  }

  alure::Context::MakeCurrent(nullptr);
  al_context.destroy();
  al_device.close();

  iplDestroyBinauralEffect(&effect);
  iplDestroyBinauralRenderer(&renderer);
  iplDestroyContext(&context);
  iplCleanup();

  save_output_audio("outputaudio.raw", outputaudio);
  return 0;
}
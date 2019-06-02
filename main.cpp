#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>
#include <cmath>
#include <chrono>
#include <unordered_map>
#include <variant>

#include <alure2.h>
#include <phonon.h>
#include "Externals/soxr-code/src/soxr.h"

#include "Context.hpp"
#include "BinauralRenderer.hpp"
#include "SteamAudioDecoder.hpp"
#include "Helpers.hpp"
#include "Structs.hpp"

std::unordered_map<alure::String, alure::Vector<float>> audioCache;
constexpr int selectedRate = 48000;
constexpr int selectedFramesize = 512;
constexpr int selectedNumBuffersPerSource = 3;

std::shared_ptr<AudioBuffer> get_resampled_audio(std::string filename, alure::Context& al_context)
{
  std::vector<float> resampledAudio;

  auto dec = al_context.createDecoder(filename);
  std::vector<ALbyte> audioData(alure::FramesToBytes(dec->getLength(), dec->getChannelConfig(), dec->getSampleType()));
  dec->read(audioData.data(), dec->getLength());

  auto inputSampleType = dec->getSampleType();
  auto inputLength = dec->getLength();
  auto inputSize = audioData.size();
  auto inputFrequency = dec->getFrequency();
  auto inputChannelConfig = dec->getChannelConfig();

  resampledAudio.resize(inputSize / get_sample_size(inputSampleType) * selectedRate / inputFrequency + 1);

  auto runtimeSpec = soxr_runtime_spec(0);
  auto audioSpec = soxr_io_spec(get_sox_type(inputSampleType), SOXR_FLOAT32_I);
  auto qualityResampleQuality = soxr_quality_spec(SOXR_VHQ, 0);
  auto resampler = soxr_create(inputFrequency, selectedRate, get_channel_quantity(inputChannelConfig), nullptr, &audioSpec, &qualityResampleQuality, &runtimeSpec);

  size_t idone = 0;
  size_t odone = 0;
  size_t block = 250000;
  size_t j = 0;
  for (size_t i = 0; alure::FramesToBytes(i, inputChannelConfig, inputSampleType) < audioData.size();)
  {
    auto num = alure::FramesToBytes(i, inputChannelConfig, inputSampleType);
    size_t toProcess = block;
    if (alure::FramesToBytes(toProcess + i, inputChannelConfig, inputSampleType) > audioData.size())
    {
      toProcess = alure::BytesToFrames(audioData.size(), inputChannelConfig, inputSampleType) - i;
    }
    auto numBytes = alure::FramesToBytes(i, inputChannelConfig, inputSampleType);
    auto soxError = soxr_process(resampler, audioData.data() + alure::FramesToBytes(i, inputChannelConfig, inputSampleType),
      toProcess, &idone,
      resampledAudio.data() + j, resampledAudio.size() - j, &odone);

    i += idone;
    j += odone * get_channel_quantity(inputChannelConfig);
  }
  resampledAudio.resize(j);

  soxr_delete(resampler);
  return std::make_shared<AudioBuffer>(resampledAudio, alure::SampleType::Float32, alure::ChannelConfig::Stereo);
}

int main(int argc, char** argv)
{
  float pos_y = 1.0f;
  // Init OpenAL
  auto al_dev_manager = alure::DeviceManager::getInstance();
  auto al_device = al_dev_manager.openPlayback("OpenAL Soft");
  //auto al_device = al_dev_manager.openPlayback();
  auto al_context = al_device.createContext();
  alure::Context::MakeCurrent(al_context);

  // Decode and resample single audio file
  auto resampledAudio = get_resampled_audio("erro.mp3", al_context);

  // Add to cache

  // Create Alure Source
  auto source = al_context.createSource();

  // Create SteamAudio decoder
  auto context = std::make_shared<Context>();
  {
    auto const samplingrate = selectedRate;
    auto const framesize = selectedFramesize;
    IPLRenderingSettings settings{ samplingrate, framesize };
    IPLHrtfParams hrtfParams{ IPL_HRTFDATABASETYPE_DEFAULT, nullptr, 0 };

    auto renderer = std::make_shared<BinauralRenderer>(context, settings, hrtfParams);

    auto decoder = std::make_shared<SteamAudioDecoder>(renderer, framesize, resampledAudio);

    // Link decoder to source
    source.play(decoder, framesize, selectedNumBuffersPerSource);

    auto clock = std::chrono::steady_clock();
    auto now = clock.now();
    while (source.isPlaying())
    {
      decoder->setDirection(rotate_clockwise_over_zero(1, std::chrono::duration_cast<std::chrono::milliseconds>(now - clock.now()), 1));
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
      al_context.update();
    }

    source.destroy();
    alure::Context::MakeCurrent(nullptr);
    al_context.destroy();
    al_device.close();
  }
  context->Destroy();
  context->Cleanup();

  return 0;
}
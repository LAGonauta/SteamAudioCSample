#pragma once

#include <alure2.h>
#include <phonon.h>

#include "BinauralRenderer.hpp"
#include "BinauralEffect.hpp"
#include "EnvironmentalRenderer.hpp"
#include "DirectEffect.hpp"
#include "Structs.hpp"

class SteamAudioDecoder final : public alure::Decoder
{
private:
  std::shared_ptr<AudioBuffer> m_audioData{ nullptr };
  IPLDirectSoundPath m_sound_path { };
  size_t m_framesize = 0;
  size_t m_samplesplayed = 0;
  size_t m_setframesplayed = 0;
  size_t m_channelconfig = 1;
  IPLAudioFormat stereo;
  IPLAudioFormat mono;
  alure::SampleType m_sample_type;

  bool m_finished = false;

  BinauralEffect m_effect;
  DirectEffect m_direct_effect;

  IPLAudioFormat get_channel_format(alure::ChannelConfig config);

public:
  SteamAudioDecoder(std::shared_ptr<BinauralRenderer> renderer, std::shared_ptr<EnvironmentalRenderer> envRenderer, const size_t& frameSize, std::shared_ptr<AudioBuffer> decodedResampledAudio, alure::SampleType forcedOutputType = alure::SampleType::Float32);
  ~SteamAudioDecoder() override;

  ALuint read(ALvoid *ptr, ALuint count) noexcept override;

  void setPosition(const IPLDirectSoundPath& pos)
  {
    m_sound_path = pos;
  }

  ALuint getFrequency() const noexcept override
  {
    return 48000;
  };

  alure::ChannelConfig getChannelConfig() const noexcept override
  {
    return alure::ChannelConfig::Stereo;
  };

  alure::SampleType getSampleType() const noexcept override
  {
    return m_sample_type;
  }

  std::pair<uint64_t, uint64_t> getLoopPoints() const noexcept override
  {
    return std::pair<uint64_t, uint64_t>(0, UINT64_MAX);
  };

  uint64_t getLength() const noexcept override
  {
    return UINT64_MAX;
  };

  bool seek(uint64_t pos) noexcept override
  {
    return false;
  }

  bool isFinished() noexcept
  {
    return m_finished;
  }
};
#pragma once

#include <alure2.h>
#include <phonon.h>

#include "BinauralRenderer.hpp"
#include "BinauralEffect.hpp"
#include "Structs.hpp"

class SteamAudioDecoder final : public alure::Decoder
{
private:
  std::shared_ptr<AudioBuffer> m_audioData{ nullptr };
  IPLVector3 m_direction { 0.0f, 1.0f, 0.0f };
  size_t m_framesize = 0;
  size_t m_samplesplayed = 0;
  size_t m_setframesplayed = 0;
  size_t m_channelconfig = 1;
  IPLAudioFormat stereo;
  IPLAudioFormat mono;

  BinauralEffect m_effect;

  IPLAudioFormat get_channel_format(alure::ChannelConfig config);

public:
  SteamAudioDecoder(std::shared_ptr<BinauralRenderer> renderer, const size_t& frameSize, std::shared_ptr<AudioBuffer> decodedResampledAudio);
  ~SteamAudioDecoder() override;

  ALuint read(ALvoid *ptr, ALuint count) noexcept override;

  void setDirection(const IPLVector3& direction)
  {
    m_direction = direction;
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
    return alure::SampleType::Float32;
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
};
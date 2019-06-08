#pragma once

#include <alure2.h>
#include <phonon.h>

#include "BinauralRenderer.hpp"
#include "BinauralEffect.hpp"
#include "EnvironmentalRenderer.hpp"
#include "DirectEffect.hpp"
#include "ConvolutionEffect.hpp"
#include "Structs.hpp"

class SteamAudioDecoder final : public alure::Decoder
{
private:
  std::shared_ptr<AudioBuffer> m_audioData{ nullptr };
  std::shared_ptr<Environment> m_env{ nullptr };
  IPLSource m_source{0};
  IPLVector3 m_listenerPosition{0};
  IPLVector3 m_listenerAhead{0};
  IPLVector3 m_listenerUp{0};
  IPLfloat32 m_radius = 1;
  IPLDirectOcclusionMode m_occlusionMode = IPL_DIRECTOCCLUSION_NONE;
  IPLDirectOcclusionMethod m_occlusionMethod = IPL_DIRECTOCCLUSION_RAYCAST;

  size_t m_framesize = 0;
  size_t m_samplesplayed = 0;
  size_t m_setframesplayed = 0;
  size_t m_channelconfig = 1;
  IPLAudioFormat stereo{};
  IPLAudioFormat stereoDeint{};
  IPLAudioFormat mono{};
  IPLAudioFormat ambisonic{};
  alure::SampleType m_sample_type;

  bool m_finished = false;

  std::unique_ptr<BinauralEffect> m_binaural_effect;
  std::unique_ptr<DirectEffect> m_direct_effect;
  std::unique_ptr<ConvolutionEffect> m_conv_effect;

  IPLAudioFormat get_channel_format(alure::ChannelConfig config);

  IPLAudioBuffer inBuffer;

  std::vector<float> directOut;
  IPLAudioBuffer directOutBuffer;

  std::vector<float> binauralOutData;
  std::vector<float*> binauralOutChannels;
  IPLAudioBuffer binauralOutBuffer;

  std::vector<float> wetOutData;
  std::vector<float*> wetOutChannels;
  IPLAudioBuffer wetOutBuffer;

  std::vector<float> wetBinauralOutData;
  std::vector<float*> wetBinauralOutChannels;
  IPLAudioBuffer wetBinauralOutBuffer;

  std::vector<float> preOutBufferData;
  std::vector<float*> preOutBufferDataChannels;
  IPLAudioBuffer preOutBuffer;
  IPLAudioBuffer finalOutBuffer;

public:
  SteamAudioDecoder(std::shared_ptr<BinauralRenderer> renderer,
    std::shared_ptr<EnvironmentalRenderer> envRenderer,
    std::shared_ptr<Environment> env, const size_t& frameSize,
    std::shared_ptr<AudioBuffer> decodedResampledAudio,
    alure::SampleType forcedOutputType = alure::SampleType::Float32);
  //~SteamAudioDecoder() override;

  ALuint read(ALvoid *ptr, ALuint count) noexcept override;

  void setPosition(const IPLSource& pos)
  {
    m_source = pos;
  }

  void setListener(const IPLVector3& listenerPosition, const IPLVector3& listenerAhead, const IPLVector3& listenerUp)
  {
    m_listenerAhead = listenerAhead;
    m_listenerPosition = listenerPosition;
    m_listenerUp = listenerUp;
  }

  void setRadius(IPLfloat32 radius)
  {
    m_radius = radius;
  }

  IPLfloat32 getRadius()
  {
    return m_radius;
  }

  void setOcclusionMode(IPLDirectOcclusionMode mode)
  {
    m_occlusionMode = mode;
  }

  IPLDirectOcclusionMode getOcclusionMode()
  {
    return m_occlusionMode;
  }

  void setOcclusionMethod(IPLDirectOcclusionMethod method)
  {
    m_occlusionMethod = method;
  }

  IPLDirectOcclusionMethod getOcclusionMethod()
  {
    return m_occlusionMethod;
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
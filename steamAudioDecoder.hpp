#pragma once

#include <alure2.h>
#include <phonon.h>

class SteamAudioDecoder final : public alure::Decoder
{
  int(*VoiceSE_GetSoundDataCallback)(sfxcache_s *pCache, char *pCopyBuf, int maxOutDataSize, int samplePos, int sampleCount);

  alure::SharedPtr<std::vector<float>> audioData;
  std::atomic<IPLVector3> position;

public:
  SteamAudioDecoder(alure::SharedPtr<std::vector<float>> decodedResampledAudio);
  ~SteamAudioDecoder() override;

  void destroy();

  ALuint read(ALvoid *ptr, ALuint count) noexcept override;

  bool setPosition(const IPLVector3& pos)
  {
    position = pos;
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
    std::pair<uint64_t, uint64_t>(0, UINT64_MAX);
  };

  uint64_t getLength() const noexcept override;

  bool seek(uint64_t pos) noexcept override
  {
    return false;
  }
};
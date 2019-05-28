class AudioObject final
{
  int(*VoiceSE_GetSoundDataCallback)(sfxcache_s *pCache, char *pCopyBuf, int maxOutDataSize, int samplePos, int sampleCount);

  aud_channel_t *m_ch;

public:
  VoiceDecoder(sfx_t *sound, aud_channel_t *ch);
  ~VoiceDecoder() override;

  void destroy();

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

  bool hasLoopPoints() const noexcept override
  {
    return false;
  };

  std::pair<uint64_t, uint64_t> getLoopPoints() const noexcept override
  {
    std::pair<uint64_t, uint64_t>(0, UINT64_MAX);
  };

  uint64_t getLength() const noexcept override;
  bool seek(uint64_t pos) noexcept override
  {
    return false;
  }

  ALuint read(ALvoid *ptr, ALuint count) noexcept override;
};
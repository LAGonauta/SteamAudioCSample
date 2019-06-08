#include "SteamAudioDecoder.hpp"
#include "Helpers.hpp"

IPLAudioFormat SteamAudioDecoder::get_channel_format(alure::ChannelConfig config)
{
  IPLAudioFormat inputFormat;
  switch (config)
  {
  case alure::ChannelConfig::Mono:
    inputFormat = mono;
    break;
  case alure::ChannelConfig::Stereo:
    inputFormat = stereo;
    break;
  default:
    throw std::exception("Not supported");
  }
  return inputFormat;
}

SteamAudioDecoder::SteamAudioDecoder(std::shared_ptr<BinauralRenderer> renderer,
  std::shared_ptr<EnvironmentalRenderer> envRenderer,
  std::shared_ptr<Environment> env,
  const size_t& frameSize,
  std::shared_ptr<AudioBuffer> decodedResampledAudio,
  alure::SampleType forcedType)
  : m_sample_type(forcedType), m_audioData(decodedResampledAudio), m_framesize(frameSize), m_env(env)
{
  m_channelconfig = get_channel_quantity(m_audioData->channels);

  mono.channelLayoutType = IPLChannelLayoutType::IPL_CHANNELLAYOUTTYPE_SPEAKERS;
  mono.numSpeakers = 1;
  mono.channelLayout = IPLChannelLayout::IPL_CHANNELLAYOUT_MONO;
  mono.channelOrder = IPLChannelOrder::IPL_CHANNELORDER_INTERLEAVED;

  stereo.channelLayoutType = IPLChannelLayoutType::IPL_CHANNELLAYOUTTYPE_SPEAKERS;
  stereo.numSpeakers = 2;
  stereo.channelLayout = IPLChannelLayout::IPL_CHANNELLAYOUT_STEREO;
  stereo.channelOrder = IPLChannelOrder::IPL_CHANNELORDER_INTERLEAVED;

  stereoDeint.channelLayoutType = IPLChannelLayoutType::IPL_CHANNELLAYOUTTYPE_SPEAKERS;
  stereoDeint.numSpeakers = 2;
  stereoDeint.channelLayout = IPLChannelLayout::IPL_CHANNELLAYOUT_STEREO;
  stereoDeint.channelOrder = IPLChannelOrder::IPL_CHANNELORDER_DEINTERLEAVED;

  ambisonic.channelLayoutType = IPLChannelLayoutType::IPL_CHANNELLAYOUTTYPE_AMBISONICS;
  ambisonic.ambisonicsOrder = 0;
  auto ambisonicNumChannels = (ambisonic.ambisonicsOrder + 1) * (ambisonic.ambisonicsOrder + 1);
  ambisonic.ambisonicsOrdering = IPLAmbisonicsOrdering::IPL_AMBISONICSORDERING_ACN;
  ambisonic.ambisonicsNormalization = IPLAmbisonicsNormalization::IPL_AMBISONICSNORMALIZATION_N3D;
  ambisonic.channelOrder = IPLChannelOrder::IPL_CHANNELORDER_DEINTERLEAVED;

  m_direct_effect = std::make_unique<DirectEffect>(envRenderer, get_channel_format(decodedResampledAudio->channels), mono);
  m_binaural_effect = std::make_unique<BinauralEffect>(renderer, mono, stereoDeint);
  m_conv_effect = std::make_unique<ConvolutionEffect>(envRenderer, IPLBakedDataIdentifier {10, IPLBakedDataType::IPL_BAKEDDATATYPE_REVERB}, IPLSimulationType::IPL_SIMTYPE_REALTIME, get_channel_format(decodedResampledAudio->channels), ambisonic);

  inBuffer = IPLAudioBuffer{ get_channel_format(m_audioData->channels), static_cast<int32_t>(m_framesize), m_audioData->data.data() };

  directOut.resize(m_framesize);
  directOutBuffer = IPLAudioBuffer{ mono, static_cast<IPLint32>(m_framesize), directOut.data() };

  binauralOutData.resize(m_framesize * stereoDeint.numSpeakers);
  binauralOutChannels.resize(stereoDeint.numSpeakers);
  for (auto i = 0; i < stereoDeint.numSpeakers; ++i)
  {
    binauralOutChannels[i] = &binauralOutData[i * m_framesize];
  }
  binauralOutBuffer = IPLAudioBuffer{ stereoDeint, static_cast<IPLint32>(m_framesize), nullptr, binauralOutChannels.data() };

  wetOutData.resize(m_framesize * ambisonicNumChannels);
  wetOutChannels.resize(ambisonicNumChannels);
  for (auto i = 0; i < ambisonicNumChannels; ++i)
  {
    wetOutChannels[i] = &wetOutData[i * m_framesize];
  }
  wetOutBuffer = IPLAudioBuffer{ ambisonic, static_cast<IPLint32>(m_framesize), nullptr, wetOutChannels.data() };

  wetBinauralOutData.resize(m_framesize * stereoDeint.numSpeakers);
  wetBinauralOutChannels.resize(stereoDeint.numSpeakers);
  for (auto i = 0; i < stereoDeint.numSpeakers; ++i)
  {
    wetBinauralOutChannels[i] = &wetBinauralOutData[i * m_framesize];
  }
  wetBinauralOutBuffer = IPLAudioBuffer{ stereoDeint, static_cast<IPLint32>(m_framesize), nullptr, wetBinauralOutChannels.data() };

  preOutBufferData.resize(m_framesize * get_channel_quantity(alure::ChannelConfig::Stereo));
  preOutBufferDataChannels.resize(get_channel_quantity(alure::ChannelConfig::Stereo));
  for (auto i = 0; i < stereoDeint.numSpeakers; ++i)
  {
    preOutBufferDataChannels[i] = &preOutBufferData[i * m_framesize];
  }
  preOutBuffer = IPLAudioBuffer{ stereoDeint, static_cast<int32_t>(m_framesize), nullptr, preOutBufferDataChannels.data() };

  finalOutBuffer = IPLAudioBuffer{ stereo, static_cast<int32_t>(m_framesize) , nullptr };
}

//not done
//SteamAudioDecoder::~SteamAudioDecoder() {}

ALuint SteamAudioDecoder::read(ALvoid *ptr, ALuint count) noexcept
{
  if (count != m_framesize)
  {
    return 0;
  }

  inBuffer.interleavedBuffer = m_audioData->data.data() + m_samplesplayed;
  finalOutBuffer.interleavedBuffer = static_cast<float*>(ptr);

  auto numframes = static_cast<int>(m_audioData->data.size() / m_framesize / m_channelconfig);
  if (m_setframesplayed < numframes)
  {
    std::cout << "                                            \r";
    if (m_source.position.z < 0)
    {
      std::cout << "Front";
    }
    else
    {
      std::cout << "Back";
    }

    if (m_source.position.x > 0)
    {
      std::cout << " Right";
    }
    else
    {
      std::cout << " Left";
    }

    IPLDirectSoundEffectOptions opts{};
    opts.applyAirAbsorption = IPL_TRUE;
    opts.applyDistanceAttenuation = IPL_TRUE;
    opts.applyDirectivity = IPL_FALSE;
    opts.directOcclusionMode = IPL_DIRECTOCCLUSION_NONE;

    m_conv_effect->SetDryAudio(m_source, inBuffer);
    m_conv_effect->GetWetAudio(m_listenerPosition, m_listenerAhead, m_listenerUp, wetOutBuffer);

    auto soundPath = m_env->GetDirectSoundPath(m_listenerPosition, m_listenerAhead, m_listenerUp, m_source, m_radius, m_occlusionMode, m_occlusionMethod);
    m_direct_effect->Apply(inBuffer, soundPath, opts, directOutBuffer);
    m_binaural_effect->Apply(directOutBuffer, soundPath.direction, IPL_HRTFINTERPOLATION_BILINEAR, binauralOutBuffer);

    std::array<IPLAudioBuffer, 2> buffers{ wetBinauralOutBuffer, binauralOutBuffer };
    iplMixAudioBuffers(2, buffers.data(), preOutBuffer);

    m_samplesplayed += m_framesize * m_channelconfig;
    ++m_setframesplayed;

    switch (m_sample_type)
    {
    case alure::SampleType::Float32:
    {
      iplInterleaveAudioBuffer(preOutBuffer, finalOutBuffer);
      break;
    }
    case alure::SampleType::Int16:
    {
      std::array<float, 1024 * 4> convArray{ 0 };
      IPLAudioBuffer conversion{ stereo, m_framesize, convArray.data() };

      iplInterleaveAudioBuffer(preOutBuffer, conversion);

      auto pointer = static_cast<int16_t*>(ptr);

      for (size_t i = 0, m = m_framesize * m_channelconfig; i < m; ++i)
      {
        convArray[i] *= std::numeric_limits<short>::max();
      }

      for (size_t i = 0, m = m_framesize * m_channelconfig; i < m; ++i)
      {
        if (convArray[i] > std::numeric_limits<short>::max())
        {
          pointer[i] = std::numeric_limits<short>::max();
        }
        else if (convArray[i] < std::numeric_limits<short>::min())
        {
          pointer[i] = std::numeric_limits<short>::min();
        }
        else
        {
          pointer[i] = static_cast<short>(convArray[i]);
        }
      }

      break;
    }
    }

    return count;
  }
  else
  {
    m_finished = true;
    return 0;
  }
}
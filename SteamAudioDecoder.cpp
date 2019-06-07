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

  m_direct_effect = DirectEffect(envRenderer, get_channel_format(decodedResampledAudio->channels), mono);
  m_binaural_effect = BinauralEffect(renderer, mono, stereo);
  m_conv_effect = ConvolutionEffect(envRenderer, IPLBakedDataIdentifier {10, IPLBakedDataType::IPL_BAKEDDATATYPE_REVERB}, IPLSimulationType::IPL_SIMTYPE_REALTIME, get_channel_format(decodedResampledAudio->channels), stereo);

  directOut.resize(m_framesize);
  directOutBuffer = IPLAudioBuffer{ mono, static_cast<IPLint32>(m_framesize), directOut.data() };

  binauralOut.resize(m_framesize * stereo.numSpeakers);
  binauralOutBuffer = IPLAudioBuffer{ stereo, static_cast<IPLint32>(m_framesize), binauralOut.data() };

  wetOut.resize(m_framesize * stereo.numSpeakers);
  wetOutBuffer = IPLAudioBuffer{ stereo, static_cast<IPLint32>(m_framesize), wetOut.data() };

  finalConversionBuffer.resize(m_framesize * get_channel_quantity(alure::ChannelConfig::Stereo));
}

//not done
//SteamAudioDecoder::~SteamAudioDecoder() {}

ALuint SteamAudioDecoder::read(ALvoid *ptr, ALuint count) noexcept
{
  if (count != m_framesize)
  {
    return 0;
  }

  IPLAudioBuffer inbuffer{ get_channel_format(m_audioData->channels), m_framesize, m_audioData->data.data() + m_samplesplayed };

  IPLAudioBuffer finalOutbuffer{ stereo, m_framesize, static_cast<float*>(ptr) };
  if (m_sample_type != alure::SampleType::Float32)
  {
    finalOutbuffer.interleavedBuffer = finalConversionBuffer.data();
  }

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

    m_conv_effect.SetDryAudio(m_source, inbuffer);

    std::vector<float> wetData(m_framesize * m_channelconfig);
    IPLAudioBuffer wetOutbuffer{ stereo, m_framesize, wetData.data() };
    m_conv_effect.GetWetAudio(m_listenerPosition, m_listenerAhead, m_listenerUp, wetOutbuffer);

    auto soundPath = m_env->GetDirectSoundPath(m_listenerPosition, m_listenerAhead, m_listenerUp, m_source, m_radius, m_occlusionMode, m_occlusionMethod);

    m_direct_effect.Apply(inbuffer, soundPath, opts, directOutBuffer);

    m_binaural_effect.Apply(directOutBuffer, soundPath.direction, IPL_HRTFINTERPOLATION_BILINEAR, binauralOutBuffer);

    std::array<IPLAudioBuffer, 2> buffers{wetOutBuffer, binauralOutBuffer};
    iplMixAudioBuffers(2, buffers.data(), finalOutbuffer);

    m_samplesplayed += m_framesize * m_channelconfig;
    ++m_setframesplayed;

    switch (m_sample_type)
    {
    case alure::SampleType::Int16:
    {
      auto pointer = static_cast<int16_t*>(ptr);

      for (size_t i = 0, m = finalConversionBuffer.size(); i < m; ++i)
      {
        finalConversionBuffer[i] *= std::numeric_limits<short>::max();
      }

      for (size_t i = 0, m = finalConversionBuffer.size(); i < m; ++i)
      {
        if (finalConversionBuffer[i] > std::numeric_limits<short>::max())
        {
          pointer[i] = std::numeric_limits<short>::max();
        }
        else if (finalConversionBuffer[i] < std::numeric_limits<short>::min())
        {
          pointer[i] = std::numeric_limits<short>::min();
        }
        else
        {
          pointer[i] = static_cast<short>(finalConversionBuffer[i]);
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
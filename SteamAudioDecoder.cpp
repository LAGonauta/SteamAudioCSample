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

SteamAudioDecoder::SteamAudioDecoder(std::shared_ptr<BinauralRenderer> renderer, std::shared_ptr<EnvironmentalRenderer> envRenderer, const size_t& frameSize, std::shared_ptr<AudioBuffer> decodedResampledAudio, alure::SampleType forcedType)
  : m_sample_type(forcedType), m_audioData(decodedResampledAudio), m_framesize(frameSize)
{
  m_channelconfig = get_channel_quantity(m_audioData->channels);

  mono.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
  mono.channelLayout = IPL_CHANNELLAYOUT_MONO;
  mono.channelOrder = IPL_CHANNELORDER_INTERLEAVED;

  stereo.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
  stereo.channelLayout = IPL_CHANNELLAYOUT_STEREO;
  stereo.channelOrder = IPL_CHANNELORDER_INTERLEAVED;

  m_effect = BinauralEffect(renderer, get_channel_format(decodedResampledAudio->channels), stereo);
  m_direct_effect = DirectEffect(envRenderer, get_channel_format(decodedResampledAudio->channels), stereo);
}

//not done
SteamAudioDecoder::~SteamAudioDecoder() {}

ALuint SteamAudioDecoder::read(ALvoid *ptr, ALuint count) noexcept
{
  if (count != m_framesize)
  {
    return 0;
  }

  IPLAudioBuffer inbuffer{ get_channel_format(m_audioData->channels), m_framesize, m_audioData->data.data() + m_samplesplayed };

  std::vector<float> directOut(m_framesize * get_channel_quantity(m_audioData->channels));

  IPLAudioBuffer directOutBuffer{ get_channel_format(m_audioData->channels), m_framesize, directOut.data() };

  IPLAudioBuffer outbuffer{ stereo, m_framesize, static_cast<float*>(ptr) };
  std::vector<float> outBuffer;
  if (m_sample_type != alure::SampleType::Float32)
  {
    outBuffer.resize(m_framesize * get_channel_quantity(m_audioData->channels));
    outbuffer.interleavedBuffer = outBuffer.data();
  }

  auto numframes = static_cast<int>(m_audioData->data.size() / m_framesize / m_channelconfig);
  if (m_setframesplayed < numframes)
  {
    std::cout << "                                            \r";
    if (m_sound_path.direction.z < 0)
    {
      std::cout << "Front";
    }
    else
    {
      std::cout << "Back";
    }

    if (m_sound_path.direction.x > 0)
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

    m_direct_effect.Apply(inbuffer, m_sound_path, opts, directOutBuffer);

    m_effect.Apply(directOutBuffer, m_sound_path.direction, IPL_HRTFINTERPOLATION_BILINEAR, outbuffer);

    m_samplesplayed += m_framesize * m_channelconfig;
    ++m_setframesplayed;

    switch (m_sample_type)
    {
    case alure::SampleType::Int16:
    {
      auto pointer = static_cast<int16_t*>(ptr);

      for (int i = 0, m = outBuffer.size(); i < m; ++i)
      {
        outBuffer[i] *= std::numeric_limits<short>::max();
      }

      for (int i = 0, m = outBuffer.size(); i < m; ++i)
      {
        if (outBuffer[i] > std::numeric_limits<short>::max())
        {
          pointer[i] = std::numeric_limits<short>::max();
        }
        else if (outBuffer[i] < std::numeric_limits<short>::min())
        {
          pointer[i] = std::numeric_limits<short>::min();
        }
        else
        {
          pointer[i] = static_cast<short>(outBuffer[i]);
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
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

SteamAudioDecoder::SteamAudioDecoder(std::shared_ptr<BinauralRenderer> renderer, const size_t& frameSize, std::shared_ptr<AudioBuffer> decodedResampledAudio)
{
  m_audioData = decodedResampledAudio;
  m_framesize = frameSize;
  m_channelconfig = get_channel_quantity(m_audioData->channels);

  mono.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
  mono.channelLayout = IPL_CHANNELLAYOUT_MONO;
  mono.channelOrder = IPL_CHANNELORDER_INTERLEAVED;

  stereo.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
  stereo.channelLayout = IPL_CHANNELLAYOUT_STEREO;
  stereo.channelOrder = IPL_CHANNELORDER_INTERLEAVED;

  std::swap(m_effect, BinauralEffect(renderer, get_channel_format(decodedResampledAudio->channels), stereo));
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

  IPLAudioBuffer outbuffer{ stereo, m_framesize, static_cast<float*>(ptr) };

  auto numframes = static_cast<int>(m_audioData->data.size() / m_framesize / m_channelconfig);
  if (m_setframesplayed < numframes)
  {
    std::cout << "                                            \r";
    if (m_direction.z < 0)
    {
      std::cout << "Front";
    }
    else
    {
      std::cout << "Back";
    }

    if (m_direction.x > 0)
    {
      std::cout << " Right";
    }
    else
    {
      std::cout << " Left";
    }

    m_effect.Apply(inbuffer, m_direction, IPL_HRTFINTERPOLATION_BILINEAR, outbuffer);
    m_samplesplayed += m_framesize * m_channelconfig;
    ++m_setframesplayed;
    return count;
  }
  else
  {
    return 0;
  }
}
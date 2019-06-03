#include <unordered_map>
#include <numeric>

#include "AudioSource.hpp"
#include "Helpers.hpp"

static std::unordered_map<std::string, std::shared_ptr<AudioBuffer>> global_cache;
static std::mutex global_mutex;

AudioSource::AudioSource(std::shared_ptr<BinauralRenderer> renderer, std::shared_ptr<EnvironmentalRenderer> envRenderer, alure::Context al_context) : m_al_context(al_context), m_renderer(renderer), m_env_renderer(envRenderer)
{
  m_al_source = al_context.createSource();
}

AudioSource::~AudioSource()
{
  m_al_source.destroy();
}

void AudioSource::Play(std::string fileName)
{
  std::shared_ptr<AudioBuffer>* m_buffer;
  {
    std::lock_guard<std::mutex> lock(global_mutex);
    m_buffer = &global_cache.emplace(fileName, std::make_shared<AudioBuffer>()).first->second;
  }

  alure::SampleType type = alure::SampleType::Float32;
  if (m_al_context.isSupported(alure::ChannelConfig::Stereo, alure::SampleType::Float32) == false)
  {
    type = alure::SampleType::Int16;
  }

  if ((*m_buffer) == nullptr || (*m_buffer)->data.size() == 0)
  {
    (*m_buffer) = get_audio(fileName, m_al_context, 48000);
  }

  m_decoder = std::make_shared<SteamAudioDecoder>(m_renderer, m_env_renderer, m_renderer->GetFramesize(), *m_buffer, type);
  m_al_source.play(m_decoder, m_renderer->GetFramesize(), 4);
}

bool AudioSource::isPlaying()
{
  return m_al_source.isPlaying();
}

void AudioSource::Stop()
{
  m_al_source.stop();
  m_decoder.reset();
  // m_decoder_vector.clear();
}

void AudioSource::SetPosition(const IPLSource& source, const std::shared_ptr<Environment> environment, const IPLVector3& listenerPosition, const IPLVector3& listenerAhead, const IPLVector3& listenerUp)
{
  IPLDirectSoundPath parameters = environment->GetDirectSoundPath(listenerPosition, listenerAhead, listenerUp, source, Radius, OcclusionMode, OcclusionMethod);

  //m_decoder_vector.remove_if([](std::shared_ptr<SteamAudioDecoder> dec) { return dec->isFinished(); });
  //std::for_each(m_decoder_vector.begin(), m_decoder_vector.end(), [&parameters](std::shared_ptr<SteamAudioDecoder> dec) { dec->setPosition(parameters); });
  m_decoder->setPosition(parameters);
}

float AudioSource::GetMean(const float* data, const size_t size)
{
  return std::accumulate(data, data + size, 0.0f) / size;
}
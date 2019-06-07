#include "DirectEffect.hpp"

DirectEffect::DirectEffect(std::shared_ptr<EnvironmentalRenderer> environmentalRenderer, const IPLAudioFormat& inputFormat, const IPLAudioFormat& outputFormat) : m_environmental_renderer(environmentalRenderer)
{
  auto error = iplCreateDirectSoundEffect(m_environmental_renderer->GetHandle(), inputFormat, outputFormat, &m_effect);
  if (error)
  {
    throw std::exception("Error creating DirectEffect: " + error);
  }
}

DirectEffect::~DirectEffect()
{
  if (m_effect)
  {
    iplDestroyDirectSoundEffect(&m_effect);
  }
}

void DirectEffect::Flush()
{
  if (m_effect)
  {
    iplFlushDirectSoundEffect(m_effect);
  }
}

void DirectEffect::Apply(const IPLAudioBuffer& inputAudio, const IPLDirectSoundPath& soundPath, const IPLDirectSoundEffectOptions& options, IPLAudioBuffer& outputAudio)
{
  if (m_effect)
  {
    iplApplyDirectSoundEffect(m_effect, inputAudio, soundPath, options, outputAudio);
  }
}

DirectEffect::DirectEffect(DirectEffect&& other) noexcept
{
  std::swap(m_effect, other.m_effect);
  std::swap(m_environmental_renderer, other.m_environmental_renderer);
  std::swap(m_inputFormat, other.m_inputFormat);
  std::swap(m_outputFormat, other.m_outputFormat);
}

DirectEffect& DirectEffect::operator=(DirectEffect&& other) noexcept
{
  std::swap(m_effect, other.m_effect);
  std::swap(m_environmental_renderer, other.m_environmental_renderer);
  std::swap(m_inputFormat, other.m_inputFormat);
  std::swap(m_outputFormat, other.m_outputFormat);
  return *this;
}
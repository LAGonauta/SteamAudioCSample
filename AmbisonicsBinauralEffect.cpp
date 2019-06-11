#include <exception>

#include "AmbisonicsBinauralEffect.hpp"

AmbisonicsBinauralEffect::AmbisonicsBinauralEffect(std::shared_ptr<BinauralRenderer> binauralRenderer, const IPLAudioFormat& inputFormat, const IPLAudioFormat& outputFormat)
  : m_binaural_renderer(binauralRenderer), m_inputFormat(inputFormat), m_outputFormat(outputFormat)
{
  auto error = iplCreateAmbisonicsBinauralEffect(m_binaural_renderer->GetHandle(), m_inputFormat, m_outputFormat, &m_effect);
  if (error)
  {
    throw std::exception("Error creating AmbisonicsBinauralEffect: " + error);
  }
}

AmbisonicsBinauralEffect::~AmbisonicsBinauralEffect()
{
  if (m_effect)
  {
    iplDestroyAmbisonicsBinauralEffect(&m_effect);
  }
}

void AmbisonicsBinauralEffect::Flush()
{
  if (m_effect)
  {
    iplFlushAmbisonicsBinauralEffect(m_effect);
  }
}

void AmbisonicsBinauralEffect::Apply(const IPLAudioBuffer& inputAudio, IPLAudioBuffer& outputAudio)
{
  if (m_effect)
  {
    iplApplyAmbisonicsBinauralEffect(m_effect, m_binaural_renderer->GetHandle(), inputAudio, outputAudio);
  }
}

AmbisonicsBinauralEffect::AmbisonicsBinauralEffect(AmbisonicsBinauralEffect&& other) noexcept
{
  std::swap(m_effect, other.m_effect);
  std::swap(m_binaural_renderer, other.m_binaural_renderer);
  std::swap(m_inputFormat, other.m_inputFormat);
  std::swap(m_outputFormat, other.m_outputFormat);
}

AmbisonicsBinauralEffect& AmbisonicsBinauralEffect::operator=(AmbisonicsBinauralEffect&& other) noexcept
{
  std::swap(m_effect, other.m_effect);
  std::swap(m_binaural_renderer, other.m_binaural_renderer);
  std::swap(m_inputFormat, other.m_inputFormat);
  std::swap(m_outputFormat, other.m_outputFormat);
  return *this;
}
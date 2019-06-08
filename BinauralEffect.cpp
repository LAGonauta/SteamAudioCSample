#include <exception>

#include "BinauralEffect.hpp"

BinauralEffect::BinauralEffect(std::shared_ptr<BinauralRenderer> binauralRenderer, const IPLAudioFormat& inputFormat, const IPLAudioFormat& outputFormat)
  : m_binaural_renderer(binauralRenderer), m_inputFormat(inputFormat), m_outputFormat(outputFormat)
{
  auto error = iplCreateBinauralEffect(m_binaural_renderer->GetHandle(), m_inputFormat, m_outputFormat, &m_effect);
  if (error)
  {
    throw std::exception("Error creating BinauralEffect: " + error);
  }
}

BinauralEffect::~BinauralEffect()
{
  if (m_effect)
  {
    iplDestroyBinauralEffect(&m_effect);
  }
}

void BinauralEffect::Flush()
{
  if (m_effect)
  {
    iplFlushBinauralEffect(m_effect);
  }
}

void BinauralEffect::Apply(const IPLAudioBuffer& inputAudio, const IPLVector3& direction, IPLHrtfInterpolation interpolation, IPLAudioBuffer& outputAudio)
{
  if (m_effect)
  {
    iplApplyBinauralEffect(m_effect, m_binaural_renderer->GetHandle(), inputAudio, direction, interpolation, outputAudio);
  }
}

void BinauralEffect::Apply(const IPLAudioBuffer& inputAudio, const IPLVector3& direction, IPLHrtfInterpolation interpolation, IPLAudioBuffer& outputAudio, IPLfloat32& leftDelay, IPLfloat32& rightDelay)
{
  if (m_effect)
  {
    iplApplyBinauralEffectWithParameters(m_effect, m_binaural_renderer->GetHandle(), inputAudio, direction, interpolation, outputAudio, &leftDelay, &rightDelay);
  }
}

BinauralEffect::BinauralEffect(BinauralEffect&& other) noexcept
{
  std::swap(m_effect, other.m_effect);
  std::swap(m_binaural_renderer, other.m_binaural_renderer);
  std::swap(m_inputFormat, other.m_inputFormat);
  std::swap(m_outputFormat, other.m_outputFormat);
}

BinauralEffect& BinauralEffect::operator=(BinauralEffect&& other) noexcept
{
  std::swap(m_effect, other.m_effect);
  std::swap(m_binaural_renderer, other.m_binaural_renderer);
  std::swap(m_inputFormat, other.m_inputFormat);
  std::swap(m_outputFormat, other.m_outputFormat);
  return *this;
}
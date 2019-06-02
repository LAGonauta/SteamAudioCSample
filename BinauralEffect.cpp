#include <exception>

#include "BinauralEffect.hpp"

BinauralEffect::BinauralEffect(std::shared_ptr<BinauralRenderer> binauralRenderer, const IPLAudioFormat& inputFormat, const IPLAudioFormat& outputFormat)
{
  m_binaural_renderer = binauralRenderer;
  auto error = iplCreateBinauralEffect(m_binaural_renderer->GetHandle(), inputFormat, outputFormat, &m_effect);
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
  iplFlushBinauralEffect(m_effect);
}

void BinauralEffect::Apply(const IPLAudioBuffer& inputAudio, const IPLVector3& direction, IPLHrtfInterpolation interpolation, IPLAudioBuffer& outputAudio)
{
  iplApplyBinauralEffect(m_effect, m_binaural_renderer->GetHandle(), inputAudio, direction, interpolation, outputAudio);
}

void BinauralEffect::Apply(const IPLAudioBuffer& inputAudio, const IPLVector3& direction, IPLHrtfInterpolation interpolation, IPLAudioBuffer& outputAudio, IPLfloat32& leftDelay, IPLfloat32& rightDelay)
{
  iplApplyBinauralEffectWithParameters(m_effect, m_binaural_renderer->GetHandle(), inputAudio, direction, interpolation, outputAudio, &leftDelay, &rightDelay);
}
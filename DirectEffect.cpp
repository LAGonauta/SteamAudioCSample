#include "DirectEffect.hpp"

DirectEffect::DirectEffect(std::shared_ptr<EnvironmentalRenderer> environmentalRenderer, const IPLAudioFormat& inputFormat, const IPLAudioFormat& outputFormat) : m_environmental_renderer(environmentalRenderer)
{
  IPLhandle effect{ nullptr };
  auto error = iplCreateDirectSoundEffect(m_environmental_renderer->GetHandle(), inputFormat, outputFormat, &effect);
  if (error)
  {
    throw std::exception("Error creating BinauralEffect: " + error);
  }
  m_effect = std::make_shared<IPLhandle>(effect);
}

DirectEffect::~DirectEffect()
{
  if (m_effect.unique())
  {
    iplDestroyDirectSoundEffect(&(*m_effect));
  }
}

void DirectEffect::Flush()
{
  iplFlushDirectSoundEffect(*m_effect);
}

void DirectEffect::Apply(const IPLAudioBuffer& inputAudio, const IPLDirectSoundPath& soundPath, const IPLDirectSoundEffectOptions& options, IPLAudioBuffer& outputAudio)
{
  iplApplyDirectSoundEffect(*m_effect, inputAudio, soundPath, options, outputAudio);
}
#include <exception>

#include "ConvolutionEffect.hpp"

ConvolutionEffect::ConvolutionEffect(std::shared_ptr<EnvironmentalRenderer> envRenderer,
  const IPLBakedDataIdentifier& identifier,
  const IPLSimulationType& simulationType,
  const IPLAudioFormat& inputFormat,
  const IPLAudioFormat& outputFormat)
  : m_environmental_renderer(envRenderer), m_baked_identifier(identifier), m_simulation_type(simulationType), m_inputFormat(inputFormat), m_outputFormat(outputFormat)
{
  auto error = iplCreateConvolutionEffect(envRenderer->GetHandle(), m_baked_identifier, m_simulation_type, m_inputFormat, m_outputFormat, &m_effect);
  if (error)
  {
    throw std::exception("Error creating ConvolutionEffect: " + error);
  }
}

ConvolutionEffect::~ConvolutionEffect()
{
  if (m_effect)
  {
    iplDestroyConvolutionEffect(&m_effect);
  }
}

void ConvolutionEffect::Flush()
{
  if (m_effect)
  {
    iplFlushConvolutionEffect(m_effect);
  }
}

void ConvolutionEffect::SetDryAudio(const IPLSource& source, const IPLAudioBuffer& dryAudio)
{
  if (m_effect)
  {
    iplSetDryAudioForConvolutionEffect(m_effect, source, dryAudio);
  }
}

void ConvolutionEffect::GetWetAudio(const IPLVector3& listenerPosition, const IPLVector3& listenerAhead, const IPLVector3& listenerUp, IPLAudioBuffer& wetAudio)
{
  if (m_effect)
  {
    iplGetWetAudioForConvolutionEffect(m_effect, listenerPosition, listenerAhead, listenerUp, wetAudio);
  }
}

void ConvolutionEffect::SetIdentifier(const IPLBakedDataIdentifier& identifier)
{
  if (m_effect)
  {
    iplSetConvolutionEffectIdentifier(m_effect, identifier);
  }
}

ConvolutionEffect::ConvolutionEffect(ConvolutionEffect&& other) noexcept
{
  std::swap(m_effect, other.m_effect);
  std::swap(m_environmental_renderer, other.m_environmental_renderer);
  std::swap(m_simulation_type, other.m_simulation_type);
  std::swap(m_baked_identifier, other.m_baked_identifier);
  std::swap(m_inputFormat, other.m_inputFormat);
  std::swap(m_outputFormat, other.m_outputFormat);
}

ConvolutionEffect& ConvolutionEffect::operator=(ConvolutionEffect&& other) noexcept
{
  std::swap(m_effect, other.m_effect);
  std::swap(m_environmental_renderer, other.m_environmental_renderer);
  std::swap(m_simulation_type, other.m_simulation_type);
  std::swap(m_baked_identifier, other.m_baked_identifier);
  std::swap(m_inputFormat, other.m_inputFormat);
  std::swap(m_outputFormat, other.m_outputFormat);
  return *this;
}
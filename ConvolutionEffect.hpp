#pragma once

#include "common.h"
#include "EnvironmentalRenderer.hpp"

class ConvolutionEffect final
{
private:
  IPLhandle m_effect{ nullptr };
  IPLBakedDataIdentifier m_baked_identifier{};
  IPLSimulationType m_simulation_type{};
  IPLAudioFormat m_inputFormat{};
  IPLAudioFormat m_outputFormat{};
  std::shared_ptr<EnvironmentalRenderer> m_environmental_renderer{ nullptr };

public:
  DLL_PUBLIC ConvolutionEffect(const ConvolutionEffect& other) = delete;
  DLL_PUBLIC ConvolutionEffect& ConvolutionEffect::operator=(const ConvolutionEffect& other) = delete;
  DLL_PUBLIC ConvolutionEffect(ConvolutionEffect&& other) noexcept;
  DLL_PUBLIC ConvolutionEffect& operator=(ConvolutionEffect&& other) noexcept;

  DLL_PUBLIC ConvolutionEffect(std::shared_ptr<EnvironmentalRenderer> environmentalRenderer, const IPLBakedDataIdentifier& identifier, const IPLSimulationType& simulationType, const IPLAudioFormat& inputFormat, const IPLAudioFormat& outputFormat);
  DLL_PUBLIC ~ConvolutionEffect();

  DLL_PUBLIC void Flush();

  DLL_PUBLIC void SetDryAudio(const IPLSource& source, const IPLAudioBuffer& dryAudio);
  DLL_PUBLIC void GetWetAudio(const IPLVector3& listenerPosition, const IPLVector3& listenerAhead, const IPLVector3& listenerUp, IPLAudioBuffer& wetAudio);
  DLL_PUBLIC void SetIdentifier(const IPLBakedDataIdentifier& identifier);
};
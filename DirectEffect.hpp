#pragma once

#include "common.h"
#include "EnvironmentalRenderer.hpp"

class DirectEffect final
{
private:
  std::shared_ptr<IPLhandle> m_effect{ nullptr };
  std::shared_ptr<EnvironmentalRenderer> m_environmental_renderer{ nullptr };

public:
  DLL_PUBLIC DirectEffect() = default;
  DLL_PUBLIC DirectEffect(std::shared_ptr<EnvironmentalRenderer> environmentalRenderer, const IPLAudioFormat& inputFormat, const IPLAudioFormat& outputFormat);
  DLL_PUBLIC ~DirectEffect();

  DLL_PUBLIC void Flush();

  DLL_PUBLIC void Apply(const IPLAudioBuffer& inputAudio, const IPLDirectSoundPath& soundPath, const IPLDirectSoundEffectOptions& options, IPLAudioBuffer& outputAudio);
};
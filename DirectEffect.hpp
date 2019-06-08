#pragma once

#include "common.h"
#include "EnvironmentalRenderer.hpp"

class DirectEffect final
{
private:
  IPLhandle m_effect{ nullptr };
  IPLAudioFormat m_inputFormat{};
  IPLAudioFormat m_outputFormat{};
  std::shared_ptr<EnvironmentalRenderer> m_environmental_renderer{ nullptr };

public:
  DLL_PUBLIC DirectEffect(const DirectEffect& other) = delete;
  DLL_PUBLIC DirectEffect& DirectEffect::operator=(const DirectEffect& other) = delete;
  DLL_PUBLIC DirectEffect(DirectEffect&& other) noexcept;
  DLL_PUBLIC DirectEffect& operator=(DirectEffect&& other) noexcept;

  DLL_PUBLIC DirectEffect(std::shared_ptr<EnvironmentalRenderer> environmentalRenderer, const IPLAudioFormat& inputFormat, const IPLAudioFormat& outputFormat);
  DLL_PUBLIC ~DirectEffect();

  DLL_PUBLIC void Flush();

  DLL_PUBLIC void Apply(const IPLAudioBuffer& inputAudio, const IPLDirectSoundPath& soundPath, const IPLDirectSoundEffectOptions& options, IPLAudioBuffer& outputAudio);
};
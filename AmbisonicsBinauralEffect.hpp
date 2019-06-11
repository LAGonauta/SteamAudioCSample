#pragma once

#include "common.h"
#include "BinauralRenderer.hpp"

class AmbisonicsBinauralEffect final
{
private:
  IPLhandle m_effect{ nullptr };
  IPLAudioFormat m_inputFormat{};
  IPLAudioFormat m_outputFormat{};
  std::shared_ptr<BinauralRenderer> m_binaural_renderer{ nullptr };

public:
  DLL_PUBLIC AmbisonicsBinauralEffect(const AmbisonicsBinauralEffect& other) = delete;
  DLL_PUBLIC AmbisonicsBinauralEffect& AmbisonicsBinauralEffect::operator=(const AmbisonicsBinauralEffect& other) = delete;
  DLL_PUBLIC AmbisonicsBinauralEffect(AmbisonicsBinauralEffect&& other) noexcept;
  DLL_PUBLIC AmbisonicsBinauralEffect& operator=(AmbisonicsBinauralEffect&& other) noexcept;

  DLL_PUBLIC AmbisonicsBinauralEffect(std::shared_ptr<BinauralRenderer> binauralRenderer, const IPLAudioFormat& inputFormat, const IPLAudioFormat& outputFormat);
  DLL_PUBLIC ~AmbisonicsBinauralEffect();

  DLL_PUBLIC void Flush();

  DLL_PUBLIC void Apply(const IPLAudioBuffer& inputAudio, IPLAudioBuffer& outputAudio);
};
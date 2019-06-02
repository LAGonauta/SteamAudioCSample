#pragma once

#include "common.h"
#include "BinauralRenderer.hpp"

class BinauralEffect final
{
private:
  IPLhandle m_effect{ nullptr };
  std::shared_ptr<BinauralRenderer> m_binaural_renderer{ nullptr };

public:
  DLL_PUBLIC BinauralEffect() = default;
  DLL_PUBLIC BinauralEffect(std::shared_ptr<BinauralRenderer> binauralRenderer, const IPLAudioFormat& inputFormat, const IPLAudioFormat& outputFormat);
  DLL_PUBLIC ~BinauralEffect();

  DLL_PUBLIC void Flush();

  DLL_PUBLIC void Apply(const IPLAudioBuffer& inputAudio, const IPLVector3& direction, IPLHrtfInterpolation interpolation, IPLAudioBuffer& outputAudio);

  DLL_PUBLIC void Apply(const IPLAudioBuffer& inputAudio, const IPLVector3& direction, IPLHrtfInterpolation interpolation, IPLAudioBuffer& outputAudio, IPLfloat32& leftDelay, IPLfloat32& rightDelay);
};
#pragma once

#include "common.h"
#include "Context.hpp"

class BinauralRenderer final
{
private:
  friend class BinauralEffect;
  std::shared_ptr<IPLhandle> m_binaural_renderer{ nullptr };
  int32_t m_framesize = 0;

  std::shared_ptr<Context> m_context{ nullptr };

  IPLhandle GetHandle();

public:
  DLL_PUBLIC BinauralRenderer(std::shared_ptr<Context> context, const IPLRenderingSettings& renderingSettings, const IPLHrtfParams& params);
  DLL_PUBLIC ~BinauralRenderer();
  DLL_PUBLIC int32_t GetFramesize();
};
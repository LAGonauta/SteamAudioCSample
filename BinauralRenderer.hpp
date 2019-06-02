#pragma once

#include "common.h"
#include "Context.hpp"

class BinauralRenderer final
{
private:
  friend class BinauralEffect;
  IPLhandle m_binaural_renderer{ nullptr };

  std::shared_ptr<Context> m_context{ nullptr };

  IPLhandle GetHandle();

public:
  DLL_PUBLIC BinauralRenderer(std::shared_ptr<Context> context, const IPLRenderingSettings& renderingSettings, const IPLHrtfParams& params);
  DLL_PUBLIC ~BinauralRenderer();
};
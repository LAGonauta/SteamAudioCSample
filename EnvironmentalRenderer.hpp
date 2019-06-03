#pragma once

#include "common.h"
#include "Context.hpp"
#include "Environment.hpp"

class EnvironmentalRenderer final
{
private:
  friend class DirectEffect;
  std::shared_ptr<IPLhandle> m_environmental_renderer{ nullptr };
  int32_t m_framesize = 0;

  std::shared_ptr<Context> m_context{ nullptr };
  std::shared_ptr<Environment> m_environment{ nullptr };

  IPLhandle GetHandle();

public:
  DLL_PUBLIC EnvironmentalRenderer(std::shared_ptr<Context> context, std::shared_ptr<Environment> env, const IPLRenderingSettings& renderingSettings, const  IPLAudioFormat& outputFormat);
  DLL_PUBLIC ~EnvironmentalRenderer();
  DLL_PUBLIC int32_t GetFramesize();
};
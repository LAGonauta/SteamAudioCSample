#pragma once

#include "common.h"
#include "Context.hpp"
#include "Scene.hpp"

class Environment final
{
private:
  friend class EnvironmentalRenderer;
  IPLhandle m_environment{ nullptr };

  std::shared_ptr<Context> m_context{ nullptr };
  std::shared_ptr<Scene> m_scene{ nullptr };

  IPLhandle GetHandle();

public:
  DLL_PUBLIC Environment() = delete;
  DLL_PUBLIC Environment(const Environment& other) = delete;
  DLL_PUBLIC Environment& operator=(const Environment& other) = delete;
  DLL_PUBLIC Environment(Environment&& other) noexcept;
  DLL_PUBLIC Environment& operator=(Environment&& other) noexcept;

  DLL_PUBLIC Environment(std::shared_ptr<Context> context, IPLhandle computeDevice, IPLSimulationSettings simulationSettings, std::shared_ptr<Scene> scene, IPLhandle probeManager);
  DLL_PUBLIC ~Environment();

  DLL_PUBLIC void SetNumBounces(IPLint32 numBounces) const;

  DLL_PUBLIC IPLDirectSoundPath GetDirectSoundPath(IPLVector3 listenerPosition,
    IPLVector3 listenerAhead, IPLVector3 listenerUp, IPLSource source, IPLfloat32 sourceRadius,
    IPLDirectOcclusionMode occlusionMode, IPLDirectOcclusionMethod occlusionMethod) const;
};
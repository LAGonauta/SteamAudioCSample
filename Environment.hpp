#pragma once

#include "common.h"
#include "Context.hpp"

class Environment final
{
private:
  friend class EnvironmentalRenderer;
  std::shared_ptr<IPLhandle> m_environment{ nullptr };

  std::shared_ptr<Context> m_context{ nullptr };

  IPLhandle GetHandle();

public:
  DLL_PUBLIC Environment(std::shared_ptr<Context> context, IPLhandle computeDevice, IPLSimulationSettings simulationSettings, IPLhandle scene, IPLhandle probeManager);
  DLL_PUBLIC ~Environment();

  DLL_PUBLIC void SetNumBounces(IPLint32 numBounces) const;

  DLL_PUBLIC IPLDirectSoundPath GetDirectSoundPath(IPLVector3 listenerPosition,
    IPLVector3 listenerAhead, IPLVector3 listenerUp, IPLSource source, IPLfloat32 sourceRadius,
    IPLDirectOcclusionMode occlusionMode, IPLDirectOcclusionMethod occlusionMethod) const;
};
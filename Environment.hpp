#pragma once

#include "common.h"
#include "Context.hpp"

class Environment final
{
private:
  IPLhandle m_environment{ nullptr };

  std::shared_ptr<Context> m_context{ nullptr };

  IPLhandle GetHandle();

public:
  DLL_PUBLIC Environment(std::shared_ptr<Context> context, const IPLhandle& computeDevice, const IPLSimulationSettings& simulationSettings, const IPLhandle& scene, const IPLhandle& probeManager);
  DLL_PUBLIC ~Environment();

  DLL_PUBLIC void SetNumBounces(IPLint32 numBounces) const;
};
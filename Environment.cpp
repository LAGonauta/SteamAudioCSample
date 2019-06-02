#pragma once

#include <exception>

#include "Environment.hpp"

Environment::Environment(std::shared_ptr<Context> context, const IPLhandle& computeDevice, const IPLSimulationSettings& simulationSettings, const IPLhandle& scene, const IPLhandle& probeManager)
{
  m_context = context;
  auto error = iplCreateEnvironment(context->GetHandle(), computeDevice, simulationSettings, scene, probeManager, &m_environment);
  if (error)
  {
    throw std::exception("Error creating evironment: " + error);
  }
}

Environment::~Environment()
{
  if (m_environment)
  {
    iplDestroyEnvironment(&m_environment);
  }
}

void Environment::SetNumBounces(IPLint32 numBounces) const
{
  iplSetNumBounces(m_environment, numBounces);
}

IPLhandle Environment::GetHandle()
{
  return m_environment;
}
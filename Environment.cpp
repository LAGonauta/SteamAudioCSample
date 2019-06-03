#pragma once

#include <exception>

#include "Environment.hpp"

Environment::Environment(std::shared_ptr<Context> context, IPLhandle computeDevice, IPLSimulationSettings simulationSettings, IPLhandle scene, IPLhandle probeManager) : m_context(context)
{
  IPLhandle environment{ nullptr };
  auto error = iplCreateEnvironment(context->GetHandle(), computeDevice, simulationSettings, scene, probeManager, &environment);
  if (error)
  {
    throw std::exception("Error creating evironment: " + error);
  }
  m_environment = std::make_shared<IPLhandle>(environment);
}

Environment::~Environment()
{
  if (m_environment.unique())
  {
    iplDestroyEnvironment(&(*m_environment));
  }
}

void Environment::SetNumBounces(IPLint32 numBounces) const
{
  iplSetNumBounces(*m_environment, numBounces);
}

IPLDirectSoundPath Environment::GetDirectSoundPath(IPLVector3 listenerPosition,
  IPLVector3 listenerAhead, IPLVector3 listenerUp, IPLSource source, IPLfloat32 sourceRadius,
  IPLDirectOcclusionMode occlusionMode, IPLDirectOcclusionMethod occlusionMethod) const
{
  return iplGetDirectSoundPath(*m_environment, listenerPosition, listenerAhead, listenerUp, source, sourceRadius, occlusionMode, occlusionMethod);
}

IPLhandle Environment::GetHandle()
{
  return *m_environment;
}
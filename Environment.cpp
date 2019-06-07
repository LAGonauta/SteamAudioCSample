#pragma once

#include <exception>

#include "Environment.hpp"

Environment::Environment(std::shared_ptr<Context> context,
  IPLhandle computeDevice,
  IPLSimulationSettings simulationSettings,
  std::shared_ptr<Scene> scene,
  IPLhandle probeManager)
  : m_context(context), m_scene(scene)
{
  auto error = iplCreateEnvironment(context->GetHandle(), computeDevice, simulationSettings, scene->GetHandle(), probeManager, &m_environment);
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

IPLDirectSoundPath Environment::GetDirectSoundPath(IPLVector3 listenerPosition,
  IPLVector3 listenerAhead, IPLVector3 listenerUp, IPLSource source, IPLfloat32 sourceRadius,
  IPLDirectOcclusionMode occlusionMode, IPLDirectOcclusionMethod occlusionMethod) const
{
  return iplGetDirectSoundPath(m_environment, listenerPosition, listenerAhead, listenerUp, source, sourceRadius, occlusionMode, occlusionMethod);
}

IPLhandle Environment::GetHandle()
{
  return m_environment;
}

Environment::Environment(Environment&& other) noexcept
{
  std::swap(m_environment, other.m_environment);
  std::swap(m_scene, other.m_scene);
  std::swap(m_context, other.m_context);
}

Environment& Environment::operator=(Environment&& other) noexcept
{
  std::swap(m_environment, other.m_environment);
  std::swap(m_scene, other.m_scene);
  std::swap(m_context, other.m_context);
  return *this;
}
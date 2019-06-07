#pragma once

#include "EnvironmentalRenderer.hpp"

EnvironmentalRenderer::EnvironmentalRenderer(std::shared_ptr<Context> context, std::shared_ptr<Environment> env, const IPLRenderingSettings& renderingSettings, const  IPLAudioFormat& outputFormat)
  : m_context(context), m_environment(env), m_framesize(renderingSettings.frameSize)
{
  auto error = iplCreateEnvironmentalRenderer(context->GetHandle(), env->GetHandle(), renderingSettings, outputFormat, nullptr, nullptr, &m_environmental_renderer);
  if (error)
  {
    throw std::exception("Error creating binaural renderer: " + error);
  }
}
EnvironmentalRenderer::~EnvironmentalRenderer()
{
  if (m_environmental_renderer)
  {
    iplDestroyEnvironmentalRenderer(&m_environmental_renderer);
  }
}

int32_t EnvironmentalRenderer::GetFramesize()
{
  return m_framesize;
}

IPLhandle EnvironmentalRenderer::GetHandle()
{
  return m_environmental_renderer;
}

EnvironmentalRenderer::EnvironmentalRenderer(EnvironmentalRenderer&& other) noexcept
{
  std::swap(m_environment, other.m_environment);
  std::swap(m_environmental_renderer, other.m_environmental_renderer);
  std::swap(m_context, other.m_context);
  std::swap(m_framesize, other.m_framesize);
}

EnvironmentalRenderer& EnvironmentalRenderer::operator=(EnvironmentalRenderer&& other) noexcept
{
  std::swap(m_environment, other.m_environment);
  std::swap(m_environmental_renderer, other.m_environmental_renderer);
  std::swap(m_context, other.m_context);
  std::swap(m_framesize, other.m_framesize);
  return *this;
}
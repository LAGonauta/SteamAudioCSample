#pragma once

#include "EnvironmentalRenderer.hpp"

EnvironmentalRenderer::EnvironmentalRenderer(std::shared_ptr<Context> context, std::shared_ptr<Environment> env, const IPLRenderingSettings& renderingSettings, const  IPLAudioFormat& outputFormat)
  : m_context(context), m_environment(env)
{
  IPLhandle renderer{ nullptr };
  auto error = iplCreateEnvironmentalRenderer(context->GetHandle(), env->GetHandle(), renderingSettings, outputFormat, nullptr, nullptr, &renderer);
  m_framesize = renderingSettings.frameSize;
  if (error)
  {
    throw std::exception("Error creating binaural renderer: " + error);
  }
  m_environmental_renderer = std::make_shared<IPLhandle>(renderer);
}
EnvironmentalRenderer::~EnvironmentalRenderer()
{
  if (m_environmental_renderer.unique())
  {
    iplDestroyEnvironmentalRenderer(&(*m_environmental_renderer));
  }
}

int32_t EnvironmentalRenderer::GetFramesize()
{
  return m_framesize;
}

IPLhandle EnvironmentalRenderer::GetHandle()
{
  return *m_environmental_renderer;
}
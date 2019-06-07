#include <exception>

#include "BinauralRenderer.hpp"

BinauralRenderer::BinauralRenderer(std::shared_ptr<Context> context, const IPLRenderingSettings& renderingSettings, const IPLHrtfParams& params) : m_context(context)
{
  auto error = iplCreateBinauralRenderer(context->GetHandle(), renderingSettings, params, &m_binaural_renderer);
  m_framesize = renderingSettings.frameSize;
  if (error)
  {
    throw std::exception("Error creating binaural renderer: " + error);
  }
}

BinauralRenderer::~BinauralRenderer()
{
  if (m_binaural_renderer)
  {
    iplDestroyBinauralRenderer(&m_binaural_renderer);
  }
}

IPLhandle BinauralRenderer::GetHandle()
{
  return m_binaural_renderer;
}

int32_t BinauralRenderer::GetFramesize()
{
  return m_framesize;
}

BinauralRenderer::BinauralRenderer(BinauralRenderer&& other) noexcept
{
  std::swap(m_binaural_renderer, other.m_binaural_renderer);
  std::swap(m_context, other.m_context);
  std::swap(m_framesize, other.m_framesize);
}

BinauralRenderer& BinauralRenderer::operator=(BinauralRenderer&& other) noexcept
{
  std::swap(m_binaural_renderer, other.m_binaural_renderer);
  std::swap(m_context, other.m_context);
  std::swap(m_framesize, other.m_framesize);
  return *this;
}
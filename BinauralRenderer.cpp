#include <exception>

#include "BinauralRenderer.hpp"

BinauralRenderer::BinauralRenderer(std::shared_ptr<Context> context, const IPLRenderingSettings& renderingSettings, const IPLHrtfParams& params)
{
  m_context = context;
  auto error = iplCreateBinauralRenderer(context->GetHandle(), renderingSettings, params, &m_binaural_renderer);
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
#include <exception>

#include "BinauralRenderer.hpp"

BinauralRenderer::BinauralRenderer(std::shared_ptr<Context> context, const IPLRenderingSettings& renderingSettings, const IPLHrtfParams& params) : m_context(context)
{
  IPLhandle renderer{ nullptr };
  auto error = iplCreateBinauralRenderer(context->GetHandle(), renderingSettings, params, &renderer);
  m_framesize = renderingSettings.frameSize;
  if (error)
  {
    throw std::exception("Error creating binaural renderer: " + error);
  }
  m_binaural_renderer = std::make_shared<IPLhandle>(renderer);
}

BinauralRenderer::~BinauralRenderer()
{
  if (m_binaural_renderer.unique())
  {
    iplDestroyBinauralRenderer(&(*m_binaural_renderer));
  }
}

IPLhandle BinauralRenderer::GetHandle()
{
  return *m_binaural_renderer;
}

int32_t BinauralRenderer::GetFramesize()
{
  return m_framesize;
}
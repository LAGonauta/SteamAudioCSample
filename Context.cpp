#include <exception>

#include "Context.hpp"

Context::Context(const IPLLogFunction& logCallback, const  IPLAllocateFunction& allocateCallback, const IPLFreeFunction& freeCallback)
{
  auto error = iplCreateContext(logCallback, allocateCallback, freeCallback, &m_context);
  if (error)
  {
    throw std::exception("Error creating binaural renderer: " + error);
  }
}

const IPLhandle Context::GetHandle()
{
  return m_context;
}

void Context::Destroy()
{
  iplDestroyContext(&m_context);
}

void Context::Cleanup()
{
  iplCleanup();
}
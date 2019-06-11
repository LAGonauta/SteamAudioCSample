#include <exception>

#include "Context.hpp"

Context::Context(const IPLLogFunction& logCallback, const  IPLAllocateFunction& allocateCallback, const IPLFreeFunction& freeCallback)
{
  IPLhandle context{ nullptr };
  auto error = iplCreateContext(logCallback, allocateCallback, freeCallback, &context);
  if (error)
  {
    throw std::exception("Error creating binaural renderer: " + error);
  }
  m_context = std::make_shared<IPLhandle>(context);
}

IPLhandle Context::GetHandle()
{
  return *m_context;
}

void Context::Destroy()
{
  iplDestroyContext(&(*m_context));
}

void Context::Cleanup()
{
  iplCleanup();
}
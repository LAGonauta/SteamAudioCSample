#pragma once

#include "common.h"

class Context final
{
private:
  friend class BinauralRenderer;
  friend class Environment;
  friend class EnvironmentalRenderer;
  friend class Scene;
  std::shared_ptr<IPLhandle> m_context{ nullptr };

  IPLhandle GetHandle();

public:
  DLL_PUBLIC Context(const IPLLogFunction& logCallback = nullptr, const  IPLAllocateFunction& allocateCallback = nullptr, const IPLFreeFunction& freeCallback = nullptr);

  DLL_PUBLIC void Destroy();

  DLL_PUBLIC void Cleanup();
};
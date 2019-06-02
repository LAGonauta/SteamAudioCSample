#pragma once

#include "common.h"

class Context final
{
private:
  friend class BinauralRenderer;
  friend class Environment;
  IPLhandle m_context{ nullptr };

  const IPLhandle GetHandle();

public:
  DLL_PUBLIC Context(const IPLLogFunction& logCallback = nullptr, const  IPLAllocateFunction& allocateCallback = nullptr, const IPLFreeFunction& freeCallback = nullptr);

  DLL_PUBLIC void Destroy();

  DLL_PUBLIC void Cleanup();
};
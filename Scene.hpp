#pragma once
#include <array>

#include "common.h"
#include "Context.hpp"

class Scene final
{
private:
  friend class Environment;
  std::array<std::tuple<std::string, IPLMaterial>, 11> default_materials_named =
  {
    std::tuple<std::string, IPLMaterial>("generic", IPLMaterial{ 0.10f,0.20f,0.30f,0.05f,0.100f,0.050f,0.030f }),
    std::tuple<std::string, IPLMaterial>("brick", { 0.03f,0.04f,0.07f,0.05f,0.015f,0.015f,0.015f }),
    std::tuple<std::string, IPLMaterial>("concrete", { 0.05f,0.07f,0.08f,0.05f,0.015f,0.002f,0.001f }),
    std::tuple<std::string, IPLMaterial>("ceramic", { 0.01f,0.02f,0.02f,0.05f,0.060f,0.044f,0.011f }),
    std::tuple<std::string, IPLMaterial>("gravel", { 0.60f,0.70f,0.80f,0.05f,0.031f,0.012f,0.008f }),
    std::tuple<std::string, IPLMaterial>("carpet",{0.24f,0.69f,0.73f,0.05f,0.020f,0.005f,0.003f }),
    std::tuple<std::string, IPLMaterial>("glass", { 0.06f,0.03f,0.02f,0.05f,0.060f,0.044f,0.011f }),
    std::tuple<std::string, IPLMaterial>("plaster", { 0.12f,0.06f,0.04f,0.05f,0.056f,0.056f,0.004f }),
    std::tuple<std::string, IPLMaterial>("wood", { 0.11f,0.07f,0.06f,0.05f,0.070f,0.014f,0.005f }),
    std::tuple<std::string, IPLMaterial>("metal", { 0.20f,0.07f,0.06f,0.05f,0.200f,0.025f,0.010f }),
    std::tuple<std::string, IPLMaterial>("rock", { 0.13f,0.20f,0.24f,0.05f,0.015f,0.002f,0.001f })
  };

  std::array<IPLMaterial, 11> default_materials =
  {
    IPLMaterial{ 0.10f,0.20f,0.30f,0.05f,0.100f,0.050f,0.030f },
    IPLMaterial{ 0.03f,0.04f,0.07f,0.05f,0.015f,0.015f,0.015f },
    IPLMaterial{ 0.05f,0.07f,0.08f,0.05f,0.015f,0.002f,0.001f },
    IPLMaterial{ 0.01f,0.02f,0.02f,0.05f,0.060f,0.044f,0.011f },
    IPLMaterial{ 0.60f,0.70f,0.80f,0.05f,0.031f,0.012f,0.008f },
    IPLMaterial{ 0.24f,0.69f,0.73f,0.05f,0.020f,0.005f,0.003f },
    IPLMaterial{ 0.06f,0.03f,0.02f,0.05f,0.060f,0.044f,0.011f },
    IPLMaterial{ 0.12f,0.06f,0.04f,0.05f,0.056f,0.056f,0.004f },
    IPLMaterial{ 0.11f,0.07f,0.06f,0.05f,0.070f,0.014f,0.005f },
    IPLMaterial{ 0.20f,0.07f,0.06f,0.05f,0.200f,0.025f,0.010f },
    IPLMaterial{ 0.13f,0.20f,0.24f,0.05f,0.015f,0.002f,0.001f }
  };


  std::shared_ptr<Context> m_context{ nullptr };
  std::shared_ptr<IPLhandle> m_scene{ nullptr };

  std::mutex m_mutex;

  IPLhandle GetHandle();

public:
  DLL_PUBLIC Scene(std::shared_ptr<Context> context,
    IPLhandle computeDevice,
    IPLSimulationSettings simulationSettings,
    IPLint32 numMaterials,
    IPLMaterial* materials,
    IPLClosestHitCallback closestCallback,
    IPLAnyHitCallback anyHitCallback,
    IPLBatchedClosestHitCallback batchedClosestCallback,
    IPLBatchedAnyHitCallback batchedAnyHitCallback,
    IPLvoid* userData);
  DLL_PUBLIC ~Scene();
};
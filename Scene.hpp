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
  IPLhandle m_scene{ nullptr };

  IPLhandle m_computeDevice{ nullptr };
  IPLSimulationSettings m_simulation_settings{};
  IPLint32 m_numMaterials{};
  IPLMaterial* m_materials{};
  IPLClosestHitCallback m_closestCallback{ nullptr };
  IPLAnyHitCallback m_anyHitCallback{ nullptr };
  IPLBatchedClosestHitCallback m_batchedClosestCallback{ nullptr };
  IPLBatchedAnyHitCallback m_batchedAnyHitCallback{ nullptr };
  IPLvoid* m_userData{};

  IPLhandle GetHandle();

  std::vector<std::vector<IPLbyte>> m_loadedScenes;

public:
  DLL_PUBLIC Scene(const Scene& other) = delete;
  DLL_PUBLIC Scene& Scene::operator=(const Scene& other) = delete;
  DLL_PUBLIC Scene(Scene&& other) noexcept;
  DLL_PUBLIC Scene& operator=(Scene&& other) noexcept;

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

  DLL_PUBLIC IPLerror CreateStaticMesh(IPLint32 numVertices, IPLint32 numTriangles, IPLVector3* vertices, IPLTriangle* triangles, IPLint32* materialIndices, IPLhandle* staticMesh);

  DLL_PUBLIC IPLerror LoadScene(const std::vector<IPLbyte>& data, IPLLoadSceneProgressCallback callback);
};
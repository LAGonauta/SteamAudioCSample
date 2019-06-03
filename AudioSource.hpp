#pragma once

#include <alure2.h>
#include <memory>

#include "common.h"
#include "BinauralRenderer.hpp"
#include "EnvironmentalRenderer.hpp"
#include "Structs.hpp"
#include "SteamAudioDecoder.hpp"
#include "Environment.hpp"

class AudioSource
{
private:
  std::shared_ptr<BinauralRenderer> m_renderer{ nullptr };
  std::shared_ptr<EnvironmentalRenderer> m_env_renderer{ nullptr };
  alure::Context m_al_context;
  alure::Source m_al_source;
  std::shared_ptr<SteamAudioDecoder> m_decoder;
  float GetMean(const float* data, const size_t dataSize);

public:
  IPLfloat32 Radius = 1;
  IPLDirectOcclusionMode OcclusionMode = IPL_DIRECTOCCLUSION_NONE;
  IPLDirectOcclusionMethod OcclusionMethod = IPL_DIRECTOCCLUSION_RAYCAST;

  AudioSource(std::shared_ptr<BinauralRenderer> renderer, std::shared_ptr<EnvironmentalRenderer> envRenderer, alure::Context al_context);
  ~AudioSource();

  void Play(std::string filename);

  void Stop();

  bool isPlaying();

  void SetPosition(const IPLSource& source, const std::shared_ptr<Environment> env, const IPLVector3& listenerPosition, const IPLVector3& listenerAhead, const IPLVector3& listenerUp);
};
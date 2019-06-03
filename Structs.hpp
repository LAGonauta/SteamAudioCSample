#pragma once

#include "common.h"

struct AudioBuffer
{
  std::vector<float> data;
  alure::SampleType type;
  alure::ChannelConfig channels;

  AudioBuffer() = default;
  AudioBuffer(std::vector<float> data, alure::SampleType type, alure::ChannelConfig channels) : data(data), type(type), channels(channels) { }
};
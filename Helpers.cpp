#pragma once

#include "common.h"
#include "Helpers.hpp"
#include "Externals/soxr-code/src/soxr.h"

IPLVector3 rotate_clockwise_over_zero(double radius, std::chrono::milliseconds run_duration, double angularVelocity)
{
  return IPLVector3
  {
    (IPLfloat32)(std::cos(run_duration.count() * angularVelocity / 1000) * radius),
    0,
    -(IPLfloat32)(std::sin(run_duration.count() * angularVelocity / 1000) * radius)
  };
}

IPLVector3 rotate_clockwise_over_zero(double radius, uint64_t run_duration, double angularVelocity)
{
  return IPLVector3
  {
    (IPLfloat32)(std::cos(run_duration * angularVelocity / 1000) * radius),
    0,
    -(IPLfloat32)(std::sin(run_duration * angularVelocity / 1000) * radius)
  };
}

size_t get_channel_quantity(alure::ChannelConfig config)
{
  switch (config)
  {
  case alure::ChannelConfig::Mono:
    return 1;
  case alure::ChannelConfig::Stereo:
    return 2;
  default:
    throw std::exception("Channel type not implemented.");
  }
}

size_t get_sample_size(alure::SampleType type)
{
  switch (type)
  {
  case alure::SampleType::Float32:
    return 4;
  case alure::SampleType::Int16:
    return 2;
  case alure::SampleType::Mulaw:
  case alure::SampleType::UInt8:
    return 1;
  }
}

soxr_datatype_t get_sox_type(alure::SampleType type)
{
  switch (type)
  {
  case alure::SampleType::Float32:
    return SOXR_FLOAT32_I;
  case alure::SampleType::Int16:
    return SOXR_INT16_I;
  default:
    throw std::exception("Sample type not supported");
  }
}

size_t get_real_input_size(alure::SampleType type, size_t size)
{
  switch (type)
  {
  case alure::SampleType::Float32:
    return size;
  case alure::SampleType::Int16:
    return size / 2;
  default:
    throw std::exception("Not supported");
  }
}
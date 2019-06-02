#pragma once

#include <chrono>
#include <alure2.h>

#include "common.h"
#include "Externals/soxr-code/src/soxr.h"

IPLVector3 rotate_clockwise_over_zero(double radius, std::chrono::milliseconds run_duration, double angularVelocity);

IPLVector3 rotate_clockwise_over_zero(double radius, uint64_t run_duration, double angularVelocity);

size_t get_channel_quantity(alure::ChannelConfig config);

size_t get_sample_size(alure::SampleType type);

soxr_datatype_t get_sox_type(alure::SampleType type);

size_t get_real_input_size(alure::SampleType type, size_t size);
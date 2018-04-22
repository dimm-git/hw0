#pragma once

#include "ktypes.h"

void generate(samples_type& s, dlib::rand& rand, long num, double radius, double shift_x = 0, double shift_y = 0);
bool read_sample(sample_type& sample, std::istream& in);

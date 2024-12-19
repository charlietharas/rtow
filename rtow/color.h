#pragma once

#include "common.h"

#include <ostream>

class vec3;

using color = vec3;

void write_color(std::ostream& out, const color& p);
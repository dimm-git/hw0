#pragma once

#include <list>
#include <string>
#include <tuple>
#include <vector>

using offsets = std::vector<std::size_t>;
using maps = std::vector<std::list<std::string> >;
using prefix = std::tuple<std::string, std::string, std::size_t>;
using results = std::list<prefix>;

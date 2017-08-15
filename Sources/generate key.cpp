//
//  generate key.cpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "generate key.hpp"

#include <random>

uint64_t generateKey() {
  static std::random_device gen;
  std::uniform_int_distribution<uint64_t> dist;
  return dist(gen);
}

/**
 * Copyright © 2019 Lehrstuhl Informatik 11 - RWTH Aachen University
 * 
 * This file is part of embeddedRTPS.
 * 
 * You should have received a copy of the MIT License along with embeddedRTPS.
 * If not, see <https://mit-license.org>.
 */

#pragma once

#include "rtps/entities/StatefulReader.h"
#include "rtps/entities/StatelessReader.h"
#include "rtps/entities/StatelessWriter.h"
#include "rtps/entities/Writer.h"

namespace rtps {

struct BuiltInEndpoints {
  Writer *spdpWriter = nullptr;
  Reader *spdpReader = nullptr;
  Writer *sedpPubWriter = nullptr;
  Reader *sedpPubReader = nullptr;
  Writer *sedpSubWriter = nullptr;
  Reader *sedpSubReader = nullptr;
};

}

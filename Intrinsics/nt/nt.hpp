#pragma once

// Order: forward typedefs → enums → concrete layouts → resolved imports → call wrappers.
#include "fwd.hpp"
#include "enums.hpp"
#include "struct/base.hpp"
#include "struct/xsave.hpp"
#include "struct/context.hpp"
#include "struct/mm.hpp"
#include "struct/wait.hpp"
#include "struct/io.hpp"

#include "method/all.hpp"

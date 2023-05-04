// This file contain includes that are commonly used in the
// engine code
#pragma once

// Engine utility functions
#include "utility/Error.hpp"
#include "utility/CheckError.hpp"
#include "utility/GetSetMacro.hpp"

// Standard library
#include "unordered_map"
#include "expected"
#include "sstream"
#include "fstream"
#include "string"
#include "memory"

// Logging related
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/common.h"
#include "spdlog/logger.h"

// OpenGL related
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// Math related
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Defines
#define UnreferencedParameter(expr) do { (void)(expr); } while (0)

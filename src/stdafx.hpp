#pragma once
#define _WIN7_PLATFORM_UPDATE
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define WINVER _WIN32_WINNT_WIN7
#define NOMINMAX

#include <algorithm>
#include <array>
#include <map>
#include <ranges>
#include <set>
#include <string>

#include <foobar2000/helpers/foobar2000+atl.h>
#include <foobar2000/helpers/DarkMode.h>

#include <wil/com.h>
#include <wil/resource.h>
#include <wincodec.h>

using namespace pfc::stringcvt;

#include "foo_cover_resizer.hpp"
#include "resource.hpp"
#include "DialogSettings.hpp"
#include "Helpers.hpp"
#include "CoverAttach.hpp"
#include "CoverConverterResizer.hpp"
#include "CoverRemover.hpp"

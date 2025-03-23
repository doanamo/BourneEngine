#pragma once

#include "Common/Containers/StringView.hpp"

bool WriteStringToFile(const StringView& filePath, const StringView& contents);

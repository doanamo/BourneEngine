#pragma once

#include "Common/Containers/StringView.hpp"

bool CheckFileExists(const StringView& filePath);
bool WriteStringToFileIfDifferent(const StringView& filePath, const StringView& contents);
bool WriteStringToFile(const StringView& filePath, const StringView& contents);
bool ReadStringFromFile(const StringView& filePath, String& contents);

#include "Shared.hpp"
#include "Common/Utility/Utility.hpp"

bool CheckFileExists(const StringView& filePath)
{
    struct stat stats;
    return stat(*filePath, &stats) == 0;
}

bool WriteStringToFileIfDifferent(const StringView& filePath, const StringView& contents)
{
    if(!CheckFileExists(filePath))
    {
        return WriteStringToFile(filePath, contents);
    }

    String existingContent;
    if(!ReadStringFromFile(filePath, existingContent))
    {
        return false;
    }

    if(existingContent != contents)
    {
        return WriteStringToFile(filePath, contents);
    }

    return true;
}

bool WriteStringToFile(const StringView& filePath, const StringView& contents)
{
    FILE* file = fopen(*filePath, "wb");
    if(!file)
    {
        LOG_ERROR("Failed to open file for writing: " STRING_VIEW_FORMAT, STRING_VIEW_VARG(filePath));
        return false;
    }

    SCOPE_GUARD
    {
        fclose(file);
    };

    const u64 written = fwrite(contents.GetData(), contents.GetCharSize(), contents.GetLength(), file);
    if(written != contents.GetLength())
    {
        LOG_ERROR("Failed to write file contents: " STRING_VIEW_FORMAT, STRING_VIEW_VARG(filePath));
        return false;
    }

    return true;
}

bool ReadStringFromFile(const StringView& filePath, String& contents)
{
    FILE* file = fopen(*filePath, "rb");
    if(!file)
    {
        LOG_ERROR("Failed to open file for reading: " STRING_VIEW_FORMAT, STRING_VIEW_VARG(filePath));
        return false;
    }

    SCOPE_GUARD
    {
        fclose(file);
    };

    fseek(file, 0, SEEK_END);
    const u64 size = ftell(file);

    contents.Resize(size, '\0');
    fseek(file, 0, SEEK_SET);

    const u64 read = fread(contents.GetData(), 1, size, file);
    if(read != size)
    {
        LOG_ERROR("Failed to read file contents: " STRING_VIEW_FORMAT, STRING_VIEW_VARG(filePath));
        return false;
    }

    return true;
}

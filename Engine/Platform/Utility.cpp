#include "Shared.hpp"
#include "Common/Utility/Utility.hpp"

bool WriteStringToFile(const StringView& filePath, const StringView& contents)
{
    FILE* file = fopen(*filePath, "w");
    if(!file)
    {
        LOG_ERROR("Failed to open \"" STRING_VIEW_FORMAT "\" for writing", STRING_VIEW_VARG(filePath));
        return false;
    }

    SCOPE_GUARD
    {
        fclose(file);
    };

    const u64 written = fwrite(contents.GetData(), contents.GetCharSize(), contents.GetLength(), file);
    return written == contents.GetLength();
}

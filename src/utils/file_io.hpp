#pragma once

#include <string_view>
#include <vector>

namespace Aery {
    enum class FileLoadOptions {
        eNone = 0,
        eBinary = 1 << 0,
    };
    FileLoadOptions operator|(FileLoadOptions a, FileLoadOptions b);
    bool operator&(FileLoadOptions a, FileLoadOptions b);

    bool GetFileContents(const char*, std::vector<char>&, FileLoadOptions);
}

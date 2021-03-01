#include "utils/types.hpp"
#include "utils/debug.hpp"
#include "utils/file_io.hpp"
#include <string_view>
#include <fstream>
#include <cassert>

namespace Aery {
    FileLoadOptions operator|(FileLoadOptions a, FileLoadOptions b) {
        return static_cast<FileLoadOptions>(static_cast<mut_u16>(a), static_cast<mut_u16>(b));
    }

    bool operator&(FileLoadOptions a, FileLoadOptions b) {
        return static_cast<mut_u16>(a) & static_cast<mut_u16>(b);
    }

    bool GetFileContents(const char* Path, std::vector<char>& Output, FileLoadOptions Options) {
        assert(Path != nullptr);

        std::ios::openmode OpenMode;
        if (Options & FileLoadOptions::eBinary) OpenMode = std::ios::ate | std::ios::binary;
        else OpenMode = std::ios::ate;

        std::ifstream Input = std::ifstream(Path, OpenMode);

        if (!Input.is_open()) {
            Aery::error(debug_format("<GetFileContents> Failed to open file {}.", Path));
            Output.resize(1, 'a');
            return false;
        }
        Output.clear();
        
        u32 FileSize = static_cast<u32>(Input.tellg());
        Output.resize(FileSize);
        Input.seekg(0);
        Input.read(Output.data(), FileSize);
        Input.close();
        return true;
    }
}
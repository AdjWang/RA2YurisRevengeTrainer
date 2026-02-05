#pragma once
#include <filesystem>
namespace fs = std::filesystem;

namespace yrtr {
namespace debug {
// For crash dump testing.
extern void CauseCrash();
// Register crash dumper. The dump_dir has to be exist otherwise .dmp file would
// fail to write.
extern void InitCrashDump(const fs::path& dump_dir);
}  // namespace debug
}  // namespace yrtr

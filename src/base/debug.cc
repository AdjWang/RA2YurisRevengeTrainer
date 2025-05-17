#include "base/debug.h"

#include <iomanip>
#include <sstream>
#include <stacktrace>

namespace yrtr {
namespace debug {

std::string DumpStackTrace() {
  auto trace = std::stacktrace::current();
  std::stringstream output;
  output << "Stack trace:\n";
  size_t frame_num = 0;
  for (const auto& entry : trace) {
    output << "  #" << frame_num++ << " ";
    if (entry) {
      output << entry.description() << " in " << entry.source_file() << ":"
             << entry.source_line();
    } else {
      output << "[unknown]";
    }
    output << "\n";
  }
  return output.str();
}

}  // namespace debug
}  // namespace yrtr

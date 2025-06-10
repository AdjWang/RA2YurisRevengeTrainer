import sys
from pathlib import Path
SCRIPT_DIR = Path(sys.argv[0]).parent

CPP_TEMPLATE = """
// Auto generated file, do not modify.
#pragma once
#include <string_view>
namespace yrtr {{
inline static constexpr std::string_view kMainPageHtml = R"({})";
}}
"""

if __name__ == '__main__':
    WEB_DIST_PATH = SCRIPT_DIR / '../src/frontend/web/dist/index.html'
    OUTPUT_PATH = SCRIPT_DIR / '../src/protocol/main_page.h'

    if not Path.exists(WEB_DIST_PATH):
        print(f'Failed to find dist dir={WEB_DIST_PATH}, build web first')
        sys.exit(1)
    with open(WEB_DIST_PATH, 'r', encoding='utf-8') as f:
        html_content = f.read()
    cpp_content = CPP_TEMPLATE.format(html_content)
    with open(OUTPUT_PATH, 'w', encoding='utf-8') as f:
        f.write(cpp_content)
    

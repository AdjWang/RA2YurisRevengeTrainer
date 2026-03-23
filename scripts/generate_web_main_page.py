import sys
from pathlib import Path

SCRIPT_DIR = Path(sys.argv[0]).parent
CPP_TEMPLATE = """
// Auto generated file, do not modify.
#pragma once
namespace yrtr {{
inline static constexpr unsigned char kMainPageHtml[] = {{ {} }};
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
    # MSVC string literal has a limit of 16380 characters, use byte array instead.
    # https://learn.microsoft.com/en-us/cpp/error-messages/compiler-errors-1/compiler-error-c2026?view=msvc-170
    byte_array_content = ', '.join([f'0x{b:02X}' for b in html_content.encode('utf-8')])
    cpp_content = CPP_TEMPLATE.format(byte_array_content)
    with open(OUTPUT_PATH, 'w', encoding='utf-8') as f:
        f.write(cpp_content)

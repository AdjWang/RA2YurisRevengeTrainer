import sys
from pathlib import Path
SCRIPT_DIR = Path(sys.argv[0]).parent
import re

def cut_ranges(data):
    range_collection = []
    sub_range = []
    for a, b in zip(data[:-1], data[1:]):
        sub_range.append(a)
        if a + 1 != b:
            range_collection.append(sub_range)
            sub_range = []
    if len(sub_range) > 0:
        range_collection.append(sub_range + [b])
    else:
        range_collection.append([b])

    return range_collection


def compress_ranges(range_collection):
    return [(r[0], r[-1]) for r in range_collection]
    
def print_cpp_range_array(range_collection):
    print('{')
    for a, b in range_collection:
        print(f"    0x{a:04X}, 0x{b:04X}, ")
    print('    0,')
    print('};')
    

if __name__ == '__main__':
    with open(SCRIPT_DIR / '../src/char_table.h', 'r') as f:
        char_tables = f.read()
    res = re.findall(r'u8"(.*)"', char_tables)
    # print(res, len(res))
    zh_chs = list(set(''.join(res)))
    num_chs = [f"{i}" for i in range(10)]
    custom_chs = list(": ")
    # for c in (zh_chs + num_chs + custom_chs):
    #     print(c, f"0x{ord(c):04X}")
    ords = sorted([ord(c) for c in (zh_chs + num_chs + custom_chs)])
    # for i in ords:
    #     print(f"0x{i:04X}")
    range_collection = cut_ranges(ords)
    # for r in range_collection:
    #     print([f"0x{i:04X}" for i in r])
    range_collection = compress_ranges(range_collection)
    # for r in range_collection:
    #     print([f"0x{i:04X}" for i in r])
    print_cpp_range_array(range_collection)

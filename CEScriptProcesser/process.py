# -*- coding: utf-8 -*-

with open('.\\script.txt', 'r') as f:
    script = f.readlines()
with open('.\\script.txt', 'w') as f:
    for line in script:
        if line.strip(' ') == '\n':             # delete empty line
            continue
        line = line.replace('\"', '\\\"')       # replace " to \"
        if line[-1] == '\n':                    # change tail of line to \n\ + \n
            line = line[:-1] + '\\n\\' + '\n'   
        else:
            line = line + '\\n\\' + '\n'
        f.write(line)

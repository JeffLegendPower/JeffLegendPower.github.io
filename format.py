def make_page(posts):
    spacer="""
<div class="post-spacer"></div>
"""
    return f"""
<!DOCTYPE html>
<html>
    <head>
        <link rel="preconnect" href="https://fonts.googleapis.com">
        <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
        <link href="https://fonts.googleapis.com/css2?family=Noto+Sans+Mono:wght@100..900&display=swap" rel="stylesheet">
        <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0">
        <link rel="stylesheet" href="style.css">
        <title>Templates</title>
    </head>
    <body>
        <div id="flex-stack">
            <div id="title-outer">
                <div id="title-inner">
                    <h2>Templates</h2>
                </div>
            </div>
            <div id="posts-outer">
                <div id="posts-inner">
{spacer.join(posts)}
                </div>
            </div>
        </div>
    </body>
</html>"""

def make_post(name, content):
    return f"""
<div class="post"><div class="content">
    <div class="post-title">
        <a class="post-title-name">{name}</a>
    </div>
    {content}
</div></div>"""

def process_file(name):
    f = open(f"templates/{name}.cpp")
    code = f.read().replace('\t', '    ').split('\n')
    f.close()

    tokens = ['']
    splits = [
            ' ', '*', '&', '[', ']', '(', ')', '{', '}', ';',
            ',', '.', '>', '<', '=', '-', '+', '/', '?', ':', '!'
        ]

    for line in code:
        if line.strip().startswith('//'):
            if len(tokens[-1]): tokens.append(line)
            else: tokens[-1] = line

        elif line.strip().startswith('/*') and line.strip().endswith('*/'):
            if len(tokens[-1]): tokens.append(line)
            else: tokens[-1] = line

        elif line.strip().startswith('#include'):
            if len(tokens[-1]): tokens.append('#include')
            else: tokens[-1] = '#include'
            tokens.append(' ')
            tokens.append(line.strip()[9:])

        elif len(line.strip()):
            if len(tokens[-1]): tokens.append('')

            instring, escape = False, False
            inangle = False

            for c in line:
                if instring:
                    if c == instring and not escape:
                        tokens[-1] += c
                        tokens.append('')
                        instring = False

                    else:
                        tokens[-1] += c

                        if c == '\\': escape = True
                        else: escape = False

                elif c in ['"', "'"]:
                    instring = c
                    if len(tokens[-1]): tokens.append(c)
                    else: tokens[-1] = c

                elif c in splits:
                    if len(tokens[-1]): tokens.append(c)
                    else: tokens[-1] = c
                    tokens.append('')

                else: tokens[-1] += c

        if len(tokens[-1]): tokens.append('\n')
        else: tokens[-1] = '\n'

    if not len(tokens[-1]): tokens.pop()
    while len(tokens) and tokens[-1] == '\n': tokens.pop()

    escape = [['&', '&amp;'], ['<', '&lt;'], ['>', '&gt;']]

    for i in range(len(tokens)):
        for e in escape:
            tokens[i] = tokens[i].replace(e[0], e[1])

        if i and tokens[i] == '\n' and tokens[i-1] in ['\n', '<br>']:
            tokens[i-1] = '<br>'

    reserved = {
            'struct': 'g', 'union': 'g', 'enum': 'g', 'namespace': 'g', 'static': 'g', 'inline': 'g',
            'void': 'g', 'const': 'g', 'constexpr': 'g', 'volatile': 'g', 'static': 'g', 'auto': 'g',
            'int': 'g', 'long': 'g', 'float': 'g', 'double': 'g', 'char': 'g', 'bool': 'g', 'size_t': 'g',
            'unsigned': 'g', 'signed': 'g', 'short': 'g',
            'true': 'y', 'false': 'y', 'null': 'y',
            'malloc': 'y', 'free': 'y', 'assert': 'y', 'using': 'y', 'typedef': 'y', 'operator': 'y',
            'break': 'y', 'continue': 'y', 'return': 'y', 'goto': 'y', 'new': 'y',
            'while': 'y', 'if': 'y', 'for': 'y', 'do': 'y', 'else': 'y', 'switch': 'y', 'case': 'y',
            '[': 'b', ']': 'b'
        }

    formatted = ''

    def iscom(i):
        return i.strip().startswith('//') or (i.strip().startswith('/*') and i.strip().endswith('*/'))

    def isnum(i):
        if tokens[i] == '-':
            if i == len(tokens)-1: return False
            return isonly(tokens[i+1], '0123456789e')

        if tokens[i] in '.':
            if i == len(tokens)-1 or i == 0: return False
            return isonly(tokens[i-1], '0123456789e') and isonly(tokens[i+1], '0123456789e')

        if len(tokens[i]) > 1 and tokens[i][0] == '0' and tokens[i][1] == 'x':
            return isonly(tokens[i][2:], '0123456789abcdef')

        return isonly(tokens[i], '0123456789e')

    def isstr(i):
        return len(i) > 2 and i[0] in ["'", '"'] and i[-1] == i[0]

    def isonly(s, allowed):
        val = 0
        for c in s:
            if c in allowed: val += 1
        return val == len(s)

    def islbl(s):
        return len(s) > 1 and s[-1] == ':'

    def isstruct(i):
        if tokens[i] == ' ': return False

        if i == 0: return False
        if tokens[i-1] == 'struct': return True

        if tokens[i-1] == ' ':
            if i == 1: return False
            
            return tokens[i-2] == 'struct'

        return False

    def isfunc(i):
        if tokens[i] == ' ': return False

        if i == len(tokens)-1: return False
        if tokens[i+1] == '(': return isonly(tokens[i], 'abcdefghijklmnopqrstuvwxyz0123456789_')

        if tokens[i+1] == ' ':
            if i == len(tokens)-2: return False
            
            return tokens[i+2] == '(' and isonly(tokens[i], 'abcdefghijklmnopqrstuvwxyz0123456789_')
        
        return False

    def isdef(i):
        return i > 1 and tokens[i-1] == ' ' and tokens[i-2] in ['#define', '#ifdef', '#ifndef']

    def isinc(i):
        if tokens[i].startswith('&lt;') and tokens[i].endswith('&gt;'): return True
        return tokens[i][0] == '"' and tokens[i][len(tokens[i])-1] == '"' and i > 1 and tokens[i-1] == ' ' and tokens[i-2] == '#include'

    for i in range(len(tokens)):
        if tokens[i] in reserved:
            formatted += f'<span class={reserved[tokens[i]]}>{tokens[i]}</span>'

        else:
            if iscom(tokens[i]):
                formatted += f'<span class=r>{tokens[i]}</span>'

            elif isstruct(i) or isfunc(i) or isdef(i) or isinc(i):
                formatted += f'<span class=b>{tokens[i]}</span>'

            elif isnum(i) or islbl(tokens[i]) or isstr(tokens[i]):
                formatted += f'<span class=y>{tokens[i]}</span>'

            elif tokens[i].startswith('#'):
                formatted += f'<span class=y>{tokens[i]}</span>'

            else: formatted += tokens[i]

    return make_post(name, '<pre class="code-color">' + formatted + '</pre>')

import glob
import os

templates = glob.glob("templates/*.cpp")

proc = [process_file(x[10:-4]) for x in templates]

f = open("index.html", "w")
f.write(make_page(proc))
f.close()

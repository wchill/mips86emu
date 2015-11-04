#!/usr/bin/python
asm = raw_input('Input hex: ')
with open('program.hex', 'wb') as f:
    out = asm.decode('hex')
    f.write(out)

#!/usr/bin/python
import binascii
asm = None
with open('program.bin', 'rb') as f:
    asm = f.readlines()
with open('program.mips', 'wb') as f:
    for line in asm:
        print(line)
        n = int(line.strip(), 2)
        out = binascii.unhexlify('%x' % n)
        f.write(out)
    # f.write('0000000c'.decode('hex'))

#!/usr/bin/env python3
import sys

if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit("Usage: {} file.bin\n".format(sys.argv[0]))

    ps_shellcode = '[Byte[]] $nice = '

    with open(sys.argv[1], 'rb') as shellcode:
        byte = shellcode.read(1)
        while byte:
            ps_shellcode += '0x{:02X}, '.format(byte[0])
            byte = shellcode.read(1)

    ps_shellcode = ps_shellcode[:-2]  # get rid of the last whitespace and comma
    print(ps_shellcode.replace(" ", ""))

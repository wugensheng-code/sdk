#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################

#!/usr/bin/python
import sys
import struct
import os

if len(sys.argv) < 3:
    print("Usage: python dump_gcov.py <file2dump.bin> <output path>")
    sys.exit(1)

fname = sys.argv[1]
out_path = sys.argv[2]
os.mkdir(out_path)
with open(fname, "rb") as f:
    raw = f.read()

while len(raw) > 0:
    fullpath, _, load = raw.partition(b'\x00')
    path, _, name = fullpath.decode("ASCII").rpartition('/')
    print("Found {:s}".format(name), end=":\t")
    length, = struct.unpack(">I", load[0: 4])
    out_file = os.path.join(out_path, name)
    with open(out_file, "wb") as f:
        f.write(load[4: 4 + length])
    print("{:d} bytes".format(length))
    raw = load[4 + length: ]

import sys
import json
import pefile

def to_int32(val):
    val &= 0xffffffff
    return val - 0x100000000 if val & 0x80000000 else val

mode = sys.argv[1]
if mode not in ("decode", "encode"):
    exit("wrong mode")

with open(sys.argv[3]) as f:
    conf = json.load(f)

with open(sys.argv[2], "rb") as f:
    p = pefile.PE(data=f.read())
    section = p.sections[0]
    if section.Name != b".decc\x00\x00\x00":
        exit("section not found")
    decc = bytearray(section.get_data())
    edi = conf["initial"]

    for i in range(len(decc)):
        b = decc[i]
        high = low = 0
        if conf["rotate"] == "before":
            edi = ((edi >> 3) ^ (edi * 0x3245 + 0x1b01)) & 0xffffffff
        match mode, conf["order"][edi & 3]:
            case _, 0:
                high = (b & 0x55) << 1
                low = b >> 1 & 0x55
            case _, 1:
                high = (b & 0x33) << 2
                low = b >> 2 & 0x33
            case _, 2:
                high = (b & 0xf) << 4
                low = b >> 4
            case "decode", 3:
                high = (b & 0x7f) << 1
                low = b >> 7
            case "encode", 3:
                high = (b & 1) << 7
                low = b >> 1
        decc[i] = high + low
        if conf["rotate"] == "after":
            edi = ((to_int32(edi) >> 3) ^ (edi * 0x3245 + 0x1b01)) & 0xffffffff

    p.set_bytes_at_rva(section.VirtualAddress, bytes(decc))
    p.write(sys.argv[2] + ".dll")

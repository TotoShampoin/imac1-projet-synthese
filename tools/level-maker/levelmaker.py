#!/usr/bin/python3

from sys import argv
from parse import parse
import ctypes

length = 0
width = 0
height = 0
obstacles = []
bonuses = []

with open(argv[1], "r") as file:
    for _line in file.readlines():
        line = _line.replace("\n", "")
        if line.startswith("l "):
            length, width, height = parse("l {} {} {}", line)
            length, width, height = float(length), float(width), float(height)
        if line.startswith("o "):
            res = parse("o {} {} {} {} {} {}", line)
            x1 , y1 , z1 , x2 , y2 , z2 = res
            x1 , y1 , z1 = float(x1) , float(y1) , float(z1)
            x2 , y2 , z2 = float(x2) , float(y2) , float(z2)
            obstacles.append({
                "boundA": (x1, y1, z1),
                "boundB": (x2, y2, z2)
            })
        if line.startswith("b "):
            typ , x , y , z = parse("b {} {} {} {}", line)
            typ = int(typ)
            x , y , z = float(x) , float(y) , float(z)
            bonuses.append({
                "type": typ,
                "coord": (x, y, z)
            })

print(length, len(obstacles), len(bonuses))

with open(argv[2], "wb") as file:
    file.write(ctypes.c_float(length))
    file.write(ctypes.c_float(width))
    file.write(ctypes.c_float(height))
    file.write(ctypes.c_uint64(len(obstacles)))
    file.write(ctypes.c_uint64(len(bonuses)))
    for obs in obstacles:
        file.write(ctypes.c_float(obs["boundA"][0]))
        file.write(ctypes.c_float(obs["boundA"][1]))
        file.write(ctypes.c_float(obs["boundA"][2]))
        file.write(ctypes.c_float(obs["boundB"][0]))
        file.write(ctypes.c_float(obs["boundB"][1]))
        file.write(ctypes.c_float(obs["boundB"][2]))
    for bon in bonuses:
        file.write(ctypes.c_uint32(bon["type"]))
        file.write(ctypes.c_float(bon["coord"][0]))
        file.write(ctypes.c_float(bon["coord"][1]))
        file.write(ctypes.c_float(bon["coord"][2]))

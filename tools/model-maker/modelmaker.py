#!/usr/bin/python3

from sys import argv
import ctypes
import pywavefront

scene = pywavefront.Wavefront(argv[1], collect_faces=True, create_materials=True)

vertices = []
faces = []
tex_coords = []

# print(scene.materials.popitem()[1].vertices)

for v in scene.vertices:
    vertices += v
for f in scene.mesh_list[0].faces:
    faces += f
for t in range(len(scene.vertices)*2):
    tex_coords += [0]

with open(argv[2], "wb") as file:
    file.write(ctypes.c_ulong(len(vertices)//3))
    file.write(ctypes.c_ulong(len(faces)//3))
    print(len(vertices)//3, len(faces)//3)
    for v in vertices:
        file.write(ctypes.c_float(v))
    for t in tex_coords:
        file.write(ctypes.c_float(v))
    for f in faces:
        file.write(ctypes.c_uint(f))


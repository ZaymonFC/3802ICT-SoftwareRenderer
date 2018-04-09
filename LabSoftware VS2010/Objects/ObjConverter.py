import json
import re
import random as rand

#
# ─── FORMATTING ─────────────────────────────────────────────────────────────────
#
def get_data_dictionary(vertices, faces):
    return {
        'VertexCount': len(vertices),
        'Vertices' : vertices,
        'PolygonCount': len(faces),
        'Polygons': faces
    }
    

#
# ─── CREATION FUNCTIONS ─────────────────────────────────────────────────────────
#
def add_vertex(coordinates):
    point = []
    
    # Add the coordinates
    print(f'Vertex Coord Count: {len(coordinates)}')
    for coordinate in coordinates:
        if coordinate == '':
            continue
        point.append(float(coordinate))
    
    # Add the colours
    point.append(0)
    point.append(rand.randint(100, 200))
    point.append(rand.randint(100, 255))

    vertices.append(point)
    

def add_face(indexes):
    face = []
    print(f'Indexes: {len(indexes)}')
    for index in indexes:
        if index == '':
            continue
        values = index.split('/')
        face.append(int(values[0]) - 1)
    
    faces.append(face)


#
# ─── MAIN ───────────────────────────────────────────────────────────────────────
#
faces = []
vertices = []
colour = [250, 0, 0]

file_data = []

file_name = 'compass'

with open(f'{file_name}.obj') as fp:
    file_data = fp.read().splitlines()

for line in file_data:
    line_values = line.split(' ')
    if line_values[0] == 'v':
        add_vertex(line_values[1:])
    elif line_values[0] == 'f':
        add_face(line_values[1:])

json_string = json.dumps(get_data_dictionary(vertices, faces), indent=2)

with open(f'{file_name}.json', 'w+') as fp:
    fp.write(json_string)
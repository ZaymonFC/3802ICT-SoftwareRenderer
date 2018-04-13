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
    if not greyscale:
        point.append(rand.randint(redMin, redMax))
        point.append(rand.randint(greenMin, greenMax))
        point.append(rand.randint(blueMin, blueMax))
    else:
        colour = rand.randint(greyscaleMin, greyscaleMax)
        point.append(colour)
        point.append(colour)
        point.append(colour)

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

# Colours
redMin   =  100
redMax   =  200
greenMin =  100
greenMax =  200
blueMin  =  50
blueMax  =  255

greyscale = False
greyscaleMin = 150
greyscaleMax = 230

file_data = []

file_name = 'Star'

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
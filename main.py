import json
import uvicorn
import math
import subprocess
import os
import heapq
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from fastapi.responses import FileResponse
from pydantic import BaseModel

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

def dist(p1, p2):
    return math.hypot(p1[0]-p2[0], p1[1]-p2[1])

NODES = []
node_id_map = {} # map (x,y) to internal index
place_map = {} # map place ID (1-25) to internal index
EDGES = []

# Initialize
with open("issax.geojson") as f:
    roads = json.load(f)

with open("place-points.geojson") as f:
    places = json.load(f)

# Collect all points from roads
road_points = set()
for f in roads['features']:
    if f['geometry']['type'] == 'MultiLineString':
        for line in f['geometry']['coordinates']:
            for pt in line:
                road_points.add((round(pt[0], 2), round(pt[1], 2)))

# Assign indices to road points
for p in road_points:
    idx = len(NODES)
    node_id_map[p] = idx
    NODES.append(p)

# Create edges for roads
# To match exactly, we'll iterate again and add edges between consecutive points
for f in roads['features']:
    if f['geometry']['type'] == 'MultiLineString':
        for line in f['geometry']['coordinates']:
            for i in range(len(line)-1):
                p1 = (round(line[i][0], 2), round(line[i][1], 2))
                p2 = (round(line[i+1][0], 2), round(line[i+1][1], 2))
                idx1 = node_id_map[p1]
                idx2 = node_id_map[p2]
                d = int(dist(p1, p2))
                EDGES.append((idx1, idx2, d))
                EDGES.append((idx2, idx1, d))

# Snap nearby road points to connect disconnected lines
road_nodes_count = len(NODES)
for i in range(road_nodes_count):
    for j in range(i+1, road_nodes_count):
        d = int(dist(NODES[i], NODES[j]))
        if d < 300:  # Tolerance distance
            EDGES.append((i, j, d))
            EDGES.append((j, i, d))

# Add places and snap to closest road point
for f in places['features']:
    pid = f['properties']['id']
    pt = f['geometry']['coordinates']
    px, py = round(pt[0], 2), round(pt[1], 2)
    
    idx = len(NODES)
    NODES.append((px, py))
    place_map[pid] = idx
    
    # find closest road point
    closest_idx = -1
    min_d = float('inf')
    for rp in road_points:
        d = dist((px, py), rp)
        if d < min_d:
            min_d = d
            closest_idx = node_id_map[rp]
    
    # edge from place to road
    d_int = int(min_d)
    EDGES.append((idx, closest_idx, d_int))
    EDGES.append((closest_idx, idx, d_int))

N = len(NODES)
INF = 999999999
matrix = [[INF for _ in range(N)] for _ in range(N)]
for i in range(N):
    matrix[i][i] = 0

for u, v, w in EDGES:
    if w < matrix[u][v]:
        matrix[u][v] = w
        matrix[v][u] = w

@app.get("/shortest-path")
def shortest_path(start: int, end: int, mode: str = "road"):
    if start not in place_map or end not in place_map:
        return {"error": "Invalid start or end place ID"}
    
    u = place_map[start]
    v = place_map[end]
    
    if mode == "any":
        path_coords = [NODES[u], NODES[v]]
        d = int(dist(NODES[u], NODES[v]))
        return {"distance": d, "path": path_coords, "all_paths": []}

    def floyd_warshall(src, target, penalties=None):
        if penalties is None: penalties = {}
        
        fw = [[INF for _ in range(N)] for _ in range(N)]
        nxt = [[-1 for _ in range(N)] for _ in range(N)]
        
        # Initialize matrix with penalties
        for i in range(N):
            fw[i][i] = 0
            for j in range(N):
                if i != j and matrix[i][j] < INF:
                    edge = tuple(sorted([i, j]))
                    fw[i][j] = matrix[i][j] * penalties.get(edge, 1)
                    nxt[i][j] = j
                    
        # Floyd-Warshall O(V^3) computation
        for k in range(N):
            for i in range(N):
                if fw[i][k] >= INF: continue
                for j in range(N):
                    if fw[k][j] >= INF: continue
                    if fw[i][k] + fw[k][j] < fw[i][j]:
                        fw[i][j] = fw[i][k] + fw[k][j]
                        nxt[i][j] = nxt[i][k]
                        
        if fw[src][target] >= INF: return -1, []
        
        # Reconstruct path
        path = [src]
        curr = src
        while curr != target:
            curr = nxt[curr][target]
            if curr == -1: return -1, []
            path.append(curr)
            
        return fw[src][target], path

    # Calculate optimal path
    d_opt, p_opt = floyd_warshall(u, v)
    if d_opt == -1:
        return {"distance": -1, "path": [], "all_paths": []}
    
    all_paths = []
    
    # Generate alternatives by penalizing optimal path edges
    penalties = {}
    for i in range(len(p_opt)-1):
        edge = tuple(sorted([p_opt[i], p_opt[i+1]]))
        penalties[edge] = 5.0 # penalty to force alternate routes
        
    d_alt1, p_alt1 = floyd_warshall(u, v, penalties)
    if d_alt1 != -1 and p_alt1 != p_opt:
        all_paths.append([NODES[x] for x in p_alt1])
        
        # Penalize this one too
        for i in range(len(p_alt1)-1):
            edge = tuple(sorted([p_alt1[i], p_alt1[i+1]]))
            penalties[edge] = 5.0
            
        d_alt2, p_alt2 = floyd_warshall(u, v, penalties)
        if d_alt2 != -1 and p_alt2 != p_opt and p_alt2 != p_alt1:
            all_paths.append([NODES[x] for x in p_alt2])

    path_coords = [NODES[x] for x in p_opt]
    return {"distance": int(d_opt), "path": path_coords, "all_paths": all_paths}

@app.get("/data")
def get_data():
    return {
        "places": places,
        "roads": roads,
        "place_map": place_map,
        "nodes": NODES
    }

app.mount("/", StaticFiles(directory=".", html=True), name="static")

if __name__ == "__main__":
    uvicorn.run("main:app", host="127.0.0.1", port=8000, reload=True)


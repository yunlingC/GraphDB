import graph_python
g = graph_python.GraphType()

v1_id = g.addVertex()
v2_id = g.addVertex()
v3_id = g.addVertex()
v4_id = g.addVertex()
v5_id = g.addVertex()
v6_id = g.addVertex()

e1_id = g.addEdge(v1_id,v2_id)
e2_id = g.addEdge(v2_id,v3_id)
e3_id = g.addEdge(v3_id,v4_id)
e4_id = g.addEdge(v1_id,v6_id)
e5_id = g.addEdge(v2_id,v5_id)
print("printing all vertex IDs")
print(v1_id)
print(v2_id)
print(v3_id)
print(v4_id)
print(v5_id)
print(v6_id)
print("printing all vertex pointers from getVertexPointer")
print(g.getVertexPointer(v1_id))
print(g.getVertexPointer(v2_id))
print(g.getVertexPointer(v3_id))
print(g.getVertexPointer(v4_id))
print(g.getVertexPointer(v5_id))
print(g.getVertexPointer(v6_id))

print("printing all vertex pointers from getAll Vertices")

vlist = g.getAllVertices()
i=0
for v in vlist:
    print(i)
    print(v)
    i=i+1
print list(vlist)

#vidlist to be sent as input to BFS function; giving a length of 10 to vidlist
vidlist=graph_python.vector_uint()
print(len(vidlist))
vidlist.extend(0 for a in range(10))
print(len(vidlist))

graph_python.breadthFirstSearch(g,0,vidlist)
print("printing output of BFS")
i=0
for a in vidlist:
    print(i,a)
    i=i+1

print list(vidlist)

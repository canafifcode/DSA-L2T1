//Problem: Find a spanning tree where the weight of the 
//heaviest edge is minimized.

//solution: Use modified Kruskal's algorithm

/*
thinking process:
1. Sort all edges in non-decreasing order of their weights.
2. Initialize an empty graph to store the MST.
3. Iterate through the sorted edges and add each edge to the MST 
    if it doesn't create a cycle.
4. The last edge added to the MST will be the heaviest edge in the spanning tree.
*/
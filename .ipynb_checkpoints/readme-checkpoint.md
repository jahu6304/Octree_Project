James Hundertmark
CSPB 2270 Final Project

Project Data Structure Implemented:
    Relevant Data Structure: Octree
    Additional Data Strucutr: 3 dimensional array
    
The Octree data structure processes each coordinate of the 3-d array into a node at a leaf of the tree (all individual coordinates are at the lowest node).
Higher nodes then refelct a "true" bool value if any of their child nodes are true, but false if not.  This permits a new  array to be created with the coordinates with the same bool value to be summarized together, while differing coordinates will be represented specifically, reducing the size of the data, while maintaining the relevant information.

Project Will run with ./run_test
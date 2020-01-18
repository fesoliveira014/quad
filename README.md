# Quadtree Implementation in C
This repository contains a quadtree implementation in C. This implementation is capable of adding and removing points from the quadtree.

## Behavior
The quadtree starts empty. A quadtree cell can contain at most 1 point before a subdivision is triggered. When a second point is added, the quadrant in which the new point belongs will suffer a subdivision, creating 4 new children. If any of the children contains more than 1 point, it will subdivide in 4 new children, and so on, until a child only has at most 1 point in it. 

Removing points can trigger a merging. If all children of a given quadrant are empty after removing a point, the children will be destroyed and merged into the parent quadrant. This recursivelly go on until the merged quadrant is no longer empty. Currently this only happen if no points are present inside the children of a quadrant, which means that if only one child has one point, the children won't collapse.

Querying the tree will only return if a given point exists inside the tree or not. 

An invalid point is defined as {-1, -1}.

## Compiling
To compile the example, just `cd` into the `quad` directory and `make`. To include it in your project, just use `quad.h` and `quad.c`.

## License
This code is under MIT License.

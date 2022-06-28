# Self-balancing Tree - Red Black Tree
This is based on what I have learned from the book [](https://mitpress.mit.edu/books/introduction-algorithms-third-edition)`Introduction to Algorithms, 3rd Edititon (The MIT Press)`.
## Concept
**Red Black Tree** is a self-balancing tree data structure invented by Rudolf Bayer in 1972.

Each node in an **Red Black Tree** has an additional **_Color_** bit which is either ![][red]`Red` or ![][black]`Black`, hence the name. To be more precise, **Red Black Tree** is a **_Binary Search Tree_** with the following properties:
1. All nodes with relevant keys are internal nodes and all leaf nodes are sentinels that are used in place of the usual **nullptr** value.
2. Each node is either ![][red]`Red` or ![][black]`Black`. 
3. The root node is ![][black]`Black`.
4. All leaf nodes are ![][black]`Black`.
5. Both children of a ![][red]`Red` node are ![][black]`Black`.
6. Every path from the root to any leaves contains the same number of ![][black]`Black` nodes.

```cpp
#include <iostream>

```

[red]: https://via.placeholder.com/15/f03c15/f03c15.png
[black]: https://via.placeholder.com/15/808080/808080.png
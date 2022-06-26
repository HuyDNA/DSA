## Search Trees
Here, I'll implement _tree-based_ data structures that support the _dictionary_ operations, namely:
* INSERT(element): INSERT _element_ equivalent to some *key* into the tree.
* DELETE(pos): DELETE element at _pos_.
* SEARCH(key): SEARCH for the first element equivalent to _key_ in the tree.

I'll assume that there's a total ordering defined on the set of all possible elements.  
In some cases, I'll implement the following operations:
* DELETE(key): DELETE the first element equivalent to _key_.
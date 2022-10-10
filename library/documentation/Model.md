# Transform hierarchies, models and managed instancing

## Transforms

A transform object is a wrapper around mat4 with extended functionality:

- basic translation, rotation and scaling operations
- a tree-type structure, each child inheriting its parent's transform (commonly used for scene graphs)
- instance management under [Model manager](ModelManager.md)

## Models

A model is a composition of a [mesh](Meshes.md), a [transform](#transforms) and a [model manager](#Managers).

## Managers

### transforms dev comments

each transform stores its own matrix relative to the origin. This speeds up propagation because updates to the parent
don't affect the child's own transform so child's total transform recalc requires 2 MM (O*P) instead of 4 (T*R*S*P);

the deferred approach: transforming the matrix does not update any video memory and does not pull or propagate through
the tree. The client should call the propagate and upload functions when appropriate (before reading transforms which
are relative to others or before rendering). Upload cannot switch between meshes so all propagation must be carried out
before upload (trees are multi-mesh).

+ as opposed to immediate propagation, this solves the problem caused when the child node is moved before the parent. In
  such case, the child node would be calculated twice.
+ as opposed to immediate upload, this allows for a more complex structure where tree nodes of the same state are stored
  contiguously which leverages openGL's mapped buffers using sequential writes.

when modifying the transform, check if it is managed. If so, access the manager's state tracker. Then access the
corresponding render state indices and amend them accordingly.

A state tracker tracks which transforms need to be reuploaded into video memory.
# Transforms

A transform object is a wrapper around mat4 which allows for extended function:

- management under [Model manager](ModelManager.md)
- association with other transforms, forming a transform tree (commonly used for scene graphs)

# Models

A model is a uniquely identifiable object which provides an association between a [mesh](Meshes.md),
a [transform](#transforms). Each model has only its own transform; should any models wish to share a transform, they
must composite a transform tree (this is because a transform can only reference one matrix and managing the second model
would overwrite the reference to the first)

### transforms dev comments

each transform stores its own matrix relative to the origin.

the deferred approach: transforming the matrix does not update any video memory and does not pull or propagate through
the tree. The client should call the propagate and upload functions when appropriate (before reading transforms which
are relative to others or before rendering). Upload cannot switch between meshes so all propagation must be carried out
before upload.

+ as opposed to immediate propagation, this solves the problem caused when the child node is moved before the parent. In
  such case, the child node would be calculated twice.
+ as opposed to immediate upload, this allows for a more complex structure where tree nodes of the same state are stored
  contiguously which leverages openGL's mapped buffers using sequential writes.

when modifying the transform, check if it is managed. If so, access the manager's state tracker. Then access the
corresponding render state indices and amend them accordingly.

A state tracker tracks which transforms need to be reuploaded into video memory.
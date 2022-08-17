# Transforms

A transform object is a wrapper around mat4 which allows for extended function:

- management under [Model manager](ModelManager.md)
- association with other transforms, forming a transform tree (commonly used for scene graphs)

# Models

A model is a uniquely identifiable object which provides an association between a [mesh](Meshes.md),
a [transform](#transforms). Each model has only its own transform; should any models wish to share a transform, they
must composite a transform tree (this is because a transform can only reference one matrix and managing the second model
would overwrite the reference to the first)


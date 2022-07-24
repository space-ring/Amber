# Model Manager

Manages the buffers for mesh instances.

Requirements:

- Once a model is registered, it must be unregistered before it can be registered again at the same manager
- A model can be registered at many managers (scenes) simultaneously
    - actually, a model cannot be. But re-register when scene activates.
- Buffer model instance ids in manager
- Models should not share transformations & transformations should not share matrices
    - instead, a new SharedTransformation should be designed

2 models want to share a transformation:

1) they have the same mesh \
   no point - they'll occupy the same shape and fight for depth


2) they have different meshes\
   they'll be drawn in different renderState calls\
   (unless it's the mesh vertexVBO that is instanced but that's too complex for this rare situation)

So models should not share transformations. But other game objects can:

If a transformation should "attachParent" another transformation, implement FollowingTransformation that indirects the
master's matrix...
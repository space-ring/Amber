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

___
Need a way to upload custom vertex or instance attributes.
Currently, the Director / instance manager class only stores instance transforms* per mesh. What if that mesh has a
specific shader which takes more attributes? Or I want to render one of the instances in a different way?

An idea:
The instance manager also maps mesh / attribute type to a vector of attributes.

- mapping by attribute type forces each mesh to have a unique attribute class. This is not scalable.
- mapping by mesh forces a nested map.

A model instance additionally contains an Attribute.
The attribute linking and data upload is moved from Mesh and Director to the attribute struct. Upload is static for
performance.

```c++
struct SnakeAttribute {
	float colour;
};

struct SnakeAttributeStorage {
	GLvbo* colourBuffer;
	
	void link(GLvao* vao) {
		//bind to vao, bind to vbos, specify attribute pointers
	}
	
	void upload(GLvao* vao, SnakeAttribute* instances, index* indices){
		//bind to vao, map vbos, copy at indices
	}
};
```

Could also have a default Attribute class

```c++
struct DefaultAttribute {
	Transform& instanceTransform;
	
	static void 
}
```

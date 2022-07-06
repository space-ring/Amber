# RenderCall

A RenderCall is a wrapper for any Callable which accepts any arguments and returns a single [Frame](./Frames.md).

The conventional RenderCall retrieves a collection of model instances for a selected [Mesh](./Meshes.md), uses
some [Shader](./Shaders.md) and a [Texture](./Textures.md), buffers required uniforms and issues a render call. It is
the RenderCall's responsibility to use the correct shaders and textures.
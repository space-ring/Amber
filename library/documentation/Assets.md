# Assets

The engine supports these asset types:

- [Shaders](#shaders)
- [Meshes](#meshes)
- [Textures](#textures)

The user can specify which assets are used by an application individually or in
a [manifest](#manifest-syntax) file. Providing a manifest registers the assets at an [Asset Manager](#asset-manager) and
does not instantiate any resource objects. Allocating memory for assets can only be done once a context is made when
requesting assets from Managers by providing tokens. Every asset is given a token unique among others of its type.

## Shaders

Two types of shader assets are used by the engine: shader code and shader programs.

Shader code is a raw code string which has no notion of the type of shader it encodes. A type can be given to a code by
the user when declaring a shader program which allows for reusability.

A shader program is a chain of up to 6 types of shaders. The user specifies which codes constitute a shader.
The codes for each shader are concatenated, compiled and linked into a single program. The compiled, and concatenated
objects exist only temporarily for concatenation and compilation and are deleted right after linking. Thus, unnecessary
work is done when programs share shaders.

## Meshes

A mesh is a wrapper for an OpenGL VAO, containing the mesh VAO, VBO for vertex positions, texture coordinates and
normals (interleaved) and an EBO. The VAO has
attributes at locators:

| Location |     Type      | Attribute                     |
|----------|:-------------:|-------------------------------|
| 0        |     vec3f     | vertex position               |
| 1        |     vec2f     | vertex texture coordinates    |
| 2        |     vec3f     | vertex normal                 |
| 10 - 13  |     mat4f     | __instance__ model transforms |
| 14 - 17  | uniform mat4f | camera transforms             |
| 18 - 21  | uniform mat4f | projection transforms         |

While attributes 0-2 are assigned statically to the VAO given the raw mesh vertex data, attributes 10-13 are assigned
dynamically to allow multiple groups of instances to use the same vertex data. When a specified group of instances
wishes to be drawn using the mesh data, the VAO's attribute pointers are reassigned to the group's VBO (which contains
instance transforms).

## Textures

# Manifest syntax

Assets are grouped by type:

## Shader code

```
shader 
[shader code token] [filepath]
```

## Shader program

```
program
[shader program token] ([shader type : V | TC | TE | G | F | C ] [shader code token]*)+
```

For example:

```
shader
0 vertex.v
1 fragment.f
2 optional.v
program
0 V 0 2 F 1
```

## Mesh

```
mesh
[mesh token] [filepath]
```

## Texture

```
texture
[texture token] [filepath] [format : 2D]
```

# Asset Manager

Provides a table of contents and resource paths for assets. Is also responsible for ownership of asset memory.

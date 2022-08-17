# Meshes

A mesh is a wrapper for an OpenGL VAO. It contains an object's vertex data: position, texture and normal. The VAO has
attributes at locators:

| Location | Attribute                           |
|----------|-------------------------------------|
| 0        | vec3f vertex position               |
| 1        | vec2f vertex texture coordinates    |
| 2        | vec3f vertex normal                 |
| 10 - 13  | mat4f instance model transforms     |
| 14 - 17  | uniform mat4f camera transforms     |
| 18 - 21  | uniform mat4f projection transforms |

The VAO uses a VBO for interleaved vertex data and an EBO.
# Asset Manager

The engine relies on a threaded asset manager.

When a scene requests to use an asset from the engine, the engine delegates the task to the manager. The manager then
returns a pointer to that asset already built in memory or loads the asset from disk using the asset registry.

# Assets rework:

There is an asset manager per engine which holds all asset object that the engine uses during runtime.
The manager owns these assets and they are not created on the heap. They are stored in a token map.

todo: use a lock guard to synchronise assets: post get-from-token

## Shaders

Shader fragments are loaded into raw string representation.
When constructing a program, shaders are compiled by concatenating string fragments. They are then linked, detached and
deleted. This means that a fragment must be recompiled for every program it serves.

## Manifest syntax:

_token type details_

where _type_ and _details_ are:

- fshader
    - filepath
- shader
  - 
- program
- fmesh
- mesh
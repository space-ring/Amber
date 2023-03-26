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

Every asset is assigned a token. Tokens must be unique among the same asset type.

\# this is a comment

Assets are grouped by type\
_type_\
_token details_\
_token details_

where _type_ and _details_ are:

- shader
    - filepath

- program
    - _shaderType_ tokens
    - where _shaderType_ can be, in any order, from:
        - V
        - TC
        - TE
        - G
        - F
        - C

    `5 program V 0 1 2 F 9 TC 6 TE 7`

- mesh
    - filepath
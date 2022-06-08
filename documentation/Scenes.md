# Scenes

A scene is a collection of models, cameras, lights and other components.

Each scene has an independent set of these components and individual world spaces. This means component instances cannot
be shared across scenes. Scenes provide a separation between worlds which may use the same components but different
instances of them.

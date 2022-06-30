# Scenes

A scene is a collection of models, cameras, lights and other components.

Each scene has an independent set of these components' instances and individual world spaces. Scenes provide a separation between worlds which may use the same components but different
instances of them.

To display a scene is to display all [frames](Frames.md) of it: separate pictures computed from render calls saved as
FBOs. Every frame has its own image to display and viewport coordinates.
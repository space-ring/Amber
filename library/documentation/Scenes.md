# Scenes, frames, layers and rendering

A scene is a world. It contains models, camera, lights and other components. Components cannot be shared between scenes,
but scenes may use shared engine resources like textures, shaders and meshes. It is also used to group layers.

Scenes are rendered into frames. Frames are render targets that can be used further within scenes to be sampled from or
displayed directly onthe screen. A scene may consist of multiple frames: imagine a painting application sectioned into
two panels (each has its own frame), the first displaying the image and the second showing available tools and colours.
Because these two penels serve to aid the same task, they are intuitively put into the same scene, though each is
rendered separately. Conceptually, a scene can divide tasks to its layers. In the example, the scene altogether has many
tasks: allow the user to select tools, paint on the canvas, save files, etc. The tasks can be split: the canvas layer
doesn't need to worry about picking tools, only about drawing; the tool layer only allows the user to select the tool.
This design allows greater granularity, abstraction and encapsulation. There is no one-size-fits-all way to use this
design, it is only a guideline. The user is free to declare the components at scene or layer scope and associate scenes
with layers however desired, but generally components that are only used in some layer should be declared there.

To display a scene is to display all [frames](Frames.md) of it: separate pictures computed from renderState calls saved
as FBOs. Every frame has its own image to display and viewport coordinates.

![minecraft inventory screen](assets/ti-sign-ingame.webp)

In this scene, the background is visible and composited with the inventory screen. All user input is confined to the
inventory screen and user game_events pass through to the engine.

The scene altogether defines the textures seen in both layers.

The engine should allow any event to be passed to the back layer if not handled by the front. This means that if
there was a pickable model (still pickable with the top layer active) in the bottom left corner in the back
layer, it should respond to mouse clicks even when the front layer is active. This allows for non-obscuring
layers. This also allows optimisation - the back layer only renders pickable models if the front layer does not
find a picked model instead of all models in the scene being rendered. For this to happen, the back layer must be
able to renderState its own content separately from other layers' content and store and manipulate the renderState image

A scene will have models, a layer layers and frames. There will be 2 passes on the layer layers: forwards and
backwards. On the forward pass (on renderState call), the layers will operate on any frame, compositing them to build
the final images. On the backward pass (on poll call), the layers will handle game_events or pass any unhandled to the
next layer, then finally to the event manager.

One might think: if there are two independent frames, why should one layer operate on them both? (both frames are
rendered for event handling, not necessary) An intuitive solution would be to give each frame its own layerstack.
Suppose now the client wants to darken the whole scene, now every frame needs an additional 'darken' layer. This
creates unnecessary complications. Alternatively, we can create a layer for frame 1 and another for frame 2.

The enforcement of frames as textures seems unfair in the case of rendering to renderbuffers and glviewporting.
Consequently, it's better not to use frames as textures to blit and renderState together unless designed to do so.

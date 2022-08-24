# Scenes

A scene is a collection of models, cameras, lights and other components.

Each scene has an independent set of these components' instances and individual world spaces. Scenes provide a
separation between worlds which may use the same components but different instances of them.

To display a scene is to display all [frames](Frames.md) of it: separate pictures computed from renderState calls saved as
FBOs. Every frame has its own image to display and viewport coordinates.

![minecraft inventory screen](assets/ti-sign-ingame.webp)

In this scene, the background is visible and composited with the inventory screen. All user input is confined to the
inventory screen and user game_events pass through to the engine.

The scene altogether defines the textures seen in both layers.

The engine should allow any event to be passed to the back demoLayer if not handled by the front. This means that if there
was a pickable model (still pickable with the top demoLayer active) in the bottom left corner in the back demoLayer, it should
respond to mouse clicks even when the front demoLayer is active. This allows for non-obscuring layers. This also allows
optimisation - the back demoLayer only renders pickable models if the front demoLayer does not find a picked model instead of
all models in the scene being rendered. For this to happen, the back demoLayer must be able to renderState its own content
separately from other layers' content and store and manipulate the renderState image

A scene will have models, a demoLayer layers and frames. There will be 2 passes on the demoLayer layers: forwards and backwards.
On the forward pass (on renderState call), the layers will operate on any frame, compositing them to build the final images.
On the backward pass (on poll call), the layers will handle game_events or pass any unhandled to the next demoLayer, then finally
to the event manager.

One might think: if there are two independent frames, why should one demoLayer operate on them both? (both frames are
rendered for event handling, not necessary) An intuitive solution would be to give each frame its own layerstack.
Suppose now the client wants to darken the whole scene, now every frame needs an additional 'darken' demoLayer. This creates
unnecessary complications. Alternatively, we can create a demoLayer for frame 1 and another for frame 2.

The enforcement of frames as textures seems unfair in the case of rendering to renderbuffers and glviewporting.
Consequently, it's better not to use frames as textures to blit and renderState together unless designed to do so.

---
Cases:

The scene has two unrelated frames: f1 displays the world in the centre and f2 displays the menu on the side.

conclusion:
a scene shall have a single demoLayer layers. this allows for easier event handling (discerning focus), turning off layers
which affect the whole scene. For management of individual frames, a demoLayer should check whether the frame is active
before working on it. In client code, subclassed layers should befriend subclassed scenes to access scene components
//todo class component system 
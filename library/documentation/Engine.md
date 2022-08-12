# Engine

A lightweight wrapper for GLFW.

Composed of components:

- [Assets](assets/AssetManager.md)
- [Events](EventManager.md)

Later development seeks to allow sharing of components between Engine instances.

## Threading

IMPORTANT: Engine should be created, run and destroyed on the main thread. Preferably, follow the following pattern:

```
int main(){
    ThreadedEngine application("Game Title", x, y, width, height);
    
    // ... add game objects to the engine
    
    application.run();
    return 0;
}
```

In addition, calling ```Engine::getInstance``` should not be called at statically since ```Engine``` is initialised at
runtime.

The threaded engine runs on threads:

- [main] receiving input callbacks (input loop)
- managing the game (game update loop)
- rendering the game (game render loop)

Furthermore, as the client desires, each thread can spawn a subthread to distribute parallel tasks. For example, the
game thread may update sprites' positions and textures simultaneously.

The engine manages deferred event callbacks using a messaging system through a synchronised queue between the threads.
The input loop calls glfw***Events with the corresponding [Stage](./Stages.md) pick methods which place
specific [Events](./Events.md) on the queue. The logic thread pulls game_events off the queue during its update call.

The engine also holds 3 copies of the game state. This is to reduce the lock wait time between updating the game and
rendering it. At the end of the game update loop, the game is copied into a buffer copy. At the beginning of the render
loop, the buffered state is copied into the renderer. This way, only the buffer is synchronised and the lock wait times
are only dependent on the duration of the copying of the game state (as opposed to the duration of updating / rendering
the game).

### So, who's doing the picking?

1) Game thread

   If the renderer is slow, the user can't see what they're picking. But we want the user to be able to pick stale
   objects(?) [And we can! just work on render copy of the game, otherwise work on the live version!](). The renderer is
   always behind the game so the user will never see the live version.

2) Input thread

   If picking takes a long time, game_events are missed.


3) Render thread - when?
    1) Right before rendering scene - useful if mouse is not moving but objects are.

       If the renderer is slow, if the user hovers over another object, the picked object is stale. Now, while the
       renderer is still busy, game thread peels a click event and passes it to the wrong object. So now picking has to
       be synchronised with handling - may as well move handling to game thread.

scenarios:

1) logic thread bottleneck

   The input thread should not put timed game_events into the event queue because this forces game ticks to be timed;
   e.g. now when 1 game tick occurs, the engine calculates the intended number of ticks the user was pressing the key
   for and the character is suddenly given too much velocity because the input thread claims that they've been moving
   the whole time. This can upset other systems like physics calculations. Events should not be timed but synchronised
   with the logic loop.


2) render thread bottleneck

### Event sync

Input thread produces game_events on the queue and the game thread consumes them. For any conditions checked during the
update function, e.g. button held, maintain a structure which adds and removes keys as their respective game_events
occur. Then, during the evaluation, peek at the structure.


___
The render thread is the main thread and polls game_events and processes them in the according order:

1) Event is passed to the front demoLayer of the front scene.
2) If the event is not marked as handled, it is propagated through the rest of the demoLayer layers.
3) If the event is still not marked as handled, it is propagated to the game event queue.
4) The game thread pulls game_events off the queue every update and handles them according to its own handlers.

___
Order:

Render thread:
click handlers require correct picked

picked requires current mouse position

mouse movement should recalculate picked

proposition:

1) get position. if not in box, focused = none, else pick.
2) now if a click occurs, picked is correct
3) if move occurs, it must match glfw get cursor position so no need to repick.
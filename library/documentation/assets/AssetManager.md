# Asset Manager

The engine relies on a threaded asset manager.

When a scene requests to use an asset from the engine, the engine delegates the task to the manager. The manager then
returns a pointer to that asset already built in memory or loads the asset from disk using the asset registry.

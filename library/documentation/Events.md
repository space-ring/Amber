current approach:
Events : static member defining type (index); dynamically polymorphic
Handlers: not polymorphic, stored using (index)

adding handlers: access type through template and add to list
calling handlers: access type through template and call each in list

problems:
1. when adding a handler of Event, the type accessed is of Event
2. when calling a handler of Event, the type accessed is of Event

possible solutions:
1) remove template for calling, use typeid instead of index. This resolves polymorphic types like Event. Now calling a handler of Event gets the correct type. solves 1.
2) remove template for calling, access type through a polymorphic function. solves 1.
3) make handler polymorphic:
   store handlers by handler type
   solves 2.
4) Don't make handlers polymorphic, only events. Consequences:
   1) when adding a handler of event, the type accessed is of event. this is desirable for empty events.
   2) when calling a handler of event, the correct handler type is resolved.
	

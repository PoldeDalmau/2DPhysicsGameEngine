# 2DPhysicsGame
A start to make a game that uses solid balls and polygons, and the physics that governs them. 

Two circles and a rectangle with a restitution factor of 0.9 for collisions with the floor and with each other. 

![circle polygon collision](https://github.com/PoldeDalmau/2DPhysicsGame/assets/73791685/4fe3fce7-b248-461e-9d51-27287a4165b9)


500 white circles are initialized at rest. An extra red circle is initialized with a velocity. Quickly, we see that through collisions, the system begins to behave like an ideal gas. See the velocity (unitless) histogram at the bottom right.

![ideal gas](https://github.com/PoldeDalmau/2DPhysicsGame/assets/73791685/20f82932-d901-44fb-bae7-c6cbd27b8f73)



The engine now can include a squishy car that can be controlled with the keys <kbd>A</kbd> and <kbd>D</kbd> to move, <kbd>Space</kbd> to jump, and <kbd>V</kbd> to view all the spring forces acting.


![squishy car](https://github.com/PoldeDalmau/2DPhysicsGame/assets/73791685/3208c108-1925-4078-8fc1-8a0c4673c035)

TO DO:
- [x] path to sfml libraries needs to be relative
- [x] path to font must also be relative
- [ ] write a little guide on how to set up game! (include compiler, sfml version, .dll in release and debug. Generate Release and Debug folders by running once in each mode. Change bow to x86 (default may be x64)
- [ ] rectangle-rectangle collisions can only have one simultaneous collision point. Extend to two for stability.
- [ ] Refactoring: collision handling class with template for different shapes instead of member functions handling collision for different shapes.
- [ ] Quadtree class that can be constructed with Circle manager.


# 2DPhysicsGame
A start to make a physics engine that uses solid balls and polygons, and the physics that governs them. For visualizations I am using [SFML](https://www.sfml-dev.org/).

Two circles and a rectangle with a restitution factor of 0.9 for collisions with the floor and with each other. 

![circle polygon collision](https://github.com/PoldeDalmau/2DPhysicsGame/assets/73791685/4fe3fce7-b248-461e-9d51-27287a4165b9)


500 white circles are initialized at rest. An extra red circle is initialized with a velocity. Quickly, we see that through collisions, the system begins to behave like an ideal gas. This is validated by the velocity (unitless) histogram at the bottom right converging to the Maxwell-Boltzmann distribution.

![ideal gas](https://github.com/PoldeDalmau/2DPhysicsGame/assets/73791685/20f82932-d901-44fb-bae7-c6cbd27b8f73)



The engine now can include a squishy car that can be controlled with the keys <kbd>A</kbd> and <kbd>D</kbd> to move, <kbd>Space</kbd> to jump, and <kbd>V</kbd> to view all the spring forces acting.


![squishy car](https://github.com/PoldeDalmau/2DPhysicsGame/assets/73791685/3208c108-1925-4078-8fc1-8a0c4673c035)

# Guide
Once you have cloned the project locally, proceed to build it (see below) in Release (and in Debug if you wish). 

![Screenshot 2024-08-16 145542](https://github.com/user-attachments/assets/aed1f984-debe-4f11-8895-c2b39d916f1b)

Make sure to select the x86 config (see screenshot below). Building will generate the Release (and Debug) folders in your project directory. 

![Screenshot 2024-08-16 141446](https://github.com/user-attachments/assets/faaa59d2-e6e5-4488-8d01-0e29e7a6b12b)

Once those folders are generated, copy the .dll files from the sfml_config.zip into the corresponding folders.


# TO DO
- [x] path to sfml libraries needs to be relative
- [x] path to font must also be relative
- [ ] write a little guide on how to set up game!
  - [ ] mention which compiler
  - [x] sfml version
  - [x] .dll in release and debug. Generate Release and Debug folders by running once in each mode. Change bow to x86 (default may be x64)
- [ ] rectangle-rectangle collisions can only have one simultaneous collision point. Extend to two for stability.
- [ ] Refactoring: collision handling class with template for different shapes instead of member functions handling collision for different shapes.
- [ ] Quadtree class that can be constructed with Circle manager.
  - [x] store circles in quadtree with visualization of borders
  - [x] write query function
  - [ ] write update function and restructure tree so it doesn't get destroyed and created every frame.
  - [ ] make templates


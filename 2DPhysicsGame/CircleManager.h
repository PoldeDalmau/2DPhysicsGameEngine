#pragma once
#include "Circle.h"
#include <vector>
#include <cassert>


// Use (void) to silence unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

using std::vector;

class CircleManager {
private:
    float screenWidth;
    float screenHeight;
    vector<float> velocityDistribution;
    sf::RenderWindow& window;
public:
    //bool canJump = false;
    vector<Circle> circles;
    const float deltaTime;
    int numCircles;
    
    CircleManager(float screenWidth, float screenHeight, const float deltaTime, 
        sf::RenderWindow& window) : screenWidth(screenWidth), screenHeight(screenHeight), 
        deltaTime(deltaTime), window(window), numCircles(0)
    {}

    //Circle at(int circleIndex) {
    //    return circles[circleIndex];
    //}

    void AddCircle(Circle circle){
        circles.push_back(circle);  
        numCircles++;
    }

    

    //I want to make a class that inherits from CircleManager and is specific for spring systems:
    //Problems to think about:
    //1. collisions between normal circles and circles that are part of the new class
    //2. maybe turn off collisions within mass system to save on computation power
    void initializeMassSpringSystem(int numCirclesRow, int numCirclesColumn, float latticeConstant, float radius, float particleMass) {
        assert(latticeConstant > 2 * radius, "Lattice Constant too small!");
        int index = 0;
        float xPos = screenWidth - 4 * radius * numCirclesRow;
        float yPos = screenHeight - 4 * radius * numCirclesColumn;
        for (int i = 0; i < numCirclesRow; i++)
            for (int j = 0; j < numCirclesColumn; j++) {
                AddCircle(Circle(numCircles, radius, Point(xPos + i * latticeConstant, yPos + j * latticeConstant), Point(0, 0), particleMass, sf::Color::Blue));
            }
    }

    void handleSpringSystem(int numCirclesRow, int numCirclesColumn, float latticeConstant) {
        //this is awful. I should change it!
        for (int i = 1; i < numCirclesRow - 1; i++)
            for (int j = 1; j < numCirclesColumn - 1; j++) {
                //if (i > 1 && i < numCirclesRow - 1) {


                //SpringLink(i + j, (i - 1) * numCirclesRow + j, latticeConstant); // left neighbor
                //drawLine(window, i + j, (i - 1) * numCirclesRow + j);

                //SpringLink(i + j, i + (j - numCirclesRow), latticeConstant); // above neighbor
                //SpringLink(i + j, (i - 1) + (j - numCirclesRow), latticeConstant); // above left neightbor
                //SpringLink(i + j, (i + numCirclesRow) + j, latticeConstant); // right neighbor
                
                //}
                //if (j > 1 && j < numCirclesColumn - 1) {
                //    SpringLink(i + j, i + (j - numCirclesColumn), latticeConstant);
                //    SpringLink(i + j, i + (j + numCirclesColumn), latticeConstant);
                //}
            }
    
        //0 3 6 9
        //1 4 7 10
        //2 5 8 11
        int i = 0;
        int j = 1;
        SpringLink(i, j, latticeConstant);
        i = 0;
        j = 4;
        SpringLink(i, j, latticeConstant * sqrt(2));
        i = 0;
        j = 3;
        SpringLink(i, j, latticeConstant);
        i = 1;
        j = 2;
        SpringLink(i, j, latticeConstant);
        i = 1;
        j = 4;
        SpringLink(i, j, latticeConstant);
        i = 1;
        j = 3;
        SpringLink(i, j, latticeConstant * sqrt(2));
        i = 1;
        j = 5;
        SpringLink(i, j, latticeConstant * sqrt(2));
        i = 2;
        j = 5;
        SpringLink(i, j, latticeConstant);
        i = 2;
        j = 4;
        SpringLink(i, j, latticeConstant * sqrt(2));
        i = 3;
        j = 4;
        SpringLink(i, j, latticeConstant);
        i = 4;
        j = 5;
        SpringLink(i, j, latticeConstant);

        // second column
        i = 3;
        j = 6;
        SpringLink(i, j, latticeConstant);
        i = 4;
        j = 7;
        SpringLink(i, j, latticeConstant);
        i = 5;
        j = 8;
        SpringLink(i, j, latticeConstant);
        i = 3;
        j = 7;
        SpringLink(i, j, latticeConstant * sqrt(2));
        i = 4;
        j = 6;
        SpringLink(i, j, latticeConstant * sqrt(2));
        i = 4;
        j = 8;
        SpringLink(i, j, latticeConstant * sqrt(2));
        i = 5;
        j = 7;
        SpringLink(i, j, latticeConstant * sqrt(2));

        i = 6;
        j = 7;
        SpringLink(i, j, latticeConstant);
        i = 7;
        j = 8;
        SpringLink(i, j, latticeConstant);
    }

    void AddCirclesMesh(float radius, int numberOfCircles, float latticeConstant) {
        assert(latticeConstant > 2 * radius, "Lattice Constant too small!");
        assert(numberOfCircles * latticeConstant * latticeConstant < screenHeight * screenWidth, "All these circles will not fit");
        assert(numberOfCircles > 0, "Number of Circles must be Natural!");

        int numberOfCirclesPerRow = int(screenWidth/latticeConstant);

        for (int numy = 0; numy < 6; numy++)
        for (int numx = 0; numx < numberOfCirclesPerRow; numx++) {
            //if (numberOfCircles == numx)
            //    break;
            float x = latticeConstant/2 + (numx * latticeConstant);
            float y = latticeConstant/2 + 3 * (numy * latticeConstant);
            float vx = 0;
            float vy = 0;
            //float vx = -100 + 5*numx;
            //float vy = 100 - 5*numx;
            AddCircle(Circle(numCircles, radius, Point(x, y), Point(vx, vy)));
        }
        // make one circle to hit all the circles;
        AddCircle(Circle(numCircles, radius, Point(screenWidth / 2.0f + radius, screenHeight - radius), Point(100, -100), 1, sf::Color::Red));
    }

    // gravity
    // g: acceleration
    void Gravity(float g) {
        for (Circle & circle : circles){
            circle.acceleration.y += (g); // down is up in sfml
        }

    }

    void drawLine(int startIndex, int endIndex)
    {// draw vertices
        sf::VertexArray line(sf::Lines, 2);
        line[0].color = sf::Color::White;
        line[0].position = sf::Vector2f(circles[startIndex].position.x, circles[startIndex].position.y);
        line[1].color = sf::Color::White;
        line[1].position = sf::Vector2f(circles[endIndex].position.x, circles[endIndex].position.y);
        window.draw(line);
    }
    // Binds two circles via a harmonic potential. 
    // eqDistance: Equilibrium position at which the force is zero.
    // k: spring constant

    float SpringForce(){
    }

    void RodLink(int circle1Index, int circle2Index, float eqDistance) {
        assert(circle1Index < circles.size(), "circle index out of bounds");
        assert(circle2Index < circles.size(), "circle index out of bounds");

        float distance = circles[circle1Index].getDistance(circles[circle2Index]);
        float normX = (circles[circle2Index].position.x - circles[circle1Index].position.x) / distance;
        float normY = (circles[circle2Index].position.y - circles[circle1Index].position.y) / distance;
        float displacement = (distance - eqDistance)/2;

        float xDisp = normX * displacement;
        float yDisp = normY * displacement;
        drawLine(circle1Index, circle2Index);

        circles[circle1Index].addxPosition(xDisp/2);
        circles[circle2Index].addxPosition(-xDisp/2);
        circles[circle1Index].addyPosition(yDisp/2);
        circles[circle2Index].addyPosition(-yDisp/2);

    }

    void SpringLink(int circle1Index, int circle2Index, float eqDistance, float k = 1000, float damping = 0, bool draw = true) {
        assert(circle1Index < circles.size(), "circle index out of bounds");
        assert(circle2Index < circles.size(), "circle index out of bounds");

        // draw link
        if(draw)
            drawLine(circle1Index, circle2Index);

        float distance = circles[circle1Index].getDistance(circles[circle2Index]);
        float distanceX = circles[circle2Index].position.x - circles[circle1Index].position.x;
        float distanceY = circles[circle2Index].position.y - circles[circle1Index].position.y;
        float relativeVelocityX = circles[circle2Index].velocity.x - circles[circle1Index].velocity.x;
        float relativeVelocityY = circles[circle2Index].velocity.y - circles[circle1Index].velocity.y;
        // Hook's Law
        // harmonic potential around the equilibrium distance eqDistance
        //float damping = 0.05;
        float forceX = distanceX/distance * (k * (distance - eqDistance) + damping * relativeVelocityX * distanceX / distance);
        float forceY = distanceY/distance * (k * (distance - eqDistance) + damping * relativeVelocityY * distanceY / distance);
        Point acceleration(forceX, forceY);
        // Update velocities according to Hook
        circles[circle1Index].acceleration += acceleration * (1 / circles[circle1Index].getMass());
        circles[circle2Index].acceleration += acceleration * (- 1 / circles[circle2Index].getMass());


    }

    void UpdateAll(bool& canJump) {
        for (Circle& circle : circles) {
            //if(circle.getIndex() != 4)
            //circle.updatePositionEuler(deltaTime);
            circle.updatePostionVerlet(deltaTime);
            //circle.circleFriction(screenHeight);
        }
        CheckCollisionsAndResolve(canJump);
    }
    void CheckCollisionsAndResolve(bool& canJump) {
        bool contact = /*false*/true;
        for (int i = 0; i < circles.size(); i++) {
            circles[i].ResolveWallCollision(screenWidth, screenHeight/*, screenWidth, contact*/);
            //for (int j = i + 1; j < circles.size(); j++) {
            //    if (circles[i].CheckCircleCircleCollision(circles[j]))
            //        circles[i].ResolveCircleCircleCollision(circles[j]);
            //}
        }
        if (contact)
            canJump = true;
        else
            canJump = false;
    }

    void DrawAll() {
        for (Circle& circle : circles)
            circle.Draw(window);
    }

    vector<std::string> getStats() {
        velocityDistribution = vector<float>(circles.size() - 9); // change hardcoded 9 to size of mass spring system

        vector < std:: string > stats(3);
        float particleEnergy,
            internalEnergy = 0;
        int circle_index = 0;
        stats[0] = (std::to_string(numCircles));

        for (Circle& circ : circles)
            if (circ.getIndex() > 9){ // change hardcoded 9 to size of mass spring system
                particleEnergy = circ.velocity.x * circ.velocity.x + circ.velocity.y * circ.velocity.y;
                internalEnergy += particleEnergy;
                velocityDistribution[circle_index] = sqrt(particleEnergy);
                circle_index++;
            }

        stats[1] = std::to_string((internalEnergy));
        stats[2] = std::to_string(velocityDistribution.back());
        return stats;
    }

    void makeVelocityHistogram(int numbins, float expectedMaxVelocity) {
        int velocity_index = 0;
        vector<int> binoccupancy(numbins, 0);
        float binwidth = expectedMaxVelocity / numbins;

        for (float value : velocityDistribution) {
            int binIndex = value/binwidth;
            if (value > expectedMaxVelocity) binIndex = numbins-1;
            binoccupancy[binIndex] ++;
        }
        float rectangleWidth = 5;
        for (int i = 0; i < numbins; i++) {
            sf::RectangleShape rectangle(sf::Vector2f(rectangleWidth, binoccupancy[i] * 2));
            rectangle.setFillColor(sf::Color::White);
            rectangle.setPosition(sf::Vector2f(1015 + i * rectangleWidth, screenHeight - binoccupancy[i] * 2));
            window.draw(rectangle);
        }


    }
};
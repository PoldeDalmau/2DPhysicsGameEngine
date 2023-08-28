//Probably obsolete


#pragma once
#include "Circle.h" // Include the Circle class header
class CollisionHandler {
public:
    //CollisionHandler();
    // Add methods for collision detection and response
    bool CheckCircleCircleCollision(Circle& circle, Circle& othercircle) {
        float distanceX = othercircle.getxPosition() - circle.getxPosition();
        float distanceY = othercircle.getyPosition() - circle.getyPosition();
        float distanceSquared = distanceX * distanceX + distanceY * distanceY;

        float sumOfRadii = circle.getRadius()  + othercircle.getRadius();
        float sumOfRadiiSquared = sumOfRadii * sumOfRadii;

        return distanceSquared <= sumOfRadiiSquared;
    }

    // first, checks displaces circles if they are overlapping by half of the the overlap for each. Then, assuming equal mass, it resolves the collision with momentum and energy conservation.
    void ResolveCircleCircleCollision(Circle& circle, Circle& othercircle) {
        //first, move circles out of overlap:
        float distanceX = othercircle.getxPosition() - circle.getxPosition();
        float distanceY = othercircle.getyPosition() - circle.getyPosition();
        float distanceMagnitude = std::sqrt(distanceX * distanceX + distanceY * distanceY);
        float overlap = (circle.getRadius() + othercircle.getRadius()) - distanceMagnitude;


        if (overlap > 0.0f) {
            // Calculate the displacement needed to move out of overlap
            float displacementX = (overlap / distanceMagnitude) * distanceX;
            float displacementY = (overlap / distanceMagnitude) * distanceY;

            // Move the circles to non-overlapping positions
            circle.addxPosition(-displacementX * 0.5f);
            circle.addyPosition(-displacementY * 0.5f);
            

            othercircle.addxPosition(displacementX * 0.5f);
            othercircle.addyPosition(displacementY * 0.5f);

        }

        // then, handle the collision assuming equal mass:

        float normalX = distanceX / distanceMagnitude;
        float normalY = distanceY / distanceMagnitude;

        float relativeVelocityX = othercircle.getxVelocity() - circle.getxVelocity();
        float relativeVelocityY = othercircle.getyVelocity() - circle.getyVelocity();

        float dotProduct = relativeVelocityX * normalX + relativeVelocityY * normalY;

        float impulse = (2.0f * dotProduct) / (1.0f + 1.0f); // Assuming equal masses

        circle.addxVelocity(impulse * normalX);
        circle.addyVelocity(impulse * normalY);

        othercircle.addxVelocity(-impulse * normalX);
        othercircle.addyVelocity(- impulse * normalY);
    }
    void ResolveCircleWallCollision(Circle& circle, float screenWidth, float screenHeight) {
        if (circle.getxPosition() < circle.getRadius()) {
            circle.setX(circle.getRadius());
            circle.flipxVelocity();
        }
        else if (circle.getxPosition() + circle.getRadius() > screenWidth){
            circle.setX(screenWidth - circle.getRadius());
            circle.flipxVelocity();
        }

        if (circle.getyPosition() < circle.getRadius()) {
            circle.flipyVelocity();
            circle.setY(circle.getRadius());
        }
        else if (circle.getyPosition() + circle.getRadius() > screenHeight) {
            circle.flipyVelocity();
            circle.setY(screenHeight - circle.getRadius());
        }
    }


private:
    // Add private helper methods if needed
};


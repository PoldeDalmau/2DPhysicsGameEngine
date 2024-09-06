#include "CircleManager.h"
#include "Quadtree.h"

void CircleManager::UpdateAllQT(QuadTree* qt) {
    /*for (Circle& circle : circles) {
    }*/
    //qt;
    //cBounds.drawBoundary(window);
    for (Circle& circle : circles) {
        circle.updatePostionVerlet(deltaTime);
        circle.ResolveWallCollision(screenWidth, screenHeight);
        AABB cBounds(circle.position, circle.radius * 2, 2 * circle.radius);
        cBounds.drawBoundary(window);
        vector<Circle*> found = (*qt).query(cBounds, circle, window);
        for (auto& c : found) {
            if (circle.CheckCircleCircleCollision(*c)){
                //circle.color = sf::Color::Red;
                //c->color = sf::Color::Red;
                circle.ResolveCircleCircleCollision(*c);
            }
        }
    }
}
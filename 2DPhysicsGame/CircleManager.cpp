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
        AABB cBounds(circle.position, circle.radius, circle.radius);
        //cBounds.drawBoundary(window);
        vector<Circle*> found = (*qt).query(cBounds, circle);
        for (auto& c : found) {
            if (circle.CheckCircleCircleCollision(*c)){
                //circle.color = sf::Color::Red;
                //c->color = sf::Color::Red;
                circle.ResolveCircleCircleCollision(*c);
            }
        }
    }

    //for (int i = 0; i < circles.size(); i++) {
    //    vector<Circle*> found = qt.query(AABB(circles[i].position, circles[i].radius, circles[i].radius));
    //    circles[i].ResolveWallCollision(screenWidth, screenHeight/*, screenWidth, contact*/);
    //    for (auto& c : found) {
    //        if (circles[i].CheckCircleCircleCollision(*c))
    //            circles[i].ResolveCircleCircleCollision(*c);
    //    }
    //}
}
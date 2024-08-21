#include "CircleManager.h"
#include "Quadtree.h"

void CircleManager::UpdateAllQT(QuadTree* qt, AABB& cBounds) {
    for (Circle& circle : circles) {
        circle.updatePostionVerlet(deltaTime);
    }
    //qt;
    //cBounds.drawBoundary(window);
    for (Circle& circle : circles) {
        circle.ResolveWallCollision(screenWidth, screenHeight);
        //(*cBounds).center;// = circle.position;
        //cBounds->width = circle.radius;
        //cBounds->height = circle.radius;
        //AABB cBounds = AABB(circle.position, circle.radius, circle.radius);
        //vector<Circle*> found = (*qt).query(cBounds);
        /*for (auto& c : found) {

            if (circle.CheckCircleCircleCollision(*c))
                circle.ResolveCircleCircleCollision(*c);
        }*/
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
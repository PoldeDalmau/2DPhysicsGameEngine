#pragma 
#include "Point.h"
#include "CircleManager.h"
#include <SFML/Graphics.hpp>


// Axis-aligned bounding box stored as a center with half-dimensions
  // to represent the boundaries of this quad tree
class AABB {
    public:
        Point center;
        float width;
        float height;
        
        AABB(Point center,
            float width,
            float height
        )
            :
            center(center),
            width(width),
            height(height)
        {}

        void drawLine(Point start, Point end, sf::RenderWindow& window)
        {// draw vertices
            sf::VertexArray line(sf::Lines, 4);
            line[0].color = sf::Color::White;
            line[0].position = sf::Vector2f(start.x, start.y);
            line[1].color = sf::Color::White;
            line[1].position = sf::Vector2f(end.x, end.y);
            window.draw(line);
        }

        void drawBoundary(sf::RenderWindow& window) {
            Point sw = center + Point(-width, height);
            Point se = center + Point(width, height);
            Point nw = center + Point(-width, -height);
            Point ne = center + Point(width, -height);

            drawLine(sw, se, window);
            drawLine(se, ne, window);
            drawLine(ne, nw, window);
            drawLine(nw, sw, window);
        }

        bool contains(Circle circle) {
            return (circle.position.x > center.x - width &&
                circle.position.x < center.x + width &&
                circle.position.y < center.y + height &&
                circle.position.y > center.y - height);
        }
};


class QuadTree {
private:
    int nodeCapacity;
    vector<Circle> cman;
    vector<Circle*> points;
    sf::RenderWindow& window;
public:
    bool divided = false;
    AABB boundary;

    QuadTree(
        AABB boundary,
        vector<Circle> cman, 
        int nodeCapacity, 
        sf::RenderWindow& window)
        :
        boundary(boundary),
        cman(cman),
        nodeCapacity(nodeCapacity),
        window(window)
    {
        boundary.drawBoundary(window);
        for (Circle& circle : cman) {
            insert(circle);
        }
    }


    void insert(Circle circle) {

        if (!boundary.contains(circle)) {
            return;
        }


        if (points.size() < nodeCapacity) {
            points.push_back(&circle);
        }
        else {
            if (!divided) {
                subdivide();
                divided = true;
            }
        }
    }

    void subdivide() {
        Point c = boundary.center;
        float w = boundary.width;
        float h = boundary.height;
        AABB swBoundary(c + Point(-w / 2, h / 2), w / 2, h / 2);
        AABB seBoundary(c + Point(w / 2, h / 2), w / 2, h / 2);
        AABB nwBoundary(c + Point(-w / 2, -h / 2), w / 2, h / 2);
        AABB neBoundary(c + Point(w / 2, -h / 2), w / 2, h / 2);

        QuadTree sw(swBoundary, cman, nodeCapacity, window);
        QuadTree se(seBoundary, cman, nodeCapacity, window);
        QuadTree nw(nwBoundary, cman, nodeCapacity, window);
        QuadTree ne(neBoundary, cman, nodeCapacity, window);
    }


    // Children
    //QuadTree* northWest;
    //QuadTree* northEast;
    //QuadTree* southWest;
    //QuadTree* southEast;
};
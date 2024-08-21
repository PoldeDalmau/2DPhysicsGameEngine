#pragma
#include "Circle.h"
#include "Point.h"
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

    bool intersects(AABB range) {
        return (
            range.center.x - range.width > center.x + width ||
            range.center.x + range.width > center.x - width ||
            range.center.y - range.height > center.y + height ||
            range.center.y + range.height > center.y - height
            );
    }
};

class QuadTree {
private:
    int nodeCapacity;
    vector<Circle>& cman;
    sf::RenderWindow& window;
    vector<Circle*> points;
public:
    bool divided = false;
    AABB boundary;
    std::unique_ptr<QuadTree> SouthEast;
    std::unique_ptr<QuadTree> SouthWest;
    std::unique_ptr<QuadTree> NorthEast;
    std::unique_ptr<QuadTree> NorthWest;

    QuadTree(
        AABB boundary,
        vector<Circle>& cman,
        int nodeCapacity,
        sf::RenderWindow& window)
        :
        boundary(boundary),
        cman(cman),
        nodeCapacity(nodeCapacity),
        window(window)
    {
        boundary.drawBoundary(window);

        vector<Circle>::iterator iter;
        for (Circle& iter : cman) {
            insert(iter);
        }
    }


    void insert(Circle &circle) {

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

        SouthEast = std::make_unique<QuadTree>(seBoundary, cman, nodeCapacity, window);
        SouthWest = std::make_unique<QuadTree>(swBoundary, cman, nodeCapacity, window);
        NorthEast = std::make_unique<QuadTree>(neBoundary, cman, nodeCapacity, window);
        NorthWest = std::make_unique<QuadTree>(nwBoundary, cman, nodeCapacity, window);
    }

    vector<Circle*> query(AABB range) {
        vector<Circle*> found;
        if (!boundary.intersects(range)) {
            return found;
        }
        else {        
            if (divided) {
                vector<Circle*> se = (*SouthEast).query(range);
                vector<Circle*> sw = (*SouthWest).query(range);
                vector<Circle*> ne = (*NorthEast).query(range);
                vector<Circle*> nw = (*NorthWest).query(range);

                found.insert(found.end(), se.begin(), se.end());
                found.insert(found.end(), sw.begin(), sw.end());
                found.insert(found.end(), ne.begin(), ne.end());
                found.insert(found.end(), nw.begin(), nw.end());
            }
            else {
                for (Circle* p : points) {
                    if (range.contains(*p)) {
                        found.push_back(p);
                    }
                }
            }

            return found;
        }
    }
};
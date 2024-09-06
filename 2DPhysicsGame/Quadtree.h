#pragma once
//#include <SFML/Graphics.hpp>
//#include "Point.h"
//#include "Circle.h"
//#include <vector>

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

    void drawLine(Point start, Point end, sf::RenderWindow& window, sf::Color col)
    {// draw vertices
        sf::VertexArray line(sf::Lines, 4);
        line[0].color = col;
        //line[0].color = sf::Color::White;
        line[0].position = sf::Vector2f(start.x, start.y);
        line[1].color = col;
        //line[1].color = sf::Color::White;
        line[1].position = sf::Vector2f(end.x, end.y);
        window.draw(line);
    }

    void drawBoundary(sf::RenderWindow& window, sf::Color col = sf::Color::White) {
        Point sw = center + Point(-width, height);
        Point se = center + Point(width, height);
        Point nw = center + Point(-width, -height);
        Point ne = center + Point(width, -height);

        drawLine(sw, se, window, col);
        drawLine(se, ne, window, col);
        drawLine(ne, nw, window, col);
        drawLine(nw, sw, window, col);
    }

    bool contains(Circle circle) {
        return (
            circle.position.x > center.x - width &&
            circle.position.x < center.x + width &&
            circle.position.y < center.y + height &&
            circle.position.y > center.y - height);
        //circle.position.x + circle.radius > center.x - width &&
        //    circle.position.x - circle.radius < center.x + width &&
        //    circle.position.y - circle.radius < center.y + height &&
        //    circle.position.y + circle.radius > center.y - height);
    }

    bool intersects(AABB range) {
        if (
            range.center.x - range.width > center.x + width ||
            range.center.x + range.width > center.x - width ||
            range.center.y - range.height > center.y + height ||
            range.center.y + range.height > center.y - height
            ) {
        }
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
    AABB& boundary;
public:
    bool divided = false;
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
        if (!divided) {
            if (points.size() < nodeCapacity) {
                points.push_back(&circle);
                return;
            }
            subdivide();
        }
    }

    void subdivide() {
        divided = true;
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
        points.clear();
    }

    
    vector<Circle*> query(AABB range, const Circle& current, sf::RenderWindow &window){
        vector<Circle*> found;
        if (!boundary.intersects(range)) {
            return found;
        }
        else {
            if (divided) {
                vector<Circle*> se = (*SouthEast).query(range, current, window);
                vector<Circle*> sw = (*SouthWest).query(range, current, window);
                vector<Circle*> ne = (*NorthEast).query(range, current, window);
                vector<Circle*> nw = (*NorthWest).query(range, current, window);

                found.insert(found.end(), sw.begin(), sw.end());
                found.insert(found.end(), se.begin(), se.end());
                found.insert(found.end(), ne.begin(), ne.end());
                found.insert(found.end(), nw.begin(), nw.end());
            }
            else {
                for (Circle* p : points) {
                    if (&current != p && range.contains(*p)) {
                        found.push_back(p);
                    }
                }
            }

            return found;
        }
    }
};

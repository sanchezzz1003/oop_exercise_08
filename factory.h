#ifndef FACTORY_H
#define FACTORY_H 1

#include "figures.h"

class Factory {
public:
    std::shared_ptr<Figure> FigureCreate(FigureType type) const {
        std::shared_ptr<Figure> res;
        if (type == Pent) {
            res = std::make_shared<Pentagon>();
        } else if (type == Rhomb) {
            res = std::make_shared<Rhombus>();
        } else if (type == Trapeze) {
            res = std::make_shared<Trapezoid>();
        }
        return res;
    }

    std::shared_ptr<Figure> FigureCreate(FigureType type, Vertex *vertices, int id) const {
        std::shared_ptr<Figure> res;
        if (type == Pent) {
            res = std::make_shared<Pentagon>(vertices[0], vertices[1], vertices[2], vertices[3], vertices[4], id);
        } else if (type == Rhomb) {
            res = std::make_shared<Rhombus>(vertices[0], vertices[1], vertices[2], vertices[3], id);
        } else if (type == Trapeze) {
            res = std::make_shared<Trapezoid>(vertices[0], vertices[1], vertices[2], vertices[3], id);
        }

        return res;
    }
};

#endif //FACTORY_H

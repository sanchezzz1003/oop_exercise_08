#ifndef FIGURES_H
#define FIGURES_H 1

#include <iostream>
#include <fstream>
#include <utility> // for pair
#include <memory>
#include <cmath>
#include <stdexcept>

enum FigureType {
    Pent,
    Rhomb,
    Trapeze
};

using Vertex = std::pair<double, double>;


class Figure {
public:
    virtual double Area() const = 0;
    virtual Vertex Center() const = 0;
    virtual std::ostream &Print(std::ostream &out) const = 0;
    virtual void Serialize(std::ofstream &os) const = 0;
    virtual void Deserialize(std::ifstream &is) = 0;
    virtual int getId() const = 0;
    virtual ~Figure() = default;
};



    Vertex Get_Center(const Vertex *vertices, int n) {
        double x = 0, y = 0;
        for (int i = 0; i < n; i++) {
            x += vertices[i].first;
            y += vertices[i].second;
        }
        return std::make_pair(x / n, y / n);
    }

    Vertex operator-(const Vertex &p1, const Vertex &p2) {
        return {p1.first - p2.first, p1.second - p2.second};
    }

    bool collinear(const Vertex &a, const Vertex &b, const Vertex &c, const Vertex &d){
        return (b.second-a.second)*(d.first-c.first) - (d.second-c.second)*(b.first-a.first) <= 1e-9;
    }

    bool perpendic(const Vertex &a, const Vertex &b, const Vertex &c, const Vertex &d){
        using vect = std::pair<double, double>;
        vect AC = c-a;
        vect BD = d-b;
        double dotProduct = AC.first*BD.first + AC.second*BD.second;
        if(dotProduct <= 1e-9 && dotProduct >= -1e-9) return true;
        else return false;
    }


    double dist(const Vertex &a, const Vertex &b){
        return sqrt( ((b.first - a.first) * (b.first - a.first)) + ((b.second - a.second) * (b.second - a.second)));
    }

    bool operator==(const Vertex &a, const Vertex &b){
        return (a.first == b.first) && (a.second == b.second);
    }


std::ostream& operator<<(std::ostream &o, const Vertex &p){
    o << "<" << p.first << ", " << p.second << ">";
    return o;
}


bool isNumber(const std::string& s){
    return !s.empty() && s.find_first_not_of("-.0123456789") == std::string::npos;
}


std::istream& operator>>(std::istream &is, Vertex &p){
    std::string checker;

    is >> checker;
    if(isNumber(checker) == false){
        throw std::overflow_error("Is not a number");
    }
    p.first = static_cast<double>(std::stod(checker));

    is >> checker;
    if(isNumber(checker) == false){
        throw std::overflow_error("Is not a number");
    }
    p.second = static_cast<double>(std::stod(checker));

    return is;
}




class Pentagon : public Figure {
    int Id;
    Vertex *vertices;
public:
    Pentagon() : Id{0}, vertices{new Vertex[5]} {
        for (int i = 0; i < 5; i++){
            vertices[i] = std::make_pair(0,0);
        }
    }

    Pentagon(Vertex &a, Vertex &b, Vertex &c, Vertex &d, Vertex &e, int id) :
    Id{id}, vertices{new Vertex[5]} {
        if (a == b || a == c || b == c || a == d || a == e) {
            throw std::logic_error("The entered coordinates of the vertices do not belong to the Pentagon.");
        } else {
            vertices[0] = a;
            vertices[1] = b;
            vertices[2] = c;
            vertices[3] = d;
            vertices[4] = e;
        }
    }
    const double PI = acos(-1.0);
    ~Pentagon() override {
        delete [] vertices;
        vertices = nullptr;
    }

    Vertex Center() const override {
        return Get_Center(vertices,5);
    }

    double Area() const override {
        auto AB = dist(vertices[0], vertices[1]);
        return 5/2*AB*sqrt(pow(AB/(2*sin(PI/5)),2)-AB*AB/4);
    }

    std::ostream &Print(std::ostream &out) const override{
        out << "Id: " << Id << "\n";
        out << "Figure: Pentagon\n";
        out << "Coords:\n";
        for (int i = 0; i < 5; i++) {
            out << vertices[i] << "\n";
        }
        return out;
    }

    void Serialize(std::ofstream &os) const override{
        FigureType type = Pent;
        os.write((char *) &type, sizeof(type));
        os.write((char *) &Id, sizeof(Id));
        for (int i = 0; i < 5; i++) {
            os.write((char*) &(vertices[i].first), sizeof(vertices[i].first));
            os.write((char*) &(vertices[i].second), sizeof(vertices[i].second));
        }
    }

    void Deserialize(std::ifstream &is) override {
        is.read((char *) &Id, sizeof(Id));
        for (int i = 0; i<5; i++) {
            is.read((char *) &(vertices[i].first), sizeof(vertices[i].first));
            is.read((char *) &(vertices[i].second), sizeof(vertices[i].second));
        }
    }

    int getId() const override {
        return Id;
    }
};

class Trapezoid : public Figure {
    int Id;
    Vertex *vertices;
public:
    Trapezoid() : Id{0}, vertices{new Vertex[4]} {
        for (int i = 0; i < 4; i++){
            vertices[i] = std::make_pair(0,0);
        }
    }

    Trapezoid(Vertex &a, Vertex &b, Vertex &c, Vertex &d, int id) :
    Id{id}, vertices{new Vertex[4]} {
        auto AB = dist(a, b);
        auto AD = dist(a, d);
        if (a == b || a == c || b == c || a == d || 
            (!collinear(a, d, c, b) && !collinear(a, b, d, c))) {
            throw std::logic_error("The entered coordinates of the vertices do not belong to the trapezoid.");
        } else {
            vertices[0] = a;
            vertices[1] = b;
            vertices[2] = c;
            vertices[3] = d;
        }
    }

    ~Trapezoid() override {
        delete [] vertices;
        vertices = nullptr;
    }

    Vertex Center() const override {
        return Get_Center(vertices,4);
    }

    double Area() const override {
        double x1,x2,x3,x4, y1,y2,y3,y4;
        x1 = vertices[0].first; y1 = vertices[0].second;
        x2 = vertices[1].first; y2 = vertices[1].second;
        x3 = vertices[2].first; y3 = vertices[2].second;
        x4 = vertices[3].first; y4 = vertices[3].second;

        auto area = ( (x1*y2-x2*y1)+(x2*y3-x3*y2)+(x3*y4-x4*y3) ) / 2;
        return std::abs(area);
    }

    std::ostream &Print(std::ostream &out) const override{
        out << "Id: " << Id << "\n";
        out << "Figure: Trapezoid\n";
        out << "Coords:\n";
        for (int i = 0; i < 4; i++) {
            out << vertices[i] << "\n";
        }
        return out;
    }

    void Serialize(std::ofstream &os) const override{
        FigureType type = Trapeze;
        os.write((char *) &type, sizeof(type));
        os.write((char *) &Id, sizeof(Id));
        for (int i = 0; i < 4; i++) {
            os.write((char *) &(vertices[i].first),sizeof(vertices[i].first));
            os.write((char *) &(vertices[i].second),sizeof(vertices[i].second));
        }
    }

    void Deserialize(std::ifstream &is) override {
        is.read((char *) &Id, sizeof(Id));
        for (int i = 0; i < 4; i++) {
            is.read((char *) &(vertices[i].first),sizeof(vertices[i].first));
            is.read((char *) &(vertices[i].second),sizeof(vertices[i].second));
        }
    }

    int getId() const override {
        return Id;
    }
};

class Rhombus: public Figure {
    int Id;
    Vertex *vertices;
public:
    Rhombus() : Id{0}, vertices{new Vertex[4]} {
        for (int i = 0; i < 4; i++){
            vertices[i] = std::make_pair(0,0);
        }
    }
    Rhombus(Vertex &a, Vertex &b, Vertex &c, Vertex &d, int id) :
    Id{id}, vertices{new Vertex[4]} {
        auto AB = dist(a, b);
        auto AD = dist(a, d);
        auto BC = dist(b,c);
        auto CD = dist(c,d);
        if (a == b || a == c || b == c || a == d || b == d || c == d ||
            !(AB == AD) || !(CD == BC) || !(AB == CD) ||
            !perpendic(a,b,c,d) || !collinear(a, b, c, d)
            || !collinear(a, d, c, b) ) {
            throw std::logic_error("The entered coordinates of the vertices do not belong to the Rhombus.");
        } else {
            vertices[0] = a;
            vertices[1] = b;
            vertices[2] = c;
            vertices[3] = d;
        }
    }

    ~Rhombus() override {
        delete [] vertices;
        vertices = nullptr;
    }

    Vertex Center() const override {
        return Get_Center(vertices,4);
    }

    double Area() const override {
        double AC = dist(vertices[0],vertices[2]);
        double DB = dist(vertices[3],vertices[1]);
        return (AC*DB) /2;
    }

    std::ostream &Print(std::ostream &out) const override{
        out << "Id: " << Id << "\n";
        out << "Figure: Trapezoid\n";
        out << "Coords:\n";
        for (int i = 0; i < 4; i++) {
            out << vertices[i] << "\n";
        }
        return out;
    }

    void Serialize(std::ofstream &os) const override{
        FigureType type = Rhomb;
        os.write((char *) &type, sizeof(type));
        os.write((char *) &Id, sizeof(Id));
        for (int i = 0; i < 4; i++) {
            os.write((char *) &(vertices[i].first),sizeof(vertices[i].first));
            os.write((char *) &(vertices[i].second),sizeof(vertices[i].second));
        }
    }

    void Deserialize(std::ifstream &is) override {
        is.read((char *) &Id, sizeof(Id));
        for (int i = 0; i < 4; i++) {
            is.read((char *) &(vertices[i].first),
                    sizeof(vertices[i].first));
            is.read((char *) &(vertices[i].second),
                    sizeof(vertices[i].second));
        }
    }

    int getId() const override {
        return Id;
    }
};


#endif //FIGURES_H

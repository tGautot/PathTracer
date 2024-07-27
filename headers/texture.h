#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>

#include "color.h"


class texture {
public:
    virtual ~texture() = default;

    virtual color value(double u, double v, point3& p) const = 0;
};

class solid_color_tex : public texture {
public:
    color col;
    solid_color_tex(): col(color(0,0,0)) {}
    solid_color_tex(double r, double g, double b): col(color(r,g,b)) {}
    solid_color_tex(color& c): col(c) {}

    color value(double u, double v, point3& p) const {
        return col;
    }
};


class checker_tex : public texture {
public:
    std::shared_ptr<texture> t1, t2;
    double sz = 1.0;
    checker_tex(): t1(std::make_shared<solid_color_tex>(0,0,0)), t2(std::make_shared<solid_color_tex>(0,0,0)) {}
    checker_tex(double r, double g, double b): t1(std::make_shared<solid_color_tex>(0,0,0)), t2(std::make_shared<solid_color_tex>(r,g,b)) {}
    checker_tex(color& c): t1(std::make_shared<solid_color_tex>(0,0,0)), t2(std::make_shared<solid_color_tex>(c)) {}
    checker_tex(color& c1, color& c2): t1(std::make_shared<solid_color_tex>(c1)), t2(std::make_shared<solid_color_tex>(c2)) {}
    checker_tex(color c1, color c2): t1(std::make_shared<solid_color_tex>(c1)), t2(std::make_shared<solid_color_tex>(c2)) {}
    //checker_tex(texture& tex1, texture& tex2): t1(make_shared<texture>(tex1)), t2(make_shared<texture>(tex2)) {}

    void set_size(double s){
        sz = fmax(0.001, s);
    }

    color value(double u, double v, point3& p) const {
        int x = int(p.x()/sz), y = int(p.y()/sz), z = int(p.z()/sz);
        if( (x+y+z) % 2 )
            return t1->value(u, v, p);

        return t2->value(u,v,p);
    }
};

#endif
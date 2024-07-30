#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>

#include "color.h"
#include "pt_image.h"
#include "perlin.h"

class texture {
public:
    virtual ~texture() = default;

    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color_tex : public texture {
public:
    color col;
    solid_color_tex(): col(color(0,0,0)) {}
    solid_color_tex(double r, double g, double b): col(color(r,g,b)) {}
    solid_color_tex(const color& c): col(c) {}

    color value(double u, double v, const point3& p) const {
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

    color value(double u, double v, const point3& p) const {
        int x = int(p.x()/sz), y = int(p.y()/sz), z = int(p.z()/sz);
        if( (x+y+z) % 2 )
            return t1->value(u, v, p);

        return t2->value(u,v,p);
    }
};

class image_tex : public texture {
private:
    pt_image img;

public:

    image_tex(const char* filename): img(filename) {}

    color value(double u, double v, const point3& p) const override{
        if (img.height() <= 0) return color(0,1,1);

        int x = u * img.width();
        int y = v * img.height();

        const unsigned char* col_bytes = img.pixel_data(x, y);

        double color_scale = 1.0/255.0;
        color col = color(col_bytes[0], col_bytes[1], col_bytes[2])*color_scale;
        return col;

    }

};

class noise_tex : public texture {
private:
    perlin noise;
    double scale;

public:
    noise_tex(double scale = 1): noise(), scale(scale){}

    color value(double u, double v, const point3& p) const override {
        double noiseVal = noise.noise(p*scale);
        return color(1, 1, 1) * 0.5 * (1.0 + noiseVal);
    }
};


#endif
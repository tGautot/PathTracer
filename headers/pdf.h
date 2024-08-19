#ifndef PDF_H
#define PDF_H

#include "vec3.h"

template <typename T>
class pdf {
public:
    virtual double val(const T& x) const = 0;
    virtual T generate() const = 0;
};


class uniform_sphere_pdf : public pdf<vec3>{
public:

    uniform_sphere_pdf(){}

    double val(const vec3& x) const override {
        return 1/(4*PI);
    }
    vec3 generate() const override {
        return vec3::random_on_unit_sphere();
    };
};

class uniform_hemisphere_pdf : public pdf<vec3>{
private:
    vec3 n;
public:

    uniform_hemisphere_pdf(const vec3& normal) : n(normal){}

    double val(const vec3& x) const override {
        return 1/(2*PI);
    }
    vec3 generate() const override {
        return vec3::random_on_hemisphere(n);
    };
};

class cosine_hemisphere_pdf : public pdf<vec3>{
private:
    vec3 n;
    double angle = PI/2.0;
    double sin_squared = 1.0;
public:

    cosine_hemisphere_pdf(const vec3& normal) : n(normal){}
    cosine_hemisphere_pdf(const vec3& normal, double angle) : n(normal), angle(angle){
        double sine = std::sin(angle*PI/180.0);
        sin_squared = sine*sine;
    }

    double val(const vec3& x) const override {
        double cos_theta = dot(n, x.normalized());
        return cos_theta < 0.0 ? 0.0 : cos_theta/(PI*sin_squared);
    }
    vec3 generate() const override {
        //std::clog << "Generating direciton via cosine pdf of angle " << angle << std::endl;
        vec3 v = n + vec3::random_on_unit_sphere() * (angle*2.0/PI);
        if(v.near_zero())
            v = n;
        return v;
    };
};

class uniform_hittable_pdf : public pdf<vec3> {
private:
    shared_ptr<hittable> obj;
    point3 orig;
public:

    uniform_hittable_pdf(shared_ptr<hittable> obj, const point3& p) : obj(obj), orig(p){}

    double val(const vec3& dir) const override {
        return obj->pdf_value(orig, dir);
    }

    vec3 generate() const override {
        return obj->random_point_towards(orig) - orig;
    };
};


template<typename T>
class point_pdf : public pdf<T> {
private:
    T p;
    
public:
    point_pdf(T p): p(p){}

    double val(const T& v) const override {
        if(v == p) return 1.0;
        return 0;
    }

    T generate() const override {
        return p;
    };


};

template<typename T>
class binary_pdf : public pdf<T> {
private:
    T v1, v2;
    double p1;

public:
    binary_pdf(T v1, T v2, double p1): v1(v1), v2(v2), p1(p1){}

    double val(const T& v) const override {
        if(v == v1) return p1;
        if(v == v2) return 1-p1;
        return 0;
    }

    T generate() const override {
        if(randDouble() < p1) return v1;
        return v2;
    };


};


#include <vector>
#include <utility> // std::pair
template<typename T>
class linear_comb_pdf : public pdf<T> {
public:
    std::vector<std::pair<shared_ptr<pdf<T>>, double>> pdfs; // pdfs and their weight
    std::vector<double> cumsum;

    linear_comb_pdf(){}

    void add(shared_ptr<pdf<T>> p, double w){
        pdfs.push_back({p, w});
        double v = 0;
        if(cumsum.size() > 0)
            v = cumsum.back();
        cumsum.push_back(v+w);

    }

    double val(const T& x) const override {
        if(cumsum.size() == 0)
            exit(1);
        double pdfsum = 0.0;
        for(auto pdfpair : pdfs){
            pdfsum += pdfpair.first->val(x) * pdfpair.second;
        }
        return pdfsum / cumsum.back();
        
    }
    T generate() const override {
        double v = randDouble() * cumsum.back();
        
        auto itt = std::lower_bound(cumsum.begin(), cumsum.end(), v);
        int index = itt - cumsum.begin();
#ifdef SIMPLE_DEBUG
        std::clog << "Cumulated pdf total weight is " << cumsum.back() << " and drew " << v << std::endl;
        std::clog << "Generating value from " << index << "th/" << pdfs.size() << " pdf " << std::endl;
#endif
        return pdfs[index].first->generate();
    };

};

#endif




#ifndef PDF_H
#define PDF_H

#include "vec3.h"

template <typename T>
class pdf {
public:
    virtual double val(const T& x) const = 0;
    virtual T generate() const = 0;
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
public:

    cosine_hemisphere_pdf(const vec3& normal) : n(normal){}

    double val(const vec3& x) const override {
        double cos_theta = dot(n, x.normalized());
        return cos_theta < 0.0 ? 0.0 : cos_theta/PI;
    }
    vec3 generate() const override {
        vec3 v = n + vec3::random_on_unit_sphere();
        if(v.near_zero())
            v = n;
        return v;
    };
};


#include <vector>
#include <utility> // std::pair
template<typename T>
class linear_comb_pdf : public pdf<T> {
    std::vector<std::pair<pdf<T>, double>> pdfs; // pdfs and their weight
    std::vector<double> cumsum;
    int size;

    linear_comb_pdf():size(0){}

    void add(pdf<T> p, double w){
        pdfs.push_back({p, w});
        double v = 0;
        if(size > 0)
            v = cumsum.back();
        cumsum.push_back(v+w);
    }

    double val(const T& x) const override {
        if(size == 0)
            exit(1);
        double pdfsum = 0.0;
        for(auto pdfpair : pdfs){
            pdfsum += pdfpair.first.val() * pdfpair.second;
        }
        return pdfsum / cumsum.back();
        
    }
    T generate() const override {
        double v = randDouble() * cumsum.back();
        auto itt = std::lower_bound(cumsum.begin(), cumsum.end(), v);
        int index = itt - cumsum.begin();
        return pdfs[index].first.generate();
    };

};

#endif




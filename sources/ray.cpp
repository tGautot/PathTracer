#include "ray.h"

std::ostream& operator<<(std::ostream& out, const ray& r){
    return out << "RAY from "<< r.origin() << " towards " << r.direction();   
}
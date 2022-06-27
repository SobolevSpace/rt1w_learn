#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3{
public:
    double e[3];

public:
    vec3():e{0, 0, 0} {}
    vec3(double e0, double e1, double e2):e{e0, e1, e2} {}

    double x() const {return e[0];}
    double y() const {return e[1];}
    double z() const {return e[2];}

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]);}
    double operator[](int i) const {
        return e[i];
    }
    double& operator[](int i) {
        return e[i];
    }

    vec3& operator+=(const vec3& v) {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double length_sqr() const {
        return e[0]*e[0]+e[1]*e[1]+e[2]*e[2];
    }

    double length() const {
        return sqrt(this->length_sqr());
    }

    inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double t_min, double t_max) {
        return vec3(random_double(t_min, t_max), random_double(t_min, t_max), random_double(t_min, t_max));
    }

    bool near_zero() const {
        const double eps = 1e-8;
        return fabs(e[0]) <= eps && fabs(e[1]) <= eps && fabs(e[2]) <= eps;
    }

};

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' '<<v.e[1]<<' '<<v.e[2];
}

inline vec3 operator+(const vec3& v, const vec3& u) {
    return vec3(v.e[0]+u.e[0], v.e[1]+u.e[1], v.e[2]+u.e[2]);
}

inline vec3 operator-(const vec3& v, const vec3& u) {
    return vec3(v.e[0]-u.e[0], v.e[1]-u.e[1], v.e[2]-u.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0]*v.e[0], u.e[1]*v.e[1], u.e[2]*v.e[2]);
}

inline vec3 operator*(const vec3& u, double t) {
    return vec3(t*u.e[0], t*u.e[1], t*u.e[2]);
}

inline vec3 operator*(double t, const vec3& u) {
    return vec3(t*u.e[0], t*u.e[1], t*u.e[2]);
}

inline vec3 operator/(const vec3& u, double t) {
    return u*(1/t);
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0]*v.e[0]+u.e[1]*v.e[1]+u.e[2]*v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1]*v.e[2]-u.e[2]*v.e[1],
    u.e[2]*v.e[0]-u.e[0]*v.e[2],
    u.e[0]*v.e[1]-u.e[1]*v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v/v.length();
}

inline vec3 random_in_unit_sphere() {
        while (true) {
            vec3 p = vec3::random(-1, 1);
            if (p.length_sqr() <= 1) return p;
        }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3& normal) {
    vec3 ret = random_in_unit_sphere();
    if (dot(ret, normal) < 0) ret =  -ret;
    return ret;
}

vec3 random_in_unit_disk() {
    while (true) {
        vec3 ret = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (ret.length_sqr() <= 1) return ret;
    }
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v, n)*n;
}

vec3 refract(const vec3& v_unit, const vec3& n_unit, double etai_over_etat) {
    double cos_theta = fmin(dot(-v_unit, n_unit), 1.0);
    vec3 r_out_prep = etai_over_etat * (v_unit + cos_theta * n_unit);
    vec3 r_out_parallel = -sqrt(fabs(1-r_out_prep.length_sqr())) * n_unit;
    return r_out_parallel + r_out_prep;
}

using point3 = vec3;
using color = vec3;
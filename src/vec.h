#pragma once 

#include <cmath>

template<typename T> struct Vec2;

template<typename T>
struct Vec3 {
	union {
		struct {
			T x;
			T y;
			T z;
		};
		T raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
	Vec3(T x, T y) : x(x), y(y), z(0) {}
	template<typename U> Vec3(Vec3<U> vec) : x((T)vec.x), y((T)vec.y), z((T)vec.z) {}
	template<typename U> Vec3(Vec2<U> vec) : x((T)vec.x), y((T)vec.y), z(0) {}

	inline Vec3<T> operator -(const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
	inline Vec3<T> operator -() const { return Vec3<T>(-x, -y, -z); }
	T& operator[](int index) { return raw[index]; }
};
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

template<typename T>
struct Vec2 {
	union {
		struct {
			T x;
			T y;
		};
		T raw[2];
	};
	Vec2() : x(0), y(0) {}
	Vec2(T x, T y) : x(x), y(y) {}
	template<typename U> Vec2(Vec2<U> vec) : x((T)vec.x), y((T)vec.y) {}
	template<typename U> Vec2(Vec3<U> vec) : x((T)vec.x), y((T)vec.y) {}

	inline Vec2<T> operator -(const Vec2<T> &v) const { return Vec2<T>(x - v.x, y - v.y); }
	inline Vec2<T> operator -() const { return Vec2<T>(-x, -y); }
	T& operator[](int index) { return raw[index]; }
};
typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

template<typename T>
Vec3<T> cross(Vec3<T> a, Vec3<T> b) {
	return Vec3<T>(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

template<typename T>
float dot(Vec3<T> a, Vec3<T> b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
Vec3<T> normalize(Vec3<T>& vec) {
	float len = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return Vec3<T>(vec.x / len, vec.y / len, vec.z / len);
}

template<typename T>
Vec3f barycentric(T x, T y, Vec3f a, Vec3f b, Vec3f c) {
	Vec3f v0 = Vec3f(c.x - a.x, b.x - a.x, a.x - x);
	Vec3f v1 = Vec3f(c.y - a.y, b.y - a.y, a.y - y);
    Vec3f crossProduct = cross(v0, v1);
    /* `pts` and `P` has integer value as coordinates
       so `abs(u[2])` < 1 means `u[2]` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    if (std::abs(crossProduct[2]) < 1) {
        return Vec3f(-1.f, 1.f, 1.f);
    } else {
        return Vec3f(
            1.f - (crossProduct[0] + crossProduct[1]) / crossProduct[2],
            crossProduct[1] / crossProduct[2],
            crossProduct[0] / crossProduct[2]
        );
    }
}

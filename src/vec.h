#pragma once 

#include <cmath>

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
	inline Vec3<T> operator -(const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
	inline Vec3<T> operator -() const { return Vec3<T>(-x, -y, -z); }
	T& operator[](int index) { return raw[index]; }
};

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
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

template<typename T>
Vec3<T> normalize(Vec3<T>& vec) {
	float len = std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	return Vec3<T>(vec.x / len, vec.y / len, vec.z / len);
}

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

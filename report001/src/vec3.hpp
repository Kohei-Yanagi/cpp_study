//3次元ベクトルクラス
#pragma once

#include<cmath>
#include<cassert>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ベクトルクラス
class vec3 {
public:
    //コンストラクタ　
	vec3() : x( 0.0 ), y( 0.0 ), z( 0.0 ) {}
    vec3( const double _x, const double _y, const double _z ) : x( _x ), y( _y ), z( _z ) {}
    explicit vec3( const double s ) : x( s ), y( s ), z( s ) {}
    //算術演算子
    vec3 operator+( const vec3& rhs ) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
    vec3 operator-( const vec3& rhs ) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
    vec3 operator*( const vec3& rhs ) const { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
    vec3 operator/( const vec3& rhs ) const { return { x / rhs.x, y / rhs.y, z / rhs.z }; }
    vec3 operator*( const double scale ) const { return { scale * x, scale * y, scale * z }; }
    vec3 operator/( const double scale ) const { return { x / scale, y / scale, z / scale }; }

    vec3& operator+=( const vec3& rhs ) { return x += rhs.x, y += rhs.y, z += rhs.z, *this; }
    vec3& operator-=( const vec3& rhs ) { return x -= rhs.x, y -= rhs.y, z -= rhs.z, *this; }
    vec3& operator*=( const vec3& rhs ) { return x *= rhs.x, y *= rhs.y, z *= rhs.z, *this; }
    vec3& operator*=( const double scale ) { return x *= scale, y *= scale, z *= scale, *this; }
    vec3& operator/=( const vec3 &rhs ) { return x /= rhs.x, y /= rhs.y, z /= rhs.z, *this; }
    vec3& operator/=( const double scale ) { return x /= scale, y /= scale, z /= scale, *this; }
    vec3 operator-() const { return { - x, - y, - z }; }
    friend vec3 operator*( const double scale, const vec3 &v ) { return { scale * v.x, scale * v.y, scale * v.z }; }
    //アクセッサ
	double operator[]( const int dim ) const { return assert( 0 <= dim && dim < 3 ), ( dim == 0 ) ? x : ( dim == 1 ) ? y : z; }
    //内積/L2ノルム/正規化/外積
    friend double dot( const vec3& lhs, const vec3& rhs ) { return ( lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z ); }
    friend double norm( const vec3& v ) { return sqrt( v.x * v.x + v.y * v.y + v.z * v.z ); }
    friend vec3 normalize( const vec3& v ) { const double l = norm( v ); assert( std::abs( l ) > 1e-10 ); return { v.x / l, v.y / l, v.z / l }; }
    friend vec3 cross( const vec3& lhs, const vec3& rhs ) { return { lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x }; }
    //ベクトルの要素が有限か否か
    friend bool isfinite( const vec3& v ) { return std::isfinite( v.x ) && std::isfinite( v.y ) && std::isfinite( v.z ); }
    //最小・最大
    friend vec3 min( const vec3& lhs, const vec3& rhs ){ return { std::min( lhs.x, rhs.x ), std::min( lhs.y, rhs.y ), std::min( lhs.z, rhs.z ) }; }
    friend vec3 max( const vec3& lhs, const vec3& rhs ){ return { std::max( lhs.x, rhs.x ), std::max( lhs.y, rhs.y ), std::max( lhs.z, rhs.z ) }; }

    double x, y, z;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef vec3 col3;


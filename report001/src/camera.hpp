//カメラクラス
#pragma once

#include<numbers>
#include"vec3.hpp"
#include"ray.hpp"

//(ピンホール)カメラクラス
class camera {
public:

    //コンストラクタ
    camera( const vec3& eye, const vec3& ref, const vec3& up, const int width, const int height, const double tanfovy = tan( 45.0 / 360 * std::numbers::pi ) ) : m_eye( eye ), m_width( width ), m_height( height ), m_p( 2 * tanfovy / m_height ) {
        m_w = normalize( m_eye - ref );
        m_u = normalize( cross( up, m_w ) );
        m_v = cross( m_w, m_u );
    }

    //視点を返す
    [[ nodiscard ]] vec3 eye() const { return m_eye; }
    //スクリーンの横の画素数
    [[ nodiscard ]] int width() const { return m_width; }
    //スクリーンの縦の画素数
    [[ nodiscard ]] int height() const { return m_height; }

    //スクリーン座標(x,y)を通るレイを生成
    [[ nodiscard ]] ray generate_ray( const double x, const double y ) const {
        double cx = (x - m_width / 2.0) * m_p;
        double cy = (m_height / 2.0 - y) * m_p;
        vec3 dir = (cx * m_u) + (cy * m_v) - m_w;
        return ray( m_eye, normalize( dir ) );
    }

private:
    vec3    m_eye;      //視点(カメラ)のワールド座標
    int     m_width;    //スクリーンの横の画素数
    int     m_height;   //スクリーンの縦の画素数
    double  m_p;        //一ピクセルの大きさ
    vec3    m_u;        //視点座標系
    vec3    m_v;
    vec3    m_w;
};

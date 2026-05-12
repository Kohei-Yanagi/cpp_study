//光線(レイ)クラス
#pragma once

#include"vec3.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//　光線(レイ)クラス
class ray {
public:
    //コンストラクタ
    ray( const vec3& o, const vec3& d ) : m_o( o ), m_d( d ) {}

    [[ nodiscard ]] vec3 o() const { return m_o; } //レイの始点を返す
    [[ nodiscard ]] vec3 d() const { return m_d; } //レイの単位方向ベクトルを返す

private:
    vec3 m_o;   //レイの始点
    vec3 m_d;   //レイの単位方向ベクトル
};


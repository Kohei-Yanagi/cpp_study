//交差情報を保持するクラス
#pragma once

#include"material.hpp"

//前方宣言
namespace material { class material; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 交点情報
class intersection {
public:

	//コンストラクタ
    intersection() : m_p(), m_n(), m_wo(), m_u(), m_v(), m_material( nullptr ) {}

    //コンストラクタ
    intersection( const vec3& p, const vec3& n, const vec3& wo, const double u, const double v, std::shared_ptr< material::material > mat ) : m_p( p ), m_n( n ), m_wo( wo ), m_u( u ), m_v( v ), m_material( std::move( mat ) ) {}

    //交点の位置を返す
    [[ nodiscard ]] vec3 p() const { return m_p; }

    //交点の法線を返す
    [[ nodiscard ]] vec3 n() const { return m_n; }

    //交点計算時のレイの方向を返す
    [[ nodiscard ]] vec3 wo() const { return m_wo; }

    //テクスチャ座標を返す
    [[ nodiscard ]] double u() const { return m_u; }

    //テクスチャ座標を返す
    [[ nodiscard ]] double v() const { return m_v; }

    //シーン中の球と交差するか
    [[ nodiscard ]] bool is_valid() const { return m_material != nullptr; }

    //交点が無効か (シーン中の球と交差しない，あるいは材質が無効)
    [[ nodiscard ]] bool is_invalid() const { return m_material == nullptr; }

    //交点が発光材質(光源)かどうか
    [[ nodiscard ]] bool is_emissive() const { return assert( is_valid() ), m_material->is_emissive(); }

    //交点が発光材質(光源)の場合，光源の放射輝度を返す
    [[ nodiscard ]] col3 Le( const vec3& wi ) const { return ( is_emissive() ) ? m_material->bsdf( m_wo, m_n, m_u, m_v )->f( wi ) : col3(); }

    //交点のBRDFを生成
    [[ nodiscard ]] std::unique_ptr< bsdf > bsdf() const { return m_material->bsdf( m_wo, m_n, m_u, m_v ); }

private:

	//交点
    vec3 m_p;
    //交点の法線
	vec3 m_n;
	//レイの出射方向
    vec3 m_wo;
    //交点のテクスチャ座標
    double m_u;
    double m_v;
    //交点の材質へのポインタ
    std::shared_ptr< material::material > m_material;
};

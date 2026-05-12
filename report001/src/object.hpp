//シーンを構成する物体クラス
#pragma once

#include"vec3.hpp"
#include"ray.hpp"
#include"material.hpp"
#include"utility.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//物体の基底クラス
class object {
public:
    object() = default;
    virtual ~object() = default;
    [[ nodiscard ]] virtual double intersect( const ray& r ) const = 0;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//球クラス
class sphere : public object {
public:
    //コンストラクタ
    sphere( const double r, const vec3& center, const std::shared_ptr< material::material >& mtl ) : m_center( center ), m_radius( r ), m_area( 4.0 * std::numbers::pi * m_radius * m_radius ), m_material( mtl ) {}

    //交差判定　交差した場合レイの始点からの距離を返す　交差しない場合0を返す
    [[ nodiscard ]] double intersect( const ray& r ) const override {
        vec3 p = m_center - r.o();
        const double b = dot( p, r.d() );
        const double det = b * b - dot( p, p ) + m_radius * m_radius;
        if( det >= 0.0 ) {
            const double sqrt_det = sqrt( det );
            const double t1 = b - sqrt_det;
            const double t2 = b + sqrt_det;
            if( t1 > eps ) {
                return t1;
            }
            if( t2 > eps ) {
                return t2;
            }
        }
        return 0.0;
    }

    //球の中心座標を返す
    [[ nodiscard ]] vec3 center() const { return m_center; }
    //球の半径を返す
    [[ nodiscard ]] double radius() const { return m_radius; }
    //球の面積を返す
    [[ nodiscard ]] double area() const { return m_area; }
    //球の材質へのポインタを返す
    [[ nodiscard ]] std::shared_ptr< material::material > material() const { return m_material; }
    //球の放射束を返す 発光材質のみ非零
    [[ nodiscard ]] double flux() const { return ( m_material->is_emissive() ) ? area() * luminance( dynamic_cast< material::light_material* >( m_material.get() )->radiant_exitance() ) : 0.0; }

private:
	//球の中心座標
    vec3                                    m_center;
    //球の半径
    double                                  m_radius;
    //球の面積
    double                                  m_area;
    //球の材質へのポインタ
    std::shared_ptr< material::material >   m_material;

    static constexpr double eps = 1e-6;
};


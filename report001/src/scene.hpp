//シーンクラス
#pragma once

#include<numbers>
#include<numeric>
#include<vector>
#include"object.hpp"
#include"intersection.hpp"
#include"ray.hpp"
#include"material.hpp"

//シーン情報を格納したクラス
class scene {
public:

    //コンストラクタ
    explicit scene( std::vector< sphere >&& obj ) : m_object( std::move( obj ) ) {
        //materialが光源の球をm_lightにコピー
        std::copy_if( m_object.begin(), m_object.end(), std::back_inserter( m_light ),[]( const sphere &s ) { return ( s.material()->is_emissive() ); } );
        m_flux = std::accumulate( m_object.begin(), m_object.end(), 0.0, []( double d, const sphere &s ) { return d + s.flux(); } );
        //assert( m_light.size() > 0 ); //光源が0個だとエラー
    }

    //レイと物体(球)との交点計算
    [[ nodiscard ]] intersection intersect( const ray& r ) const {
        double mint = std::numeric_limits< double >::infinity();
        bool hit = false;
        size_t id = 0;
        for( size_t i = 0, n = m_object.size(); i < n; ++i ) {
            if( const double t = m_object[ i ].intersect( r ); t < mint && t > 0.0 ) {
                mint = t;
                hit = true;
                id = i;
            }
        }

        if( hit ) {
            const auto p = r.o() + mint * r.d();
            const auto n = normalize( p - m_object[ id ].center() );
            const auto th = acos( std::clamp( n.y, - 1.0, 1.0 ) );
            const auto ph = atan2( n.x, n.z );
            const auto u = th * std::numbers::pi;
            const auto v = ( ph + std::numbers::pi ) / ( 2.0 * std::numbers::pi );
            return { p, n, - r.d(), u, v, m_object[ id ].material() };
        }
        return {};
    }

    // ray rが距離d以内で物体と交差するかどうかを判定　(交点は求めない, shadow ray用)
    [[ nodiscard ]] bool intersect( const ray& r, const double d ) const {
        constexpr double eps = 1e-10;
        return std::ranges::any_of( m_object, [ & ]( const auto& s ){ const double t = s.intersect( r ); return ( 0.0 < t && t < d - eps ); } );
    }

    //シーン中の光源をサンプリング， pdf_aはサンプリング確率(area measure)
    intersection sample_light( random_number_generator& rng, double& pdf_a ) const {
        //光源を一つサンプリング(現状一様サンプリング，光束に比例させたほうがいい)
        const sphere& light = m_light[ rng.rand( 0, m_light.size() ) ];
        //光源の選択確率
        const double pdf_l = 1.0 / m_light.size();

        //uniform sampling of sphere
        const double costh = 1.0 - 2.0 * rng.rand();
        const double sinth = sqrt( std::max( 0.0, 1.0 - costh * costh ) );
        const double ph = 2.0 * std::numbers::pi * rng.rand();
        const double cosph = cos( ph );
        const double sinph = sin( ph );
        const vec3 p = vec3( sinth * cosph, sinth * sinph, costh );

        //pdf = 1 / A (A : 面積) * pdf_l (光源の選択確率)
        pdf_a = 1.0 / light.area() * pdf_l;
        return { light.center() + light.radius() * p, p, {}, 0.0, 0.0, light.material() };
    }

    //シーン中の光源と交差する方向を明示的にサンプリング pdf_wは確率密度(solid angle measure)
    intersection sample_light( const intersection &x, random_number_generator &rng, double &pdf_w ) const {
        //光源を一つサンプリング(現状一様サンプリング，光束に比例させたほうがいい)
        const sphere& light = m_light[ rng.rand( 0, m_light.size() ) ];
        //光源の選択確率
        const double pdf_l = 1.0 / m_light.size();
        //ここを実装してください
        pdf_w = 0.0;
        return {};
    }

private:
	//シーンを構成する物体
    std::vector< sphere >   m_object;
    //シーンを構成する光源
    std::vector< sphere >   m_light;
    //光源の放射束
    double                  m_flux;

};

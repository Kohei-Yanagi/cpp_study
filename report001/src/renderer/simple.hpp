#pragma once

#include"renderer.hpp"

//交点の色でピクセルを塗りつぶすシンプルなレンダラ
namespace simple {

class renderer : public ::renderer {

public:

    //コンストラクタ
    renderer() = default;
    //デストラクタ
    ~renderer() override = default;

    //画像生成
    [[ nodiscard ]] std::vector< float > render( const scene& scene, const camera& camera ) const final {
        std::vector< float > pixel( 3 * camera.width() * camera.height() );
        for( int y = 0, h = camera.height(); y < h; ++y ) {
            for( int x = 0, w = camera.width(); x < w; ++x ) {
                col3 L {};

                const ray r = camera.generate_ray( x + 0.5, y + 0.5 );
                const auto isect = scene.intersect( r );

                if( isect.is_valid() ) { //レイとシーンが交差する場合
                    const auto bsdf = isect.bsdf();
                    L = bsdf->f( vec3() );
                }
                pixel[ 3 * ( y * w + x ) + 0 ] = std::isfinite( L.x ) ? L.x : 0.0;
                pixel[ 3 * ( y * w + x ) + 1 ] = std::isfinite( L.y ) ? L.y : 0.0;
                pixel[ 3 * ( y * w + x ) + 2 ] = std::isfinite( L.z ) ? L.z : 0.0;
            }
        }
        return pixel;
    }

};


}


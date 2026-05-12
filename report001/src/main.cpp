#include<iostream>
#include<numbers>
#include<cassert>
#include<random>
#include<limits>
#include<algorithm>
#include<chrono>
#include<memory>
#include<array>
#include<print>

#include"camera.hpp"
#include"scene.hpp"

#include"renderer/simple.hpp"


//fopen_s系のエラーを回避
#pragma warning( disable : 4996 )

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"

//画像サイズと画角
constexpr size_t image_width    = 512;
constexpr size_t image_height   = 512;
constexpr double fov            = 40.0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void save_png( const size_t width, const size_t height, const std::vector< float >& pixel, const std::string& filename )
{
    std::vector< unsigned char > pixels( 3 * width * height );
    std::transform( pixel.begin(), pixel.end(), pixels.begin(), []( const float x ){ return static_cast< unsigned char >( std::clamp( 255 * powf( std::max( x, 0.f ), 1.f / 2.2f ), 0.f, 255.f ) ); } );
    stbi_write_png( filename.c_str(), int( width ), int( height ), 3, pixels.data(), 0 );
}

//メイン関数
int main( int argc, char ** argv )
{
	//シーンの設定
	std::vector< sphere > spheres = { sphere( 1e5, vec3( 0.0, - 1e5 - 4.0, 0 ), std::make_shared< material::diffuse >( std::make_shared< constant_texture< col3 > >( col3( 0.5 ) ) ) ),                 //床
									  sphere( 1e5, vec3(  1e5 + 5, -3, 0     ), std::make_shared< material::diffuse >( std::make_shared< constant_texture< col3 > >( col3( 0.8, 0.2, 0.1 ) ) ) ),       //右
									  sphere( 1e5, vec3( -1e5 - 5, -3, 0     ), std::make_shared< material::diffuse >( std::make_shared< constant_texture< col3 > > ( col3( 0.1, 0.2, 0.8 ) ) ) ),      //左
									  sphere( 1e5, vec3( 0.0, -3, -1e5 - 15  ), std::make_shared< material::diffuse >( std::make_shared< constant_texture< col3 > >( col3( 0.8 ) ) ) ),                 //奥
									  sphere( 1e5, vec3( 0.0, 1e5 + 5, 0     ), std::make_shared< material::diffuse >( std::make_shared< constant_texture< col3 > >( col3( 0.6 ) ) ) ),                 //上
									  sphere( 1.0, vec3(- 2.0, -2.9, - 10.0  ), std::make_shared< material::diffuse >( std::make_shared< constant_texture< col3 > >( col3( 0.7 ) ) ) ),                 //球
									  sphere( 1.0, vec3(  2.0, -2.9, - 10.0  ), std::make_shared< material::glossy  >( std::make_shared< constant_texture< double > >( 0.1 ), std::make_shared< constant_texture< col3 > >( col3( 0.8 ) ) ) ),
									  sphere( 0.5, vec3( 0.0,   4.0, - 10.0  ),std::make_shared< material::light_material >( std::make_shared< constant_texture< col3 > >( col3( 100.0 ) ) ) )          //光源
	};
	//視点，注視点，上方向ベクトル
	const vec3 eye { + 0.0, + 0.5, + 5.00 };
    const vec3 ref { + 0.0, + 0.0, + 0.00 };
	const vec3 up { 0.0, 1.0, 0.0 };
    const scene scn( std::move( spheres ) );
    const camera cmr( eye, ref, up, image_width, image_height, tan( fov / 2 * std::numbers::pi / 180.0 ) );

	const simple::renderer render;
	const auto pixel = render.render( scn, cmr );

    save_png( image_width, image_height, pixel, "image.png" );

}

//テクスチャクラス
#pragma once

#include<memory>
#include"../image/stb_image.h"

#include"utility.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//テクスチャ基底クラス
template< typename T > class texture {
public:
	//デストラクタ
	virtual ~texture() = default;

	//サンプリング
	virtual T operator()( const double u, const double v ) const = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//(テクスチャ座標によらず)一定値を返すテクスチャ
template< typename T > class constant_texture : public texture< T > {
public:

	//コンストラクタ
	explicit constant_texture( const T& val ) : m_val( val ) {
	}

	//デストラクタ
	~constant_texture() override = default;

	//サンプリング
	T operator()( const double u, const double v ) const override {
		return m_val;
	}

private:
	T   m_val;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//画像テクスチャ
class image_texture : public texture< col3 > {
public:

	//コンストラクタ
	explicit image_texture( const std::string &filename ) : m_width(), m_height(), m_img( nullptr ) {
		int channel;
		const unsigned char *img = stbi_load( filename.c_str(), &m_width, &m_height, &channel, 0 );
		assert( m_width > 0 && m_height > 0 && channel == 3 && img != nullptr );
		m_img = std::make_unique< col3 [] >( m_width * m_height );
		for( int i = 0, n = m_height * m_width; i < n; ++i ) {
			m_img[ i ] = col3( img[ 3 * i + 0 ] / 255.0, img[ 3 * i + 1 ] / 255.0, img[ 3 * i + 2 ] / 255.0 );
		}
		stbi_image_free( ( void* ) img );
	}

	//デストラクタ
	~image_texture() override = default;

	//サンプル
	col3 operator()( const double u, const double v ) const override {
		const int x0 = std::clamp( static_cast< int >( u * ( m_width - 1  ) ), 0, m_width - 1  );
		const int y0 = std::clamp( static_cast< int >( v * ( m_height - 1 ) ), 0, m_height - 1 );
		const int x1 = std::clamp( x0 + 1, 0, m_width - 1  );
		const int y1 = std::clamp( y0 + 1, 0, m_height - 1 );
		const double dx = std::clamp( u * ( m_width - 1  ) - x0, 0.0, 1.0 );
		const double dy = std::clamp( v * ( m_height - 1 ) - y0, 0.0, 1.0 );
		return ( 1.0 - dx ) * ( 1.0 - dy ) * m_img[ y0 * m_width + x0 ]
		     + ( 0.0 + dx ) * ( 1.0 - dy ) * m_img[ y0 * m_width + x1 ]
		     + ( 1.0 - dx ) * ( 0.0 + dy ) * m_img[ y1 * m_width + x0 ]
		     + ( 0.0 + dx ) * ( 0.0 + dy ) * m_img[ y1 * m_width + x1 ];
	}

private:
	//画素データ
	std::unique_ptr< col3 [] >  m_img;
	//画像の縦の画素数
	int                         m_width;
	//画像の横の画素数
	int                         m_height;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//手続き型テクスチャ
template< typename T, typename Func > class procedural_texture : public texture< T >
{
public:
	//コンストラクタ
	procedural_texture( const T &val, Func &func ) : m_val( val ), m_func( func ) {}

	//サンプリング
	T operator()( const double u, const double v ) const
	{
		return m_val * m_func( u, v );
	}

private:
	T m_val;
	Func m_func;
};

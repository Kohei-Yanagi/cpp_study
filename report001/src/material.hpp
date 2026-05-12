//材質クラス
#pragma once

#include"bsdf.hpp"
#include"texture.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//材質名前空間
namespace material {

//材質の基底クラス
class material {
public:

	enum type {
		non_emissive    = 0 << 1,
		emissive        = 1 << 1,
	};

    //コンストラクタ
    material() : m_type( non_emissive ) {}

    //コンストラクタ
    explicit material( enum type type ) : m_type( type ) {}

    //デストラクタ
    virtual ~material() = default;

    //BSDFを生成し返す wo, nはワールド座標系での出射方向，法線
    [[ nodiscard ]] virtual std::unique_ptr< bsdf > bsdf( const vec3& wo, const vec3& n, double, double ) const = 0;

    bool is_emissive() { return ( m_type != 0 ); }

private:
	type    m_type;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//拡散反射材質
class diffuse : public material {
public:

	//コンストラクタ
    explicit diffuse( std::shared_ptr< texture< col3 > > kd_map ) : material(), m_kd_map( std::move( kd_map ) ) {}

    //lambertian bsdfの生成
    [[ nodiscard ]] std::unique_ptr< class bsdf > bsdf( const vec3& wo, const vec3& n, const double u, const double v ) const override {
        return std::make_unique< lambertian >( wo, n, ( *m_kd_map )( u, v ) );
    }

private:
	//アルベドマップ
    std::shared_ptr< texture< col3 > >  m_kd_map;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//光沢反射材質
class glossy : public material {
public:

	//コンストラクタ
    glossy( std::shared_ptr< texture< double > > alpha, std::shared_ptr< constant_texture< col3 > > f0 ) : material(), m_alpha_map( std::move( alpha ) ), m_f0_map( std::move( f0 ) ) {}

    //bsdf::microfacet brdfの生成
    [[ nodiscard ]] std::unique_ptr< class bsdf > bsdf( const vec3& wo, const vec3& n, const double u, const double v ) const override {
        return std::make_unique< microfacet >( wo, n, ( *m_alpha_map )( u, v ), ( *m_f0_map )( u, v ) );
    }

private:
	//GGXのパラメータのテクスチャ
	std::shared_ptr< texture< double > >        m_alpha_map;
	//フレネル項の反射率パラメータのテクスチャ
    std::shared_ptr< constant_texture< col3 > > m_f0_map;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//拡散光源
class light_material : public material {
public:

	//コンストラクタ
    explicit light_material( std::shared_ptr< constant_texture< col3 > > Me ) : material( type::emissive ), m_Me( std::move( Me ) ) {}

    //bsdf::diffuse_lightの生成
    [[ nodiscard ]] std::unique_ptr< class bsdf > bsdf( const vec3& wo, const vec3& n, const double u, const double v ) const override {
        return std::make_unique< diffuse_light >( wo, n, ( *m_Me )( u, v ) );
    }

    [[ nodiscard ]] col3 radiant_exitance() const { return ( *m_Me )( 0.0, 0.0 ); }

private:
	//放射発散度(exitant)
	std::shared_ptr< constant_texture< col3 > > m_Me;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ガラスクラス
class glass : public material {
public:

	//コンストラクタ
    explicit glass( std::shared_ptr< texture< double > > eta_map ) : material(), m_eta_map( std::move( eta_map ) ) {}

    //完全鏡面反射透過BRDF(bsdf::specular_reflection_transmission)の生成
    [[ nodiscard ]] std::unique_ptr< class bsdf > bsdf( const vec3& wo, const vec3& n, const double u, const double v ) const override
    {
        return std::make_unique< specular_reflection_transmission >( wo, n, ( *m_eta_map )( u, v ) );
    }

private:
	//材質の屈折率を保持したテクスチャ
	std::shared_ptr< texture< double > >   m_eta_map;
};

}
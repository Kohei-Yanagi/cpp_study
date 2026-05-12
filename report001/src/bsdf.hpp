//BSDFクラス
#pragma once

#include<numbers>
#include"vec3.hpp"
#include"rng.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// bsdf_sampleクラス  サンプリングした方向やpdfなどの情報をまとめたクラス
class bsdf_sample {
public:

	//コンストラクタ
	bsdf_sample() : m_f(), m_pdf(), m_wi(), m_cos(), m_abs_cos() {}

	//コンストラクタ
	bsdf_sample( const vec3& wi, const col3& f, const double pdf, const double cosine ) : m_f( f ), m_pdf( pdf ), m_wi( wi ), m_cos( cosine ), m_abs_cos( std::abs( cosine ) ) {}

	//サンプリングされた方向を返す
	[[ nodiscard ]] vec3 w() const { return m_wi; }

	//サンプリング方向のBSDF値を返す
	[[ nodiscard ]] col3 f() const { return m_f; }

	//サンプリング方向の確率密度を返す
	[[ nodiscard ]] double pdf() const { return m_pdf; }

	//サンプリング方向と法線との余弦を返す
	[[ nodiscard ]] double cos() const { return m_cos; }

	//サンプリング方向と法線との余弦の絶対値を返す
	[[ nodiscard ]] double abs_cos() const { return m_abs_cos; }

	//サンプルが有効か pdfが非負か否かで判定
	[[ nodiscard ]] bool is_valid() const { return ( m_pdf > 0.0 ); }

	//サンプルが無効か
	[[ nodiscard ]] bool is_invalid() const { return ( m_pdf == 0.0 ); }

private:

	//サンプルのBSDF値
	col3    m_f;
	//サンプルの確率密度(BSDFは立体角measure)
	double  m_pdf;
	//サンプル方向
	vec3    m_wi;
	//サンプル方向と法線の余弦と絶対値
	double  m_cos;
	double  m_abs_cos;
};


//BSDF基底クラス
class bsdf {
public:

	//BSDFの散乱型
	enum type {
		reflection              = 1 << 0,
		transmission            = 1 << 1,
		diffuse                 = 1 << 2,
		glossy                  = 1 << 3,
		specular                = 1 << 4,
		diffuse_reflection      = reflection | diffuse,
		glossy_reflection       = reflection | glossy,
		specular_reflection     = reflection | specular,
		diffuse_transmission    = transmission | diffuse,
		glossy_transmission     = transmission | glossy,
		specular_transmission   = transmission | specular,
		all_reflection          = reflection | diffuse | glossy | specular,
		all_transmission        = transmission | diffuse | glossy | specular,
		all                     = diffuse | glossy | specular | reflection | transmission,
	};

	//コンストラクタ
    bsdf() = default;

	//コンストラクタ
    bsdf( const vec3& wo, const vec3& n, const type& type ) : m_wo( wo ), m_n( n ), m_t( normalize( cross( n, ( std::abs( n.x ) < std::abs( n.y ) ) ? vec3(0.0, n.z, -n.y ) : vec3( -n.z, 0.0, n.x ) ) ) ), m_b( cross( m_t, m_n ) ), m_lwo( to_local( m_wo ) ), m_type( type ) {}

    //デストラクタ
    virtual ~bsdf() = default;

    //BSDFに基づいてサンプリング
    virtual bsdf_sample sample( random_number_generator& rng ) {
	    //余弦項に基づいたサンプリング
    	const double costh = sqrt( 1.0 - rng.rand() );
    	const double sinth = sqrt( std::max( 0.0, 1.0 - costh * costh ) );
    	const double ph = 2.0 * rng.rand() * std::numbers::pi;
    	const double cosph = cos( ph );
    	const double sinph = sin( ph );
    	// サンプル方向wiと確率密度pdf
    	const auto wi = to_world( vec3( sinth * cosph, sinth * sinph, costh ) );
    	const auto pdf = std::numbers::inv_pi * costh;
    	return { wi, f( wi ), pdf, costh };
    };

    //方向wiの確率密度を返す
    virtual double pdf( const vec3& wi ) {
    	return dot( wi, m_n ) * std::numbers::inv_pi;
    };

    //BRDFの値f(wi,wo)を返す
    virtual col3 f( const vec3& wi ) = 0;

    //出射方向を返す(ワールド座標系)
    [[ nodiscard ]] vec3 wo() const { return m_wo; }

    //ローカル座標系への変換
    [[ nodiscard ]] vec3 to_local( const vec3& w ) const { return { dot(m_t, w ), dot(m_b, w ), dot(m_n, w ) }; }

    //ワールド座標系への変換
    [[ nodiscard ]] vec3 to_world( const vec3& w ) const { return w.x * m_t + w.y * m_b + w.z * m_n; }

    //BSDFの型を返す
    [[ nodiscard ]] enum type type() const { return m_type; }

    //BSDFが鏡面材質か
    [[ nodiscard ]] bool is_specular() const { return ( m_type & type::specular ) != 0; }

protected:
    vec3        m_wo;  //出射方向
    vec3        m_n;   //法線(normal)
    vec3        m_t;   //接線(tangent)
    vec3        m_b;   //従法線(binormal)
    vec3        m_lwo; //outgoing direction in local coordinate system
    enum type   m_type;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ランバートBRDF
class lambertian : public bsdf {
public:

	//コンストラクタ
    lambertian( const vec3& wo, const vec3& n, const col3& albedo ) : bsdf( wo, n, type::diffuse_reflection ), m_albedo( albedo ) {}

    //方向wiに対するBRDFの値を計算
    col3 f( const vec3& wi ) override { return m_albedo * std::numbers::inv_pi; }

private:
	//アルベド(0.0 <= m_albedo <= 1.0)
    col3 m_albedo;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// microfacet BRDF
class microfacet : public bsdf {
public:

	//コンストラクタ
    microfacet( const vec3& wo, const vec3& n, const double alpha, const col3& f0 ) : bsdf( wo, n, type::glossy_reflection ), m_alpha( alpha ), m_f0( f0 ) {}

    //方向のサンプリング
    bsdf_sample sample( random_number_generator& rng ) override {
        //ハーフベクトルをサンプリング
        const double xi1 = rng.rand();
        const double xi2 = rng.rand();
        const double theta_h = atan( m_alpha * sqrt( xi1 / ( 1.0 - xi1 ) ) );
        const double cosph = cos( 2.0 * std::numbers::pi * xi2 );
        const double sinph = sin( 2.0 * std::numbers::pi * xi2 );
        const double costh = cos( theta_h );
        const double sinth = sin( theta_h );
		//ハーフベクトルから方向wiへ変換
        const vec3 h = { sinth * cosph, sinth * sinph, costh };
        const vec3 wi = to_world( 2.0 * dot( m_lwo, h ) * h - m_lwo );
        const double cos2 = costh * costh;
        const double sin2 = std::max( 0.0, 1.0 - cos2 );
        const double pdf_h = costh / ( std::numbers::pi * m_alpha * m_alpha * ( cos2 + sin2 / ( m_alpha * m_alpha ) ) * ( cos2 + sin2 / ( m_alpha * m_alpha ) ) );
		const double pdf = pdf_h / ( 4.0 * dot( h, m_lwo ) );
        return { wi, f( wi ), pdf, dot( wi, m_n ) };
    }

    //方向 wiの確率密度を返す
    double pdf( const vec3& wi ) override {
        const vec3 h = normalize( to_local( wi ) + m_lwo );
        const double cos2 = h.z * h.z;
        const double sin2 = std::max( 0.0, 1.0 - cos2 );
        const double pdfh = sqrt( cos2 ) / ( std::numbers::pi * m_alpha * m_alpha * ( cos2 + sin2 / ( m_alpha * m_alpha ) ) * ( cos2 + sin2 / ( m_alpha * m_alpha ) ) );
        const double pdf = pdfh / ( 4.0 * dot( h, m_lwo ) );
    	assert( std::isfinite( pdf ) );
        return pdf;
    }

    //calculate fresnel term using schlick approximation
    [[ nodiscard ]] col3 F( const vec3& lwi ) {
        const vec3 h = normalize( lwi + m_lwo );
        const double cosine = dot( h, lwi );
        const double tmp = ( 1.0 - cosine ) * ( 1.0 - cosine ) * ( 1.0 - cosine ) * ( 1.0 - cosine ) * ( 1.0 - cosine );
        return m_f0 + ( col3( 1.0, 1.0, 1.0 ) - m_f0 ) * tmp;
    }

    //シャドウィング・マスキング項
    [[ nodiscard ]] double G( const vec3& lwi, const vec3& lwo ) const {
        const double tan_i = 1.0 / ( lwi.z * lwi.z ) - 1.0; assert( std::isfinite( tan_i ) );
        const double tan_o = 1.0 / ( lwo.z * lwo.z ) - 1.0; assert( std::isfinite( tan_o ) );
        const double lambda_i = ( - 1.0 + sqrt( 1.0 + m_alpha * m_alpha * tan_i ) ) / 2.0; assert( std::isfinite( lambda_i ) );
        const double lambda_o = ( - 1.0 + sqrt( 1.0 + m_alpha * m_alpha * tan_o ) ) / 2.0; assert( std::isfinite( lambda_o ) );
        return 1.0 / ( 1.0 + lambda_i + lambda_o );
    }

    //法線分布関数
    [[ nodiscard ]] double D( const vec3& lwi ) const {
        const vec3 h = normalize( lwi + m_lwo );
        const double cos_h = h.z * h.z;
        const double sin_h = std::max( 0.0, 1.0 - cos_h );
        const double denom = std::numbers::pi * m_alpha * m_alpha * ( cos_h + sin_h / ( m_alpha * m_alpha ) ) * ( cos_h + sin_h / ( m_alpha * m_alpha ) );
        assert( std::isfinite( denom ) );
        return 1.0 / denom;
    }

    //入射方向 wiに対するBRDFの値を計算
    [[ nodiscard ]] col3 f( const vec3& wi ) override {
        const vec3 lwi = to_local( wi );
        col3 brdf {};
        if( lwi.z > 0.0 && m_lwo.z > 0.0 ) {
            const double cos_i = lwi.z;
            const double cos_o = m_lwo.z;
            brdf = F( lwi ) * G( lwi, m_lwo ) * D( lwi ) / ( 4.0 * cos_i * cos_o );
            assert( isfinite( brdf ) );
        }
        return brdf;
    }

private:
	//GGXの粗さパラメータ
    double m_alpha;
	//フレネル項のパラメータ
    col3 m_f0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//拡散光源(全方向に一様な輝度を放射する光源)
class diffuse_light : public bsdf {
public:

    //コンストラクタ
    diffuse_light( const vec3& wo, const vec3& n, const col3& Me ) : bsdf( wo, n, type::diffuse_reflection ), m_Me( Me ) {}

    //方向wiの放射輝度(発光)を返す
    col3 f( const vec3 &wi ) override { return m_Me * std::numbers::inv_pi; }

private:
	//放射発散度 radiant exitance
    col3 m_Me;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// utility関数
namespace util {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 誘電体用のフレネル反射率を計算 (eta_i : 入射側の絶対屈折率, eta_t : 透過側の絶対屈折率, cosi : 入射方向と法線の余弦
double fresnel( const double eta_i, const double eta_t, const double cosi ) {
	/*
	bool enter = ( cosi > 0.0 );
	if( not( enter ) ) {
		std::swap( eta_i, eta_t );
		cosi = std::abs( cosi );
	}
	*/
	const auto sini = sqrt( std::max( 0.0, 1.0 - cosi * cosi ) );
	const auto sint = eta_i / eta_t * sini;
	const auto cost = sqrt( std::max( 0.0, 1.0 - sint * sint ) );
	const auto r0 = ( eta_t * cosi - eta_i * cost ) / ( eta_t * cosi + eta_i * cost );
	const auto r1 = ( eta_i * cosi - eta_t * cost ) / ( eta_i * cosi + eta_t * cost );
	return ( r0 * r0 + r1 * r1 ) / 2.0;
}
// 全反射(total internal reflection)かどうかを判定してフレネル反射率を計算 全反射する場合 tir <- true
double fresnel( const double eta_i, const double eta_t, const double cosi, bool &tir ) {
	double g = eta_t * eta_t / ( eta_i * eta_i ) - 1.0 + cosi * cosi;
	tir = ( g < 0.0 ); //全反射かどうか判定
	if ( tir ) {
		return 1.0;
	} else {
		g = sqrt( g );
		const auto tmp0 = ( g - cosi ) / ( g + cosi );
		const auto tmp1 = ( cosi * ( g + cosi ) - 1.0 ) / ( cosi * ( g - cosi ) + 1.0 );
		return 0.5 * tmp0 * tmp0 * ( 1.0 + tmp1 * tmp1 );
	}
}

//正反射ベクトルを計算
vec3 reflection( const vec3 &wo, const vec3 &n ) {
	return -wo + 2 * dot( wo, n ) * n;
}

//屈折ベクトルを計算 : eta = eta_i / eta_t
bool refraction( const vec3 &wi, const vec3 &n, const double eta, vec3 &wo ) {
	const auto cosi = dot( n, wi );
	const auto sini = std::max( 0.0, 1.0 - cosi * cosi );
	const auto sint = eta * eta * sini;
	if ( sint > 1.0 )
		return false; //total internal reflection
	const auto cost = std::sqrt( 1.0 - sint );
	wo = eta * ( -wi ) + ( eta * cosi - cost ) * n;
	return true;
}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//完全鏡面透過BSDF
class specular_reflection_transmission : public bsdf {
public:
    // コンストラクタ
    specular_reflection_transmission( const vec3& wo, const vec3& n, const double eta ) : bsdf( wo, n, type::specular_transmission ), m_eta( eta ), m_fr( 0.0 ), m_ft( 0.0 ), m_is_reflect( true ) {}

    // サンプリング
    bsdf_sample sample( random_number_generator& rng ) override {
        const auto cosine = dot( m_wo, m_n );
        const bool enter = ( cosine > 0.0 );
        const auto eta_i = ( enter ) ? 1.0   : m_eta;
        const auto eta_t = ( enter ) ? m_eta : 1.0  ;
        const auto eta = eta_i / eta_t;
        m_fr = ( enter ) ? util::fresnel( eta_i, eta_t, cosine ) : util::fresnel( eta_i, eta_t, - cosine ); assert( 0.0 <= m_fr && m_fr <= 1.0 );
        m_ft = ( 1.0 - m_fr ) / ( eta * eta ); //eta_t^2/eta_i^2 * ( 1 - Fr )
        const auto xi0 = rng.rand();
        double pdf = 1.0;
        vec3 wi;
        const auto nn = ( enter ) ? m_n : - m_n;

		//反射を選択，反射方向を返す
        if( xi0 < m_fr ) {
            m_is_reflect = true;
            pdf = m_fr;
            wi = util::reflection( m_wo, nn );
            return { wi, fr( wi ), pdf, dot( wi, nn ) };
        }
		//透過を選択
        if( not( util::refraction( m_wo, nn, eta, wi ) ) ) {
            m_is_reflect = true;
            wi = util::reflection( m_wo, nn );
            return { wi, fr( wi ), pdf, dot( wi, nn ) };
        } else {
            pdf = 1 - m_fr;
            m_is_reflect = false;
            wi = normalize( wi );
            return { wi, ft( wi ), pdf, dot( wi, nn ) };
        }
    }

    // 方向wiのpdfは（デルタ関数のため）0
    double pdf( const vec3 &wi ) override { return 0.0; }

    // 方向wiのBRDF値
    col3 f( const vec3 &wi ) override { return {}; }

private:
	//フレネル反射率
    double m_fr;
    //フレネル透過率
    double m_ft;
    //相対屈折率
    double m_eta;
    //反射か透過かのフラグ 反射の場合true, 透過の場合false
    bool m_is_reflect;

    col3 fr( const vec3 &wi ) { return col3( m_fr / std::abs( dot( wi, m_n ) ) ); }

    col3 ft( const vec3 &wi ) { return col3( m_ft / std::abs( dot( wi, m_n ) ) ); }

};


//乱数クラス
#pragma once

#include<random>
#include<algorithm>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//乱数クラス
class random_number_generator {
public:
    //コンストラクタ
    random_number_generator() : m_mt( 0 ) {}

    random_number_generator( std::random_device& rd ) : m_mt( rd() ) {}

    //[0,1)の一様乱数を生成
    double rand() { return m_rng( m_mt ); }

    //[x0,x1)の範囲でランダムな整数を生成
    size_t rand( const size_t x0, const size_t x1 ) {
        const double x = m_rng( m_mt );
        return std::clamp< size_t >( x0 + x * ( x1 - x0 ), x0, x1 - 1 );
    }

private:
    //std::random_device m_device;
    std::mt19937 m_mt;
    std::uniform_real_distribution< double > m_rng;
};


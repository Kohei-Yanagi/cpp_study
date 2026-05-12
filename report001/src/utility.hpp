//ユーティリティ関数定義
#pragma once

#include"vec3.hpp"
#include"rng.hpp"

//RGBから輝度に変換
inline double luminance( const col3& c ) {
    return 0.2126171 * c.x + 0.715160 * c.y + 0.072169 * c.z;
}

inline double russian_roulette( const col3& throughput_weight, const col3& prev_throughput_weight, random_number_generator& rng ) {
    const auto probability = std::min( luminance( throughput_weight ) / luminance( prev_throughput_weight ), 0.9 );
    if( rng.rand() < probability ) {
        return probability;
    } else {
        return 0.0;
    }
}

inline double russian_roulette( const double q, random_number_generator& rng ) {
    if( rng.rand() < q ) {
        return q;
    } else {
        return 0.0;
    }
}


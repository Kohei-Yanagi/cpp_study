//レンダラー基底クラス
#pragma once

#include"../scene.hpp"
#include"../camera.hpp"

//レンダラー(画像生成)の基底クラス
class renderer {
public:
	//デストラクタ
    virtual ~renderer() = default;
	//画像生成の純粋仮想関数
    [[ nodiscard ]] virtual std::vector< float > render( const scene& scene, const camera& camera ) const = 0;
};


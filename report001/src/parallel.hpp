//並列処理用関数
#pragma once

#include<vector>
#include<thread>

//関数f(f(0),...,f(n-1))を並列処理
template< class F > void in_parallel( const size_t n, F&& f, const size_t nt = std::thread::hardware_concurrency() )
{
    std::atomic< size_t > i( 0 );
    std::vector< std::thread > threads( nt );

    for( auto &thread : threads ) {
        auto g = [ &n, &i ]( const F& f ) {
            for( size_t j; ( j = i.fetch_add( 1 ) ) < n; ) {
                f( j );
            }
        };
        thread = std::thread( g, std::forward< F >( f ) );
    }

    for( auto& thread : threads ) {
        thread.join();
    }
}

#pragma once
#include <string>

///
/// thanks to Progrematc
/// https://www.youtube.com/watch?v=DQCkMnMNFBI
///
namespace logger
{
    class manager
    {
    public:
        manager() = default;
        ~manager() = default;

        void Initialize();
        void Shutdown();        
    };
}
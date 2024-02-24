#pragma once

#include "detail/PluginBase.h"

namespace Schism::Ecs
{
    class IScene;

    template<typename ...Components>
    class Plugin: public detail::PluginBase
    {
    public:
        Plugin();
        
    protected:
        void AddSystem()
        {

        }

    private:
        
        void Update(float dt) override
        {

        }

        void Draw() override
        {

        }
    };
}

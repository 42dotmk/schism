#pragma once

#include "detail/PluginBase.h"
#include <sol/state.hpp>

namespace Schism::Ecs
{
    class IScene;

    template<typename ...Components>
    class Plugin: public detail::PluginBase
    {
    public:
        friend IScene;

        Plugin();

        static void Bind(sol::state& state)
        {

        }

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

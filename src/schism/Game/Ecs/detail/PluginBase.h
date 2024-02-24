#pragma once

namespace Schism::Ecs::detail
{
    class IScene;

    class PluginBase
    {
    public:
        virtual void Build() = 0;

    protected:
        virtual void Update(float dt) = 0;
        
        // Not sure if we should have draw here
        // the drawing system might need to be refactored
        virtual void Draw() = 0;
    };
}

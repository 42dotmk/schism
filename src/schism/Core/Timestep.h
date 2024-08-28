#pragma once

namespace Schism {

class Timestep {
    public:
        Timestep(float dt = 0.f) : m_Time(dt) {}

        operator float() const { return m_Time; }

        float GetSeconds() const { return m_Time; }

        float GetMiliseconds() const { return m_Time * 1000.f; }

    private:
        float m_Time;
};

}  // namespace Schism

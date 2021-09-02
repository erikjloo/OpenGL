#pragma once
#include "Test.h"
#include "../Util.h"
#include "../vendor/imgui/imgui.h"

namespace test
{

    // ============================================================================
    // Class definition
    // ============================================================================

    class TestClearColor : public Test
    {
    public:
        TestClearColor() : Test(), m_ClearColor{0.2f, 0.3f, 0.8f, 1.0f} {}
        ~TestClearColor() {}
        void OnUpdate(float deltaTime) override {}
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        float m_ClearColor[4];
    };

    // ============================================================================
    // Implementation
    // ============================================================================

    void TestClearColor::OnRender()
    {
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
    void TestClearColor::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
    }
}
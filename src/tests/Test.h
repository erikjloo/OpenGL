#pragma once
#include <functional>
#include <vector>

#include "../vendor/imgui/imgui.h"

namespace test
{

    // ============================================================================
    // Class definitions
    // ============================================================================

    class Test
    {
    public:
        Test() {}
        virtual ~Test() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}
    };

    class TestMenu : public Test
    {
    public:
        TestMenu(Test *&currentTestPointer) : m_CurrentTest(currentTestPointer) {}
        ~TestMenu() {};

        template <typename T>
        void RegisterTest(const std::string& testName);
        void OnImGuiRender() override;
    private:
        Test *&m_CurrentTest;
        std::vector<std::pair<std::string, std::function<Test *()>>> m_Tests;
    };

    // ============================================================================
    // Implementation
    // ============================================================================

    template <typename T>
    void TestMenu::RegisterTest(const std::string& testName)
    {
        m_Tests.push_back(std::make_pair(testName, [](){ return new T(); }));
    }

    void TestMenu::OnImGuiRender()
    {
        for (auto &test : m_Tests)
            if (ImGui::Button(test.first.c_str()))
                m_CurrentTest = test.second();
    }
}
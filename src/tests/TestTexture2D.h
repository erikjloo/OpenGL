#pragma once
#include "Test.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Util.h"
#include "../Renderer.h"
#include "../Texture.h"

namespace test
{

    // ============================================================================
    // Class definition
    // ============================================================================

    class TestTexture2D : public Test
    {
    public:
        TestTexture2D();
        ~TestTexture2D() {}
        void OnUpdate(float deltaTime) override {}
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        glm::mat4 m_Proj;
        glm::mat4 m_View;
        glm::vec3 m_translationA{200, 200, 0};
        glm::vec3 m_translationB{400, 200, 0};
    };

    // ============================================================================
    // Implementation
    // ============================================================================

    TestTexture2D::TestTexture2D() : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
                                     m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
    {
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
            50.0f, -50.0f, 1.0f, 0.0f,  // 1
            50.0f, 50.0f, 1.0f, 1.0f,   // 2
            -50.0f, 50.0f, 0.0f, 1.0f   // 3
        };
        uint indices[] = {0, 1, 2, 2, 3, 0}; // elements

        /* Show alpha channels correctly */
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Define vertices */
        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        /* Define elements */
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        /* Bind texture */
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_Texture = std::make_unique<Texture>("res/textures/icon.png");
        m_Shader->SetUniform1i("u_Texture", 0);
    }
    void TestTexture2D::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        Renderer renderer;
        renderer.Clear();

        m_Texture->Bind();
        /* Bind and draw */
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }
    }
    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("m_translationA", &m_translationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("m_translationB", &m_translationB.x, 0.0f, 960.0f);
    }
}
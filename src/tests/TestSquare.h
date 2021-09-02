#pragma once
#include "Test.h"

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

    class TestSquare : public Test
    {
    public:
        TestSquare();
        ~TestSquare() {}
        void OnUpdate(float deltaTime) override {}
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        float positions[16];
        uint indices[6];
        VertexArray va;
        VertexBuffer vb;           // vertices
        IndexBuffer ib;            // elements
        Shader shader{"res/shaders/Basic.shader"};
        Texture texture{"res/textures/icon.png"};

        glm::mat4 proj;
        glm::mat4 view;
        glm::vec3 translationA{200, 200, 0};
        glm::vec3 translationB{400, 200, 0};
    };

    // ============================================================================
    // Implementation
    // ============================================================================

    TestSquare::TestSquare() : positions{
                                   -50.0f, -50.0f, 0.0f, 0.0f, // 0
                                   50.0f, -50.0f, 1.0f, 0.0f,  // 1
                                   50.0f, 50.0f, 1.0f, 1.0f,   // 2
                                   -50.0f, 50.0f, 0.0f, 1.0f   // 3
                               },
                               indices{0, 1, 2, 2, 3, 0},                                 //
                               vb{positions, 4 * 4 * sizeof(float)},                      //
                               ib{indices, 6},                                            //
                               proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), //
                               view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
    {
        /* Show alpha channels correctly */
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Define vertices */
        VertexBufferLayout layout; // structure of each vertex
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        /* Bind texture */
        shader.Bind();
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
    }
    void TestSquare::OnRender()
    {
        Renderer renderer;
        renderer.Clear();

        shader.Bind();

        /* Bind and draw */
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = proj * view * model;
            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }
    }
    void TestSquare::OnImGuiRender()
    {
        ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, 960.0f);
    }
}
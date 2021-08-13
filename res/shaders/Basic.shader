  #shader vertex
#version 330 core

layout(location = 0) in vec4 position; // 0 should match with 0 in glVertexAttribPointer

void main()
{
    gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color; // 0 should match with 0 in glVertexAttribPointer

uniform vec4 u_Color;

void main()
{
    color = u_Color;
};
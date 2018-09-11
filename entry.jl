# =============================================================================
#  <entry.jl> Module Name: None
# -----------------------------------------------------------------------------
#  The entry point of VM-76 Julia Edition
# =============================================================================

include("dependencies.jl")

# If you miss dependencies:
# Dependencies.install_all()

using ModernGL

include("render_manager.jl")
include("render_target.jl")
include("shaders.jl")
include("GL/VBO.jl")
include("GL/VAO.jl")

main_state = RenderManager.create_state()

vertices = GLfloat[
    -0.5, -0.5, 0.0, 1.0, 0.0, 0.0,
     0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
     0.0,  0.5, 0.0, 0.0, 0.0, 1.0
]
vbo = VBO.createVBO([(GLfloat, 3, false), (GLfloat, 3, false)], vertices)
vao = VAO.createVAO([vbo])
draw_vao = VAO.getDrawcall(vao)

vsh = """
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertcolor;

out vec3 color;

void main()
{
    color = vertcolor;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
"""

fsh = """
#version 330 core

in vec3 color;
out vec4 FragColor;

void main()
{
    FragColor = vec4(color, 1.0f);
}
"""

shaders = Shaders.create(fsh, vsh)
draw_using_shaders = Shaders.drawUsing(shaders)

main_render_loop = RenderManager.create_render_loop(main_state, () -> begin
    RenderTarget.setRenderTargetToState(main_state)
    RenderTarget.clear()
    draw_using_shaders(() -> begin
        draw_vao()
    end)
end)

main_render_loop()

Shaders.delete(shaders)
RenderManager.destroy_state(main_state)
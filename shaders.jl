# =============================================================================
#  <shaders.jl> Module Name: Shaders
# -----------------------------------------------------------------------------
#  Manage graphics shaders
# =============================================================================

module Shaders
    using ModernGL

    include("GL/gltools.jl")

    export ShadersObject
    export create, delete, drawUsing

    mutable struct ShadersObject
        vertex::GLint
        fragment::GLint
        program::GLint
    end

    function create(fsh::String, vsh::String)
        vertexShader = createShader(vsh, GL_VERTEX_SHADER)
        fragmentShader = createShader(fsh, GL_FRAGMENT_SHADER)
        program = createShaderProgram(vertexShader, fragmentShader)

        return ShadersObject(vertexShader, fragmentShader, program)
    end

    function delete(shaders::ShadersObject)
        glDeleteShader(shaders.vertex)
        glDeleteShader(shaders.fragment)
        glDeleteProgram(shaders.program)
    end

    function drawUsing(shaders::ShadersObject)
        function draw(f::Function)
            glUseProgram(shaders.program)
            f()
            glUseProgram(0)
        end
        return draw
    end
end

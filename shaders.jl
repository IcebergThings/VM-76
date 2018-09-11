# =============================================================================
#  <shaders.jl> Module Name: Shaders
# -----------------------------------------------------------------------------
#  Manage graphics shaders
# =============================================================================

module Shaders
    using ModernGL

    include("GL/gltools.jl")

    export ShadersObject
    export create, delete, drawUsing, setFloat

    mutable struct ShadersObject
        vertex::GLint
        fragment::GLint
        program::GLint
        uniforms::Dict{String, GLint}
    end

    function create(fsh::String, vsh::String)
        vertexShader = createShader(vsh, GL_VERTEX_SHADER)
        fragmentShader = createShader(fsh, GL_FRAGMENT_SHADER)
        program = createShaderProgram(vertexShader, fragmentShader)

        return ShadersObject(vertexShader, fragmentShader, program, Dict{String, GLint}())
    end

    function delete(shaders::ShadersObject)
        glDeleteShader(shaders.vertex)
        glDeleteShader(shaders.fragment)
        glDeleteProgram(shaders.program)
    end

    function drawUsing(shaders::ShadersObject)
        function draw(f::Function)
            glUseProgram(shaders.program)
            f(shaders)
            glUseProgram(0)
        end
        return draw
    end

    function getUniformLoca(shaders::ShadersObject, key::String)
        loca = 0
        if haskey(shaders.uniforms, key)
            loca = shaders.uniforms[key]
        else
            shaders.uniforms[key] = loca = glGetUniformLocation(shaders.program, "iTime")
        end
        return loca
    end

    function setFloat(shaders::ShadersObject, key::String, value::Float32)
        glUseProgram(shaders.program)
        glUniform1f(getUniformLoca(shaders, key, value), value)
    end

    # This is faster in functions called in drawUsing()
    function setFloatCurrent(shaders::ShadersObject, key::String, value::Float32)
        glUniform1f(getUniformLoca(shaders, key), value)
    end
end

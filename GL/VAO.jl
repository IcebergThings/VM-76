# =============================================================================
#  <GL/VAO.jl> Module Name: VAO
# -----------------------------------------------------------------------------
#  Manage Vertex Array Object in OpenGL
# =============================================================================

module VAO
    using ModernGL
    include("gltools.jl")

    export VAO
    export createVAO, bindVBO, getDrawcall

    VBO = Main.VBO

    mutable struct VAO_obj
        object_id::GLint
        vbos::Array{VBO.VBO_obj}
        elements::Int
    end

    function createVAO(vbos::Array{VBO.VBO_obj} = [])
        vao = VAO_obj(glGenVertexArray(), vbos, 0)

        if length(vbos) > 0
            bindVBO(vao)
        end

        return vao
    end

    function bindVBO(vao::VAO_obj)
        glBindVertexArray(vao.object_id)
        location, elements = 0, 0
        for obj in vao.vbos
            location = VBO.applyAttrFormat(obj, location)
            # This must be uniform for all VBOs in the same group
            vao.elements = obj.num_elements
        end
        for i = 0:(location-1)
            glEnableVertexAttribArray(i)
        end
        glBindVertexArray(0)
    end

    function getDrawcall(vao::VAO_obj, before_draw::Function = ()->(), after_draw::Function = ()->(), start_element = 0, end_element = -1)
        function draw()
            glBindVertexArray(vao.object_id)
            before_draw()
            glDrawArrays(GL_TRIANGLES, start_element, end_element < 0 ? vao.elements : end_element)
            after_draw()
            glBindVertexArray(0)
        end
        return draw
    end
end

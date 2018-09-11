# =============================================================================
#  <GL/VBO.jl> Module Name: VBO
# -----------------------------------------------------------------------------
#  Manage Vertex Buffer Object in OpenGL
# =============================================================================

module VBO
    using ModernGL
    include("gltools.jl")

    export VBO, VertexAttrFormat
    export createVBO, applyAttrFormat, uploadData

    VertexAttrFormat = Array{Tuple{DataType,Int ,Bool      }}
    #                       [     (Type    ,Size,Normalized)]

    mutable struct VBO_obj
        buffer_id::GLint
        attr_format::VertexAttrFormat
        size::GLint
        num_elements::GLint
    end

    function createVBO(attr_format::VertexAttrFormat = [(GLfloat, 3, false)], vertices = [], draw_hint = GL_STATIC_DRAW)
        vbo = VBO_obj(glGenBuffer(), attr_format, sizeof(vertices), 0)

        uploadData(vbo, vertices, draw_hint)
        
        glCheckError("Creating VBO")

        return vbo
    end

    function applyAttrFormat(vbo::VBO_obj, location::Int = 0)
        glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer_id)

        stride = 0
        for attr in vbo.attr_format
            stride += attr[2] * sizeof(attr[1])
        end

        i, offset = location, 0
        for attr in vbo.attr_format
            glVertexAttribPointer(i, attr[2], attr[1] == GLfloat ? GL_FLOAT : GL_INT, attr[3], stride, Base.convert(Ptr{Nothing}, offset))
            glEnableVertexAttribArray(i)
            i += 1
            offset += attr[2] * sizeof(attr[1])
        end
        
        # We do not clean this up, since it is always quite useful to leave this here
        #glBindBuffer(GL_ARRAY_BUFFER, 0)
        glCheckError("Setting VBO Attr Format")

        vbo.num_elements = vbo.size / stride

        return i
    end

    function uploadData(vbo::VBO_obj, data_array::Array, draw_hint = GL_STATIC_DRAW)
        glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer_id)
        glBufferData(GL_ARRAY_BUFFER, sizeof(data_array), data_array, draw_hint)
        glBindBuffer(GL_ARRAY_BUFFER, 0)
    end
end

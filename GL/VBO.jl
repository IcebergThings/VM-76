# =============================================================================
#  <GL/VBO.jl> Module Name: VBO
# -----------------------------------------------------------------------------
#  Manage Vertex Buffer Object in OpenGL
# =============================================================================

module VBO
    using ModernGL
    include("gltools.jl")

    export VBO, VertexAttrFormat
    export createVBO, applyAttrFormat, uploadData, uploadElementIndices

    VertexAttrFormat = Array{Tuple{DataType,Int ,Bool      }}
    #                       [     (Type    ,Size,Normalized)]

    mutable struct VBO_obj
        buffer_id::GLuint
        attr_format::VertexAttrFormat
        size::GLint
        num_elements::GLint
        ebo_id::GLuint
    end

    function createVBO(attr_format::VertexAttrFormat = [(GLfloat, 3, false)], vertices = [], ebo = [], draw_hint = GL_STATIC_DRAW)
        vbo = VBO_obj(glGenBuffer(), attr_format, sizeof(vertices), 0, 0)

        uploadData(vbo, vertices, draw_hint)
        
        glCheckError("Creating VBO")

        if length(ebo) > 0
            ebo_id = glGenBuffer()
            uploadElementIndices(ebo_id, ebo, draw_hint)
            glCheckError("Creating EBO")

            vbo.ebo_id = ebo_id
        end

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

        if vbo.ebo_id != 0
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.ebo_id)
        end

        return i
    end

    function _uploadData(T::GLuint, id::GLuint, data_array::Array, draw_hint = GL_STATIC_DRAW)
        glBindBuffer(T, id)
        glBufferData(T, sizeof(data_array), data_array, draw_hint)
        glBindBuffer(T, 0)
    end

    uploadElementIndices(ebo::GLuint, data_array::Array, draw_hint = GL_STATIC_DRAW) = _uploadData(GL_ELEMENT_ARRAY_BUFFER, ebo, data_array, draw_hint)
    uploadData(vbo::VBO_obj, data_array::Array, draw_hint = GL_STATIC_DRAW) = _uploadData(GL_ARRAY_BUFFER, vbo.buffer_id, data_array, draw_hint)
end

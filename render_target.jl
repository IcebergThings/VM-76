# =============================================================================
#  <render_target.jl> Module Name: RenderTarget
# -----------------------------------------------------------------------------
#  Manage graphics render target
# =============================================================================

module RenderTarget
    using ModernGL

    export RenderTargetObject
    export setRenderTarget, clear, screen_target

    mutable struct RenderTargetObject
        target::GLint
    end

    screen_target = RenderTargetObject(0)

    # OpenGL can't handle multiple contexts easily without breaking
    # So we use this in global context (like Unity3D, YEAH)
    function setRenderTarget(target::RenderTargetObject, x = 0, y = 0, w = 0, h = 0)
        glBindFramebuffer(GL_FRAMEBUFFER, target.target)
        glViewport(x,y,w,h)
    end

    function setRenderTargetToState(state::Main.RenderManager.RenderState)
        setRenderTarget(RenderTarget.screen_target, 0, 0, state.w, state.h)
    end

    function clear(color::Bool = true, depth::Bool = true, stencil::Bool = true;
        r = 0.0,
        g = 0.0,
        b = 0.0,
        a = 1.0
    )
        mask::GLuint = 0
        if color
            mask |= GL_COLOR_BUFFER_BIT
            glClearColor(r, g, b, a)
        end
        if depth
            mask |= GL_DEPTH_BUFFER_BIT
        end
        if stencil
            mask |= GL_STENCIL_BUFFER_BIT
        end
        glClear(mask)
    end
end

# =============================================================================
#  <render_manager.jl> Module Name: RenderManager
# -----------------------------------------------------------------------------
#  The render manager. It does the GL operations and window management
# =============================================================================

module RenderManager
    import GLFW
    using ModernGL
    using Printf

    export RenderState, GraphicsEnvirontment
    export create_state, destroy_state, create_render_loop
    export graphics_environment

    include("GL/gltools.jl")
    
    mutable struct GraphicsEnvirontment
        OpenGL::Bool
        version_major::Int
        version_minor::Int
        version_patch::Int
    end

    mutable struct RenderState
        window
        title::String
        w::Int
        h::Int
        fps::Float32
        fps_average::Float32
        delta::Float64
        elapsed::Float32
    end

    graphics_environment = GraphicsEnvirontment(
        true,
        4, 5, 0
    )

    function create_state(w = 1280, h = 720, title = "VM-76")
        state = RenderState(0, title, w, h, 0.0, 0.0, 1.0, 0.0)

        println("Creating state:", title)
        
        # If it is not successful, we then guess it's GL 4.3, then GL 3.3
        try
            state.window = GLFW.CreateWindow(state.w, state.h, title)
        catch
            GLFW.WindowHint(GLFW.CONTEXT_VERSION_MAJOR, 4)
            GLFW.WindowHint(GLFW.CONTEXT_VERSION_MINOR, 3)
            
            global graphics_environment.version_major = 4
            global graphics_environment.version_minor = 3

            try
                state.window = GLFW.CreateWindow(w, h, title)
            catch
                GLFW.WindowHint(GLFW.CONTEXT_VERSION_MAJOR, 3)
                GLFW.WindowHint(GLFW.CONTEXT_VERSION_MINOR, 3)
                    
                global graphics_environment.version_major = 3
                global graphics_environment.version_minor = 3

                state.window = GLFW.CreateWindow(w, h, title)
            end
        end

        println("Using OpenGL version ", graphics_environment.version_major, graphics_environment.version_minor)

        # Finish setup
        GLFW.MakeContextCurrent(state.window)
        GLFW.SetWindowSizeCallback(state.window, (window::GLFW.Window, w::GLint, h::GLint) -> begin
            glViewport(0, 0, w, h)
            state.w = w
            state.h = h
        end)
        printGLInfo()

        glViewport(0, 0, state.w, state.h)

        return state
    end

    function destroy_state(state)
        GLFW.DestroyWindow(state.window)
        GC.gc()
    end

    function create_render_loop(state, loop)
        function render_loop()
            t_base = t_prev = Base.time_ns()
            timer_sec = 0.0
            while !GLFW.WindowShouldClose(state.window)
                loop()

                GLFW.SwapBuffers(state.window)
                GLFW.PollEvents()

                t_now = Base.time_ns()
                diff_t = t_now - t_prev
                state.delta = diff_t * 1.0e-6
                state.fps = 1000.0e6 / diff_t
                state.elapsed = (t_now - t_base) * 1.0e-9
                # This is not according to time step, the integration according to timestep will be done later
                state.fps_average = 0.9 * state.fps_average + 0.1 * state.fps
                timer_sec += diff_t * 1.0e-9
                if timer_sec > 0.1
                    timer_sec = 0.0
                    GLFW.SetWindowTitle(state.window, @sprintf("%s - FPS:%d/%d %.2fms", state.title, round(state.fps), round(state.fps_average), state.delta))
                end
                t_prev = t_now
            end
        end
        return render_loop
    end

    function __init__()
        println("Initialize Render Manager")

        GLFW.SetErrorCallback((error, description) -> begin
            println("GLFW Error", error, description)
        end)

        # Try and guess process. Unless we create a window, we actually don't know
        GLFW.WindowHint(GLFW.OPENGL_PROFILE, GLFW.OPENGL_CORE_PROFILE)
        GLFW.WindowHint(GLFW.OPENGL_FORWARD_COMPAT, true)
        GLFW.WindowHint(GLFW.CONTEXT_VERSION_MAJOR, 4)
        GLFW.WindowHint(GLFW.CONTEXT_VERSION_MINOR, 5)
        GLFW.WindowHint(GLFW.RESIZABLE, true)
    end
end

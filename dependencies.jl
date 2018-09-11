# =============================================================================
#  <dependencies.jl> Module Name: Dependencies
# -----------------------------------------------------------------------------
#  Manage dependencies
# =============================================================================

module Dependencies

    import Pkg
    export install_all

    function install_dependencies(deps, deps_git)
        for d in deps
            Pkg.add(d)
        end
        for d in deps_git
            Pkg.clone(d)
        end
    end
    
    install_all() = install_dependencies(["GLFW","ModernGL"],[])
end

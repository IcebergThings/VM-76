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
    -1.0,  -1.0,  0.0,
    -1.0,   1.0,  0.0,
     1.0,   1.0,  0.0,
     
    -1.0,  -1.0,  0.0,
     1.0,  -1.0,  0.0,
     1.0,   1.0,  0.0,

    -0.5,  -0.5,  0.0,
     0.5,  -0.5,  0.0,
     0.0,   0.5,  0.0
]
colors = GLfloat[
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
     
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,

    1.0, 0.8, 0.6,
    0.6, 1.0, 0.8,
    0.8, 0.6, 1.0
]
vbo0 = VBO.createVBO([(GLfloat, 3, false)], vertices)
vbo1 = VBO.createVBO([(GLfloat, 3, false)], colors)
vao = VAO.createVAO([vbo0, vbo1])
draw_vao = VAO.getDrawcall(vao)

vsh = """
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertcolor;

out vec3 color;
out vec2 fragCoord;

void main()
{
    color = vertcolor;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    fragCoord = aPos.xy;
}
"""

fsh = """
#version 330 core

uniform float iTime;

in vec2 fragCoord;
in vec3 color;
out vec4 fragColor;

// The MIT License
// Copyright © 2014 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

vec3 diskWithMotionBlur( vec3 col, in vec2 uv, in vec3 sph, in vec2 cd, in vec3 sphcol )
{
	vec2 xc = uv - sph.xy;
	float a = dot(cd,cd);
	float b = dot(cd,xc);
	float c = dot(xc,xc) - sph.z*sph.z;
	float h = b*b - a*c;
	if( h>0.0 )
	{
		h = sqrt( h );
		
		float ta = max( 0.0, (-b - h)/a );
		float tb = min( 1.0, (-b + h)/a );
		
		if( ta < tb ) // we can comment this conditional, in fact
		    col = mix( col, sphcol, clamp(2.0*(tb-ta),0.0,1.0) );
	}

	return col;
}


vec3 hash3( float n ) { return fract(sin(vec3(n,n+1.0,n+2.0))*43758.5453123); }
vec4 hash4( float n ) { return fract(sin(vec4(n,n+1.0,n+2.0,n+3.0))*43758.5453123); }

const float speed = 8.0;
vec2 getPosition( float time, vec4 id ) { return vec2(       0.9*sin((speed*(0.75+0.5*id.z))*time+20.0*id.x),        0.75*cos(speed*(0.75+0.5*id.w)*time+20.0*id.y) ); }
vec2 getVelocity( float time, vec4 id ) { return vec2( speed*0.9*cos((speed*(0.75+0.5*id.z))*time+20.0*id.x), -speed*0.75*sin(speed*(0.75+0.5*id.w)*time+20.0*id.y) ); }

void main() {
	vec2 p = fragCoord;
	
	vec3 col = vec3(0.2) + 0.05*p.y;
	
	for( int i=0; i<16; i++ )
	{		
		vec4 off = hash4( float(i)*13.13 );
        vec3 sph = vec3( getPosition( iTime, off ), 0.02+0.1*off.x );
        vec2 cd = getVelocity( iTime, off ) /24.0 ;
		vec3 sphcol = 0.7 + 0.3*sin( 3.0*off.z + vec3(4.0,0.0,2.0) );
		
        col = diskWithMotionBlur( col, p, sph, cd, sphcol );
	}		

    col += (1.0/255.0)*hash3(p.x+13.0*p.y);

	fragColor = vec4(col * color,1.0);
}
"""

shaders = Shaders.create(fsh, vsh)
draw_using_shaders = Shaders.drawUsing(shaders)

main_render_loop = RenderManager.create_render_loop(main_state, () -> begin
    RenderTarget.setRenderTargetToState(main_state)
    RenderTarget.clear()
    RenderTarget.setRenderTarget(RenderTarget.screen_target, 50, 50, main_state.w - 100, main_state.h - 100)
    draw_using_shaders((shaders) -> begin
        Shaders.setFloatCurrent(shaders, "iTime", main_state.elapsed);
        draw_vao()
    end)
end)

main_render_loop()

Shaders.delete(shaders)
RenderManager.destroy_state(main_state)
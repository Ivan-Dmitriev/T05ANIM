/**/
#version 420
 
layout(location = 0) out vec4 OutColor;

layout(binding = 0) uniform sampler2D Texture0;
uniform bool IsTexture0;

in vec2 DrawTexCoord;                    
uniform vec3 CamRight;
uniform vec3 CamUp;
uniform vec3 CamDir;


uniform int FrameW, FrameH;

void  main( void )
{
  vec2 s = DrawTexCoord * 2 - 1;
  vec3 dir = normalize(CamDir + CamRight * s.x + CamUp * s.y);
  float
    theta = acos(dir.y),
    phi = atan(dir.x, dir.z),
    pi = acos(-1);
  vec2 c = vec2(phi / 2 / pi, 1 - theta / pi);
  
    vec4 tc = texture(Texture0, -c.yx);
    OutColor = vec4(tc.bgr, 1);    

  //OutNormalIsShade = vec4(1, 1, 1, 0);
}

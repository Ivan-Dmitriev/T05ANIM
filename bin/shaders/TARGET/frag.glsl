/* */

// shader language version (3.3)
#version 420
 
// painting color
layout(location = 0) out vec4 OutColor;
 
// input params (varying)
in vec3 DrawPos;  
in vec2 DrawTexCoord;
in vec3 DrawNormal;  
in vec4 DrawColor;   

uniform mat4 MatrWVP;
uniform float Time;
uniform vec3 Ka, Kd, Ks;
uniform float Ph;

uniform vec3 CamLoc;

layout(binding = 0) uniform sampler2D Texture0;
uniform bool IsTexture0;
 
layout(binding = 1) uniform sampler2D Texture1;
uniform bool IsTexture1;

layout(binding = 2) uniform sampler2D Texture2;
uniform bool IsTexture2;


vec3 Shade( vec3 P, vec3 N, vec3 L, vec3 LColor )
{
  //float Ph = 100.0;
  vec3 V = normalize(P - CamLoc);
  vec3 color = vec3(0);

  // Reverse normal if need
  //N = faceforward(N, V, N);
  
  // Ambient
  //color += Ka;

  // Duffuse
  float nl = dot(N, L);
  nl = max(nl, 0);
  vec3 diff = LColor * Kd * nl;

  //if (IsTexture0)
  //diff *= texture(Texture0, DrawTexCoord).rgb;
   /*
   if (IsTexture1)
     diff *= texture(Texture1, DrawTexCoord).rgb;
   if (IsTexture2)
     diff *= texture(Texture1, DrawTexCoord).rgb;
   */

  color += diff;
 
  // Specular
  vec3 R = reflect(V, N);
  float rl = max(dot(R, L), 0);
  color += rl * LColor * Ks * pow(rl, Ph);

  return color;
}
 
void main( void )
{
  OutColor = vec4(1, 1, 1, 1);
  vec3 N = normalize(DrawNormal);
  vec3 L = normalize(vec3(10, 15, 1));  // light source
  //OutColor = vec4(Shade(DrawPos, N, 0.3 + L * vec3(1, 1.30, 1) * abs(cos(0.5 * Time)), vec3(1)), 1);
  //OutColor1 = vec4(N, 1);
  OutColor = vec4(texture(Texture0, DrawTexCoord).rgb, 1);
  //OutColor = vec4(1, 1, 1, 1);
  //OutColor = vec4(Shade(DrawPos, N, L, vec3(1)), 1);
  //OutColor = mix(DrawColor, OutColor, 0.5);//vec4(Shade(DrawPos, N, 0.3 + L * vec3(1, 1.30, 1), vec3(1)), 1);
  //OutColor = vec4(1, 1, 1, 1);
  //OutColor = DrawColor;
}
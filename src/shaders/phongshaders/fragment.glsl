#version 330 core
#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D uSampler;
uniform vec3 uKd;
uniform vec3 uKs;
uniform vec3 uLightPos;
uniform vec3 uCameraPos;
uniform float uLightIntensity;
uniform int uTextureSample;

in highp vec2 vTextureCoord;
in highp vec3 vFragPos_WS;
in highp vec3 vNormal_WS;

out vec4 FragColor;

void main() {
  vec3 color;
  if (uTextureSample == 1) {
    color = pow(texture2D(uSampler, vTextureCoord).rgb, vec3(2.2));
  } else {
    color = uKd;
  }
  
  vec3 ambient = 0.05 * color;

  vec3 lightDir = normalize(uLightPos - vFragPos_WS);
  vec3 normal = normalize(vNormal_WS);
  float diff = max(dot(lightDir, normal), 0.0);
  float light_atten_coff = uLightIntensity / length(uLightPos - vFragPos_WS);
  vec3 diffuse =  diff * light_atten_coff * color;

  vec3 viewDir = normalize(uCameraPos - vFragPos_WS);
  float spec = 0.0;
  vec3 reflectDir = reflect(-lightDir, normal);
  spec = pow (max(dot(viewDir, reflectDir), 0.0), 35.0);
  vec3 specular = uKs * light_atten_coff * spec;  
  
  FragColor = vec4(pow((ambient + diffuse + specular), vec3(1.0/2.2)), 1.0);
}


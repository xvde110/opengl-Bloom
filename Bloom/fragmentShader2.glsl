#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D colorBuffer;
uniform sampler2D hdrBuffer;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec2 tex_offset = 1.0 / textureSize(hdrBuffer, 0); // gets size of single texel
    vec3 result = texture(hdrBuffer, texCoord).rgb * weight[0]; // current fragment's contribution
    for(int i = 1; i < 5; ++i)
    {
        result += texture(hdrBuffer, texCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        result += texture(hdrBuffer, texCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    }
    for(int i = 1; i < 5; ++i)
    {
        result += texture(hdrBuffer, texCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        result += texture(hdrBuffer, texCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
    }
	color=vec4(result + texture(colorBuffer,texCoord).rgb,1.0);
}
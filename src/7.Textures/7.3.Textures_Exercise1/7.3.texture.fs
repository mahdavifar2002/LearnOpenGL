#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	vec4 tex1 = texture(texture1, TexCoord);
	vec4 tex2 = texture(texture2, TexCoord * 1.2 - vec2(0.1, 0.1));
	FragColor = mix(tex1, tex2, tex2.a);
}
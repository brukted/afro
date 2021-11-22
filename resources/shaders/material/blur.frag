#version 330 core
out vec4 FragColor;
in vec2 UV;

uniform sampler2D MainTex;
uniform float intensity;
uniform vec2 pixel_shape;

void main() {
    vec2 offset = 1.0 / textureSize(MainTex, 0);

    float whalf = intensity * 0.5;
    vec4 result = vec4(0);
	
	offset.y = offset.y * pixel_shape.y;
	offset.x = offset.x * pixel_shape.x;

    for(float j = -whalf; j <= whalf; j++) {
        result += texture(MainTex, UV + (j * offset));
    }

    FragColor = result / (intensity + 1);
}
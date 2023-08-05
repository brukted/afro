#version 330 core
out vec4 frag_color;
in vec2 UV;

uniform sampler2D input1;
uniform sampler2D input2;

uniform int channel_red = 0;
uniform int channel_green = 1;
uniform int channel_blue = 2;
uniform int channel_alpha = 3;

float get_channel(vec4 c1, vec4 c2, int channel) {
    switch (channel) {
        case 0:
        return c1.r;
        case 1:
        return c1.g;
        case 2:
        return c1.b;
        case 3:
        return c1.a;
        case 4:
        return c2.r;
        case 5:
        return c2.g;
        case 6:
        return c2.b;
        case 7:
        return c2.a;
    }
    return 0;
}

void main() {
    vec4 c = texture(input1, UV);
    vec4 c2 = texture(input2, UV);

    vec4 final = vec4(0);

    final.r = get_channel(c, c2, channel_red);
    final.g = get_channel(c, c2, channel_green);
    final.b = get_channel(c, c2, channel_blue);
    final.a = get_channel(c, c2, channel_alpha);

    frag_color = final;
}
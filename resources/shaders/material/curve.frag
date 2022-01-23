#version 330 core
out vec4 frag_color;
in vec2 UV;

uniform sampler2D input_tex;
uniform sampler1D lum_curve_lut;
uniform sampler1D main_curve_lut;
uniform bool apply_curve;

void main() {
    if(!apply_curve) {
        float y=  texture(lum_curve_lut, UV.x).r;
        frag_color = vec4(y, y, y, 1.0);
        return;
    }
    vec4 c = texture(input_tex, UV);
    float lum_l = texture(lum_curve_lut, (c.r+c.g+c.b)/3).r;
    float rr = texture(main_curve_lut, c.r).r;
    float gg = texture(main_curve_lut, c.g).g;
    float bb = texture(main_curve_lut, c.b).b;
    float aa = texture(main_curve_lut, c.a).a;

    frag_color = vec4(rr, gg, bb, 1.0);
}
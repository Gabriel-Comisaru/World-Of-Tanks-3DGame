#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 color;
in int hp_out;

// Output
out vec3 world_pos;
out vec3 world_normal;

layout(location = 0) out vec4 out_color;

uniform int hp;


void main()
{
    // TODO(student): Write pixel out color
    float healthFactor = float(hp) / 3.0;
    vec3 moddifiedColor;
    if (hp <= 3) {
		moddifiedColor = mix(color, vec3(1.0, 0.0, 0.0), 1.0 - healthFactor);
	} else {
		moddifiedColor = color;}
    out_color = vec4(moddifiedColor, 1);
}

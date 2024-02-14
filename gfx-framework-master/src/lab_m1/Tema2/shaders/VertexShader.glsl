#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;
uniform vec3 light_color;
uniform int hp;


// Output value to fragment shader
out vec3 color;


void main()
{
    vec3 deformedPos = v_position;
    if (hp <= 3) {
        float deformationFactor = (hp / 3.0);
        deformedPos.x *= deformationFactor;
    }

    vec3 world_pos = (Model* vec4(deformedPos, 1.0)).xyz;
    vec3 world_normal = normalize(mat3(Model) * normalize(v_normal));

    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_pos);
    vec3 V = normalize(eye_position - world_pos);
    vec3 R = normalize(L + V);

    float ambient_light = 0.25;

    float diffuse_light = material_kd * max(dot(N, L), 0.0);

    float specular_light = 0;

    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(normalize(N), R), 0.0), material_shininess);
    }

    float light = ambient_light + diffuse_light + specular_light;


    color = object_color * light;
    gl_Position = Projection * View * Model * vec4(deformedPos, 1.0);
}

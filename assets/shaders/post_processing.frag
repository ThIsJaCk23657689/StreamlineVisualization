#version 330 core
out vec4 FargColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int edge_kernel[9];
uniform float blur_kernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main() {
    FargColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec3 sampled[9];
    if (chaos || shake) {
       for (int i = 0; i < 9; i++) {
           sampled[i] = vec3(texture(scene, TexCoords.st + offsets[i]));
       }
    }

    if (chaos) {
        for (int i = 0; i < 9; i++) {
            FargColor += vec4(sampled[i] * edge_kernel[i], 0.0f);
        }
        FargColor.a = 1.0f;
    } else if (confuse) {
        FargColor = vec4(1.0f - texture(scene, TexCoords).rgb, 1.0f);
    } else if (shake) {
        for (int i = 0; i < 9; i++) {
            FargColor += vec4(sampled[i] * blur_kernel[i], 0.0f);
        }
        FargColor.a = 1.0f;
    } else {
        FargColor = texture(scene, TexCoords);
    }
}
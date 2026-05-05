#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    // Ambient (NEW)
    float ambient = 0.2;

    vec3 color = (ambient + diff) * vec3(1.0, 0.5, 0.3);
    //vec3 color = vec3(1.0, 0.3, 0.3) * diff;

    FragColor = vec4(color, 1.0);
    //FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0); to test if norma is broken
}

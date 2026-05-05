#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {

    // ----------------------------
    // 1. Normalize inputs
    // ----------------------------
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(lightPos - FragPos);

    // ----------------------------
    // 2. Basic Lighting
    // ----------------------------
    float diff = max(dot(norm, lightDir), 0.0);
    float ambient = 0.2;

    vec3 baseColor = (ambient + diff) * vec3(1.0, 0.5, 0.3);

    // ----------------------------
    // 3. LEVEL 1: Edge Enhancement (Fresnel)
    // ----------------------------
    float edge = 1.0 - max(dot(norm, viewDir), 0.0);

    // sharpen edge effect
    edge = pow(edge, 3.0);

    vec3 edgeColor = vec3(1.0); // white highlight

    vec3 colorWithEdges = mix(baseColor, edgeColor, edge);


    // ----------------------------
    // 4. LEVEL 2: Curvature Enhancement
    // ----------------------------
    float curvature = length(fwidth(norm));

    // amplify curvature
    curvature = clamp(curvature * 5.0, 0.0, 1.0);
    //curvature = clamp(curvature * 2.0, 0.0, 1.0);

    vec3 curvatureColor = vec3(1.0, 0.0, 0.0); // red highlight

    vec3 finalColor = mix(colorWithEdges, curvatureColor, curvature);

    // ----------------------------
    // 5. Output
    // ----------------------------
    FragColor = vec4(finalColor, 1.0);
}
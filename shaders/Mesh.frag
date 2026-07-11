#version 330 core

// ======================================
// Output color
// ======================================

out vec4 FragColor;

// ======================================
// Inputs from vertex shader
// ======================================

in vec3 Normal;
in vec3 FragPos;

// ======================================
// Uniforms
// ======================================

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform int tumorClass;

uniform float time;

// ======================================
// Tumor material colors
// ======================================

vec4 getTumorColor(int c)
{
    // ----------------------------------
    // Animated ET pulse
    // ----------------------------------

    float pulse =
        0.65 +
        0.35 *
        (
            0.5 +
            0.5 *
            sin(time * 2.5)
        );

    // ----------------------------------
    // Brain shell
    // ----------------------------------

    if(c == 0)
        return vec4(
            0.7,
            0.7,
            0.7,
            0.05
        );

    // ----------------------------------
    // Whole tumor (WT)
    // ----------------------------------

    if(c == 3)
        return vec4(
            0.0,
            0.55,
            1.0,
            0.08
        );

    // ----------------------------------
    // Tumor core (TC)
    // ----------------------------------

    if(c == 2)
        return vec4(
            1.0,
            0.7,
            0.0,
            0.22
        );

    // ----------------------------------
    // Enhancing tumor (ET)
    // ----------------------------------

    if(c == 1)
        return vec4(
            1.0,
            0.0,
            0.0,
            pulse
        );

    return vec4(1.0);
}

// ======================================
// Main shader
// ======================================

void main()
{
    // ----------------------------------
    // Normalize surface normal
    // ----------------------------------

    vec3 norm =
        normalize(Normal);

    // ----------------------------------
    // Lighting vectors
    // ----------------------------------

    vec3 lightDir =
        normalize(
            lightPos - FragPos
        );

    vec3 viewDir =
        normalize(
            viewPos - FragPos
        );

    // ==================================
    // Ambient lighting
    // ==================================

    float ambient = 0.25;

    // ==================================
    // Diffuse lighting
    // ==================================

    float diff =
        max(
            dot(norm, lightDir),
            0.0
        );

    // ==================================
    // Specular lighting
    // ==================================

    vec3 reflectDir =
        reflect(
            -lightDir,
            norm
        );

    float spec =
        pow(
            max(
                dot(viewDir, reflectDir),
                0.0
            ),
            32.0
        );

    float specStrength = 0.35;

    vec3 specular =
        specStrength *
        spec *
        vec3(1.0);

    // ==================================
    // Base tumor color
    // ==================================

    vec4 base =
        getTumorColor(tumorClass);

    // ==================================
    // Combined lighting
    // ==================================

    vec3 color =
        (
            ambient +
            diff
        ) * base.rgb +
        specular;

    // ==================================
    // Fresnel rim lighting
    // ==================================

    float edge =
        pow(
            1.0 -
            max(dot(viewDir, norm), 0.0),
            2.0
        );

    // WT rim
    if(tumorClass == 3)
    {
        color +=
            edge *
            vec3(0.0, 0.5, 1.0) *
            0.25;
    }

    // TC rim
    if(tumorClass == 2)
    {
        color +=
            edge *
            vec3(1.0, 0.7, 0.0) *
            0.35;
    }

    // ET rim
    if(tumorClass == 1)
    {
        color +=
            edge *
            vec3(1.0, 0.2, 0.2) *
            0.8;
    }

    // ==================================
    // Gamma correction
    // ==================================

    color =
        pow(
            color,
            vec3(1.0 / 2.2)
        );

    // ==================================
    // Soft medical fog
    // ==================================

    float dist =
        length(viewPos - FragPos);

    float fog =
        clamp(
            exp(-dist * 0.02),
            0.0,
            1.0
        );

    vec3 fogColor =
        vec3(
            0.12,
            0.12,
            0.14
        );

    color =
        mix(
            fogColor,
            color,
            fog
        );

    // ==================================
    // Final output
    // ==================================

    FragColor =
        vec4(
            color,
            base.a
        );
}
#version 430 core

in vec2 UV;
in float DensityNorm;
in float SpeedVal;


// ======================================
// BASELINE FRAGMENT SHADER
// --------------------------------------
// Traditional mesh rendering baseline
// used for comparison against the
// proposed boundary-aware framework.
//
// Disabled Features:
//  - Fresnel rim lighting
//  - Specular enhancement
//  - Transparency layering
//  - ET pulsation
//  - Gamma correction
//  - Medical fog
//
// Only basic:
//  - Ambient lighting
//  - Diffuse lighting
// remains enabled.
// ======================================

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
uniform int tumorClass;

// ======================================
// Basic tumor colors
// --------------------------------------
// Fixed opacity values are used.
// No animation or transparency effects.
// ======================================

vec4 getTumorColor(int c)
{
    // ----------------------------------
    // Brain shell
    // ----------------------------------

    if(c == 0)
    {
        return vec4(
            0.7,
            0.7,
            0.7,
            1.0
        );
    }

    // ----------------------------------
    // Whole Tumor (WT)
    // ----------------------------------

    if(c == 3)
    {
        return vec4(
            0.0,
            0.55,
            1.0,
            1.0
        );
    }

    // ----------------------------------
    // Tumor Core (TC)
    // ----------------------------------

    if(c == 2)
    {
        return vec4(
            1.0,
            0.7,
            0.0,
            1.0
        );
    }

    // ----------------------------------
    // Enhancing Tumor (ET)
    // ----------------------------------

    if(c == 1)
    {
        return vec4(
            1.0,
            0.0,
            0.0,
            1.0
        );
    }

    return vec4(1.0);
}

// ======================================
// Main Shader
// ======================================

void main()
{
    // ----------------------------------
    // Normalize surface normal
    // ----------------------------------

    vec3 norm =
        normalize(Normal);

    // ----------------------------------
    // Light direction
    // ----------------------------------

    vec3 lightDir =
        normalize(
            lightPos - FragPos
        );

    // ==================================
    // Ambient lighting
    // ----------------------------------
    // Simple constant illumination
    // ==================================

    float ambient = 0.25;

    // ==================================
    // Diffuse lighting
    // ----------------------------------
    // Lambertian shading model
    // ==================================

    float diff =
        max(
            dot(norm, lightDir),
            0.0
        );

    // ==================================
    // Base material color
    // ==================================

    vec4 base =
        getTumorColor(
            tumorClass
        );

    // ==================================
    // Final basic shading
    // ----------------------------------
    // No:
    //  - rim lighting
    //  - specular
    //  - fog
    //  - gamma correction
    // ==================================

    vec3 color =
        (
            ambient +
            diff
        ) * base.rgb;

    // ==================================
    // Final output
    // ==================================

    FragColor =
        vec4(
            color,
            1.0
        );
}
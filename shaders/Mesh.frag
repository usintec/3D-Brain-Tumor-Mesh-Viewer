#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    
    // ----------------------------
    // 1. Normalize inputs
    // ----------------------------
    vec3 norm=normalize(Normal);
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 lightDir=normalize(lightPos-FragPos);
    
    // ----------------------------
    // 2. Basic Lighting (Phong)
    // ----------------------------
    float diff=max(dot(norm,lightDir),0.);
    float ambient=.35;
    
    vec3 baseColor=vec3(.65,.65,.7);
    vec3 lighting=baseColor*(ambient+diff*.8);
    
    // ----------------------------
    // 3. Specular highlight
    // ----------------------------
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.),16.);
    vec3 specColor=vec3(1.,1.,1.)*spec*.4;
    
    // ----------------------------
    // 4. Rim highlight (boundary enhancement)
    // ----------------------------
    float rim=1.-max(dot(norm,viewDir),0.);
    rim=smoothstep(.3,1.,rim);
    vec3 rimColor=vec3(.8,.85,1.)*rim*.15;
    
    // ----------------------------
    // 5. Final Output
    // ----------------------------
    vec3 finalColor=lighting+specColor+rimColor;
    finalColor=clamp(finalColor,0.,1.);
    
    FragColor=vec4(finalColor,1.);
}
#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int tumorClass;

vec4 getTumorColor(int c)
{
    // RGBA (A = transparency)
    if(c==1)return vec4(1.,0.,0.,1.);// ET (solid)
    if(c==2)return vec4(1.,.5,0.,.6);// TC (semi)
    if(c==3)return vec4(0.,.5,1.,.2);// WT (very transparent)
    
    return vec4(1.);
}

void main()
{
    vec3 norm=normalize(Normal);
    
    vec3 lightDir=normalize(lightPos-FragPos);
    float diff=max(dot(norm,lightDir),0.);
    
    float ambient=.2;
    
    vec4 base=getTumorColor(tumorClass);
    
    vec3 viewDir=normalize(viewPos-FragPos);
    
    // boundary enhancement (Fresnel)
    float edge=pow(1.-max(dot(viewDir,norm),0.),2.);
    
    vec3 color=(ambient+diff)*base.rgb;
    
    // stronger boundary for ET
    if(tumorClass==1)
    color+=edge*vec3(1.);
    
    FragColor=vec4(color,base.a);
}
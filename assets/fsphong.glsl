#version 400

const int MAX_LIGHTS=64;

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;
in vec3 Tangent;
in vec3 BiTangent;

out vec4 FragColor;

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;
uniform sampler2D ShadowMapTexture[MAX_LIGHTS];
uniform mat4 ShadowMapMat[MAX_LIGHTS];

struct Light
{
    int Type;
    vec3 Color;
    vec3 Position;
    vec3 Direction;
    vec3 Attenuation;
    vec3 SpotRadius;
    int ShadowIndex;
};

uniform Lights
{
    int LightCount;
    Light lights[MAX_LIGHTS];
};

float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}

void main()
{
    
    vec4 DiffTex = texture( DiffuseTexture, Texcoord);
    if(DiffTex.a < 0.3f) discard;
    
    vec3 NormalTex = texture(NormalTexture, Texcoord).rgb;
    NormalTex.r = NormalTex.r * 2 - 1;
    NormalTex.g = NormalTex.g * 2 - 1;
    NormalTex.b = NormalTex.b * 2 - 1;
    
    mat3 tbnMatrix = mat3(Tangent, -BiTangent, Normal);
    vec3 N = normalize(tbnMatrix * NormalTex);
    
    vec3 E = normalize(EyePos-Position);
        
    vec3 DiffuseComponent = vec3(0);
    vec3 SpecularComponent = vec3(0);
    
    for (int i = 0; i < LightCount; i++) {
        Light light = lights[i];
        
        vec3 L = normalize(light.Position - Position);
        
        // Light Drop Off
        float Dist = length(light.Position-Position);
        float intensity = 1.0 / (light.Attenuation.x + light.Attenuation.y * Dist + light.Attenuation.z * Dist * Dist);
        
        
        // Calculate Shadows
        float visibility = 1.0;
        
        if(light.ShadowIndex >= 0){
            
            vec4 PosSM = ShadowMapMat[i] * vec4(Position.xyz,1);
            PosSM.xyz /= PosSM.w;
            PosSM.xy = PosSM.xy * 0.5 + 0.5;
            vec4 DepthSM = texture(ShadowMapTexture[i], PosSM.xy);
            
            if (DepthSM.r < PosSM.z) {
                visibility = 0.0;
            }
        }
        
        vec3 Color = vec3(0,0,0);
        switch (light.Type) {
                // Point Light
            case 0:
                Color += intensity * light.Color;
                break;
                // Directional Light
            case 1:
                L = -normalize(light.Direction);
                Color += visibility * light.Color;
                break;
                // Spot Light
            case 2:
                float omega = acos(dot(normalize(light.Direction), normalize(-L)));
                
                if(omega <= light.SpotRadius.x){
                    Color += light.Color;
                }else if(omega <= light.SpotRadius.y){
                    float x = (omega - light.SpotRadius.x) / (light.SpotRadius.y - light.SpotRadius.x);
                    Color += (1.0f - sat(x)) * light.Color;
                }
                
                Color *= intensity * visibility;
                break;
            default:
                break;
        }
        
        vec3 H = normalize(L + E);
        DiffuseComponent += DiffuseColor * Color * sat(dot(N,L));
        SpecularComponent += SpecularColor  * Color * pow( sat(dot(N,H)), SpecularExp);
        
        
    }
    
    FragColor = vec4((DiffuseComponent + AmbientColor)*DiffTex.rgb + SpecularComponent, DiffTex.a);
    
}

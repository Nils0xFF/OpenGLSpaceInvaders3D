#version 330

const int MAX_LIGHTS=64;

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;
in vec3 Tangent;
in vec3 BiTangent;
in vec4 ViewSpace;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 DepthColor;

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

uniform float WorldDepth;
const float near = 0.1;

float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    float linearDepth = (2.0 * near * WorldDepth) / (z * (WorldDepth - near) - (WorldDepth + near));
    return (near + linearDepth)/(near - WorldDepth);
}

vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

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
    
    mat3 tbnMatrix = mat3(Tangent, BiTangent, Normal);
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
            
            vec4 PosSM = ShadowMapMat[light.ShadowIndex] * vec4(Position.xyz,1);
            PosSM.xy /= PosSM.w;
            PosSM.xy = PosSM.xy * 0.5 + 0.5;

            float closestDepth = texture(ShadowMapTexture[light.ShadowIndex], PosSM.xy).r;
            float currentDepth = PosSM.z;
            if(light.Type == 1){
                L = -normalize(light.Direction);
            }                
            float bias = 0.005 * (sat(dot(N, L)));

            for (int j=0;j<4;j++){
                float closestDepth = texture(ShadowMapTexture[light.ShadowIndex], PosSM.xy + poissonDisk[j]/1000.0).r;
                if(closestDepth < currentDepth - bias){
                    visibility -= 0.2;
                }
            }


        }
        
        vec3 Color = vec3(0,0,0);
        switch (light.Type) {
                // Point Light
            case 0:
                Color = intensity * light.Color;
                break;
                // Directional Light
            case 1:
                L = -normalize(light.Direction);
                Color = visibility * light.Color;
                break;
                // Spot Light
            case 2:
                float omega = acos(dot(normalize(light.Direction), normalize(-L)));
                
                if(omega <= light.SpotRadius.x){
                    Color = light.Color;
                }else if(omega <= light.SpotRadius.y){
                    float x = (omega - light.SpotRadius.x) / (light.SpotRadius.y - light.SpotRadius.x);
                    Color = (1.0f - sat(x)) * light.Color;
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
    
    FragColor = vec4((DiffuseComponent + AmbientColor) * DiffTex.rgb + SpecularComponent, DiffTex.a);
    
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);    
    float brightness = dot(FragColor.rgb, 1.0 * vec3(0.2126, 0.7152, 0.0722));    
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
         
    
    float dist = 2 * (vec3(0,0,WorldDepth)-Position).z - 1.0;
    vec3 projected = Position - dist * vec3(0,0,1);
    DepthColor = vec4(vec3(1, projected.y / WorldDepth, length(ViewSpace) / WorldDepth), 1.0);    
}


static const float PI = 3.14159265358979323846;

static const float subsurface = 0.0f;
static const float specular = 0.5f;
static const float specularTint = 0.0f;
static const float sheen = 0.0f;
static const float sheenTint = 0.5f;
static const float clearcoat = 0.0f;
static const float clearcoatGloss = 1.0f;

float sqr(float x) { return x*x; }

float SchlickFresnel(float u)
{
    float m = clamp(1.0f-u, 0.0f, 1.0f);
    float m2 = m*m;
    return m2*m2*m; // pow(m,5)
}

float GTR1(float NdotH, float a)
{
    if (a >= 1.0f) return 1.0f/PI;
    float a2 = a*a;
    float t = 1.0f + (a2-1.0f)*NdotH*NdotH;
    return (a2-1.0f) / (PI*log(a2)*t);
}

float GTR2(float NdotH, float a)
{
    float a2 = a*a;
    float t = 1.0f + (a2-1.0f)*NdotH*NdotH;
    return a2 / (PI * t*t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
	float t = sqr(HdotX/ax);
    return 1 / ( PI * ax*ay * sqr( t + sqr(HdotY/ay) + NdotH*NdotH ));
}

float smithG_GGX(float Ndotv, float alphaG)
{
    float a = alphaG*alphaG;
    float b = Ndotv*Ndotv;
    float t = Ndotv + sqrt(a + b - a*b);

    return 1.0f/t;
}

float3 mon2lin(float3 x)
{
    return float3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}

/*!
 * @brief	BRDF
 *@param[in]	L				光源までのベクトル。
 *@param[in]	V				視線までのベクトル。
 *@param[in]	normal			法線
 *@param[in]	tangentNormal	接法線
 *@param[in]	biNormal		従法線。
 */
float3 BRDF( 
	float3 L, 
	float3 V, 
	float3 normal, 
	float3 tangentNormal, 
	float3 biNormal, 
	float3 baseColor, 
	float roughness,
	float metallic
)
{
    float NdotL = dot(normal,L);

    float NdotV = max(0.5f, dot(normal,V));
    if (NdotL < 0 ) return 0.0f;

    float3 H = normalize(L+V);

    float NdotH = dot(normal,H);
    float LdotH = dot(L,H);
    
    
    float3 Cdlin = mon2lin(baseColor);
    float Cdlum = 0.3f*Cdlin[0] + 0.6f*Cdlin[1]  + 0.1f*Cdlin[2]; // luminance approx.

    float3 Ctint = Cdlum > 0 ? Cdlin/Cdlum : float3(1.0f, 1.0f, 1.0f); // normalize lum. to isolate hue+sat
    float3 Cspec0 = lerp(specular*0.08f*lerp(float3(1.0f, 1.0f, 1.0f), Ctint, specularTint), Cdlin, metallic);
    float3 Csheen = lerp(float3(1.0f, 1.0f, 1.0f), Ctint, sheenTint);

    // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
    // and lerp in diffuse retro-reflection based on roughness
    float FL = SchlickFresnel(NdotL), FV = SchlickFresnel(NdotV);
    float Fd90 = 0.5f + 2.0f * LdotH*LdotH * roughness;
    float Fd = lerp(1.0f, Fd90, FL) * lerp(1.0f, Fd90, FV) ;

    // Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf
    // 1.25 scale is used to (roughly) preserve albedo
    // Fss90 used to "flatten" retroreflection based on roughness
    float Fss90 = LdotH*LdotH*roughness;
    float Fss = lerp(1.0f, Fss90, FL) * lerp(1.0f, Fss90, FV);
    float ss = 1.25f * (Fss * (1.0f / (NdotL + NdotV) - 0.5f) + 0.5f);

    // specular
    float aspect = sqrt(1-anisotropic*0.9f);
    float ax = max(.001, sqr(roughness)/aspect);
    float ay = max(.001, sqr(roughness	)*aspect);
    float Ds = GTR2_aniso(NdotH, dot(H, tangentNormal), dot(H, biNormal), ax, ay);
    float FH = SchlickFresnel(LdotH);
    float3 Fs = lerp(Cspec0, float3(1.0f, 1.0f, 1.0f), FH);
    float roughg = sqr(roughness*.5+.5);
    float Gs = smithG_GGX(NdotL, roughg) * smithG_GGX(NdotV, roughg);

    // sheen
    float3 Fsheen = FH * sheen * Csheen;

    // clearcoat (ior = 1.5 -> F0 = 0.04)
    float Dr = GTR1(NdotH, lerp(0.1f,0.001f,clearcoatGloss));
    float Fr = lerp(0.04, 1.0f, FH);
    float Gr = smithG_GGX(NdotL, 0.25f) * smithG_GGX(NdotV, 0.25f);
    
    float3 lig = ((1.0f/PI) * lerp(Fd, ss, subsurface)*Cdlin + Fsheen)	//ディフューズ。
        * (1.0f-metallic)											//ディフューズ。
        + (Gs*Fs*Ds + 0.25f*clearcoat*Gr*Fr*Dr) * metallic;			//スペキュラ。
    lig = max(0.0f, lig);
    return lig;
}

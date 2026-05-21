Shader "Custom/Gummy"
{
    Properties
    {
        // --- Base ---
        _Color          ("Base Color",          Color)      = (0.4, 0.8, 0.4, 1)
        _MainTex        ("Albedo (RGB)",         2D)        = "white" {}

        // --- Superficie gomosa ---
        _Glossiness     ("Smoothness",           Range(0,1)) = 0.92
        _Metallic       ("Metallic",             Range(0,1)) = 0.0
        _SpecColor      ("Specular Color",       Color)      = (1, 1, 1, 1)
        _SpecPower      ("Specular Power",       Range(1, 512)) = 128

        // --- Rim / Fresnel ---
        _RimColor       ("Rim Color",            Color)      = (0.6, 1.0, 0.8, 1)
        _RimPower       ("Rim Power",            Range(0.1, 8.0)) = 2.5
        _RimStrength    ("Rim Strength",         Range(0, 2)) = 1.0

        // --- Subsurface Scattering (fake) ---
        _SSSColor       ("SSS Color",            Color)      = (0.8, 1.0, 0.6, 1)
        _SSSStrength    ("SSS Strength",         Range(0, 1)) = 0.4
        _SSSPower       ("SSS Power (tightness)",Range(1, 16)) = 4.0

        // --- Refracción interna (fake) ---
        _InnerGlow      ("Inner Glow Strength",  Range(0, 1)) = 0.2
        _InnerGlowColor ("Inner Glow Color",     Color)      = (0.5, 1.0, 0.5, 1)

        // --- Vertex Pulse (squish) ---
        _PulseSpeed     ("Pulse Speed",          Range(0, 5)) = 1.0
        _PulseStrength  ("Pulse Strength",       Range(0, 0.05)) = 0.01
        
        // --- Procedural Texture ---
        _NoiseScale        ("Noise Scale",           Range(0.1, 10))  = 2.0
        _NoiseStrength     ("Noise Normal Strength", Range(0, 2))     = 0.4
        _NoiseSpeed        ("Noise Animation Speed", Range(0, 2))     = 0.3

        _VoronoiScale      ("Voronoi Scale",         Range(0.1, 20))  = 5.0
        _VoronoiStrength   ("Voronoi Depth",         Range(0, 1))     = 0.15
        _VoronoiSharpness  ("Voronoi Sharpness",     Range(1, 8))     = 3.0

        _ProceduralTiling  ("Procedural Tiling",     Range(0.1, 5))   = 1.0
    }

    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 200

        CGPROGRAM

        // Surface shader con lighting custom
        #pragma surface surf GummySpecular vertex:vert fullforwardshadows nolightmap
        #pragma target 3.0

        #include "UnityCG.cginc"
        #include "Lighting.cginc"

        // ─────────────────────────────────────────
        // PROPIEDADES
        // ─────────────────────────────────────────
        sampler2D _MainTex;
        fixed4    _Color;
        half      _Glossiness;
        half      _Metallic;
        half      _SpecPower;

        fixed4    _RimColor;
        half      _RimPower;
        half      _RimStrength;

        fixed4    _SSSColor;
        half      _SSSStrength;
        half      _SSSPower;

        half      _InnerGlow;
        fixed4    _InnerGlowColor;

        half      _PulseSpeed;
        half      _PulseStrength;

        half _NoiseScale;
        half _NoiseStrength;
        half _NoiseSpeed;
        half _VoronoiScale;
        half _VoronoiStrength;
        half _VoronoiSharpness;
        half _ProceduralTiling;

        // ─────────────────────────────────────────
        // STRUCTS
        // ─────────────────────────────────────────
        struct Input
        {
            float2 uv_MainTex;
            float3 viewDir;
            float3 worldNormal;
            float3 worldPos;
            INTERNAL_DATA
        };

        struct SurfaceOutputGummy
        {
            fixed3 Albedo;
            fixed3 Normal;
            fixed3 Emission;
            fixed3 Specular;
            half   Gloss;
            half   Alpha;

            // Custom: datos que pasamos al lighting function
            fixed3 RimColor;
            half   RimStrength;
            fixed3 SSSColor;
            half   SSSStrength;
            half   SSSPower;
        };

        // ─────────────────────────────────────────
        // CUSTOM LIGHTING FUNCTION
        // ─────────────────────────────────────────
        half4 LightingGummySpecular(SurfaceOutputGummy s,
                                    half3 lightDir,
                                    half3 viewDir,
                                    half atten)
        {
            // --- Diffuse estándar ---
            half NdotL = max(0, dot(s.Normal, lightDir));
            half3 diffuse = s.Albedo * _LightColor0.rgb * NdotL * atten;

            // --- Specular Blinn-Phong (brillante como un globo) ---
            half3 halfVec = normalize(lightDir + viewDir);
            half  NdotH   = max(0, dot(s.Normal, halfVec));
            half3 specular = s.Specular * _LightColor0.rgb
                           * pow(NdotH, _SpecPower) * atten;

            // --- Fake SSS: luz que "traspasa" en dirección opuesta ---
            // Simula que la luz atraviesa el material gomoso
            half  VdotL   = max(0, dot(viewDir, -lightDir));
            half3 sss     = s.SSSColor * _LightColor0.rgb
                           * pow(VdotL, s.SSSPower) * s.SSSStrength * atten;

            // --- Rim / Fresnel ---
            half rim      = 1.0 - saturate(dot(viewDir, s.Normal));
            half3 rimTerm = s.RimColor * pow(rim, _RimPower) * s.RimStrength;

            half4 c;
            c.rgb = diffuse + specular + sss + rimTerm;
            c.a   = s.Alpha;
            return c;
        }


        // ─────────────────────────────────────────
        // PROCEDURAL HELPERS
        // ─────────────────────────────────────────

        // Hash sin trigonometría — rápido en GPU
        float2 hash2(float2 p)
        {
            p = float2(dot(p, float2(127.1, 311.7)),
                       dot(p, float2(269.5, 183.3)));
            return frac(sin(p) * 43758.5453);
        }

        // Voronoi (distancia a celda más cercana)
        // Devuelve: x = distancia al punto, y = distancia al borde
        float2 voronoi(float2 uv)
        {
            float2 cell  = floor(uv);
            float2 local = frac(uv);

            float  minDist  = 8.0;
            float  minDist2 = 8.0;

            for (int y = -1; y <= 1; y++)
            for (int x = -1; x <= 1; x++)
            {
                float2 neighbor = float2(x, y);
                float2 pointanim = hash2(cell + neighbor);
                // Anima los puntos Voronoi en el tiempo → superficie "viva"
                pointanim = 0.5 + 0.5 * sin(_Time.y * _NoiseSpeed + 6.2831 * pointanim);

                float2 diff = neighbor + pointanim - local;
                float  d    = dot(diff, diff); // distancia al cuadrado (más barato)

                if (d < minDist)
                {
                    minDist2 = minDist;
                    minDist  = d;
                }
                else if (d < minDist2)
                {
                    minDist2 = d;
                }
            }

            return float2(sqrt(minDist), sqrt(minDist2));
        }

        // Perlin Noise 2D — smooth interpolation con gradientes
        float perlinNoise(float2 uv)
        {
            float2 cell  = floor(uv);
            float2 local = frac(uv);

            // Smoothstep cúbico (C2 continuo → gradientes suaves)
            float2 u = local * local * (3.0 - 2.0 * local);

            float a = dot(hash2(cell + float2(0,0)) * 2 - 1, local - float2(0,0));
            float b = dot(hash2(cell + float2(1,0)) * 2 - 1, local - float2(1,0));
            float c = dot(hash2(cell + float2(0,1)) * 2 - 1, local - float2(0,1));
            float d = dot(hash2(cell + float2(1,1)) * 2 - 1, local - float2(1,1));

            return lerp(lerp(a, b, u.x),
                        lerp(c, d, u.x), u.y) * 0.5 + 0.5; // normalizado [0,1]
        }

        // Construye normal tangente a partir de un heightmap procedural
        // Usa diferencias finitas: muestra el noise en ±epsilon y calcula gradiente
        float3 proceduralNormal(float2 uv, float strength)
        {
            float eps = 0.01; // aumentado — con 0.005 la diferencia era casi imperceptible

            float2 animUV = uv + _Time.y * _NoiseSpeed * 0.1;

            float hC = perlinNoise(animUV * _NoiseScale);
            float hR = perlinNoise((animUV + float2(eps, 0))   * _NoiseScale);
            float hU = perlinNoise((animUV + float2(0,   eps)) * _NoiseScale);

            float2 vC = voronoi(animUV * _VoronoiScale);
            float2 vR = voronoi((animUV + float2(eps, 0))   * _VoronoiScale);
            float2 vU = voronoi((animUV + float2(0,   eps)) * _VoronoiScale);

            float voronoiEdge  = pow(saturate(vC.y - vC.x), _VoronoiSharpness);
            float voronoiEdgeR = pow(saturate(vR.y - vR.x), _VoronoiSharpness);
            float voronoiEdgeU = pow(saturate(vU.y - vU.x), _VoronoiSharpness);

            // Gradiente: diferencia entre muestra desplazada y centro
            // Signo corregido para tangent space de Unity (X→right, Y→up)
            float dX = (hR + voronoiEdgeR * _VoronoiStrength)
                      - (hC + voronoiEdge  * _VoronoiStrength);
            float dY = (hU + voronoiEdgeU * _VoronoiStrength)
                      - (hC + voronoiEdge  * _VoronoiStrength);

            // Devuelve XY del desplazamiento; Z se reconstruye en surf()
            return float3(dX, dY, 1.0);
        }
        
        // ─────────────────────────────────────────
        // VERTEX FUNCTION — Pulse suave
        // ─────────────────────────────────────────
        void vert(inout appdata_full v)
        {
            // Desplazamiento sinusoidal a lo largo de la normal
            // Da sensación de que el personaje "respira" o es blando
            /*
            float pulse = sin(_Time.y * _PulseSpeed) * _PulseStrength;
            v.vertex.xyz += v.normal * pulse;
            */
        }

        // ─────────────────────────────────────────
        // SURFACE FUNCTION
        // ─────────────────────────────────────────
        void surf(Input IN, inout SurfaceOutputGummy o)
        {
            fixed4 tex  = tex2D(_MainTex, IN.uv_MainTex);
            fixed4 base = tex * _Color;
            o.Albedo    = base.rgb;
            o.Alpha     = base.a;

            o.Specular  = _SpecColor.rgb * _Glossiness;
            o.Gloss     = _Glossiness;

            half fresnel = 1.0 - saturate(dot(normalize(IN.viewDir),
                                                  WorldNormalVector(IN, o.Normal)));
            half fresnelTerm = pow(fresnel, _RimPower);
            o.Emission       = _InnerGlowColor.rgb * _InnerGlow * fresnelTerm;

            // ── PROCEDURAL NORMAL ──────────────────────────────────────────────
            float2 proceduralUV = IN.uv_MainTex * _ProceduralTiling;
            float3 proceduralN  = proceduralNormal(proceduralUV, _NoiseStrength);

            // Built-In Surface Shader espera la normal en tangent space normalizada.
            // XY son el desplazamiento tangencial, Z es la profundidad (nunca 0).
            // Sin saturar Z el normalize puede producir NaN si _NoiseStrength es muy alto.
            float2 tN  = proceduralN.xy * _NoiseStrength;
            float  tNz = sqrt(max(0.001, 1.0 - dot(tN, tN))); // Z reconstruida correctamente
            o.Normal   = float3(tN, tNz);                      // ya normalizado por construcción
            // ──────────────────────────────────────────────────────────────────

            o.RimColor    = _RimColor.rgb;
            o.RimStrength = _RimStrength;
            o.SSSColor    = _SSSColor.rgb;
            o.SSSStrength = _SSSStrength;
            o.SSSPower    = _SSSPower;
        }

        ENDCG
    }

    FallBack "Diffuse"
}

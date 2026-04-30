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
    }

    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 200

        CGPROGRAM

        // Surface shader con lighting custom
        #pragma surface surf GummySpecular vertex:vert fullforwardshadows
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
            // Base color
            fixed4 tex  = tex2D(_MainTex, IN.uv_MainTex);
            fixed4 base = tex * _Color;
            o.Albedo    = base.rgb;
            o.Alpha     = base.a;

            // Specular y gloss
            o.Specular  = _SpecColor.rgb * _Glossiness;
            o.Gloss     = _Glossiness;

            // Fresnel → rim visible desde superficie
            half fresnel    = 1.0 - saturate(dot(normalize(IN.viewDir),
                                                  WorldNormalVector(IN, o.Normal)));
            half fresnelTerm = pow(fresnel, _RimPower);

            // Inner glow: emissión interna que simula luz atrapada
            // (como un globo traslúcido con luz dentro)
            o.Emission  = _InnerGlowColor.rgb * _InnerGlow * fresnelTerm;

            // Pasamos datos al lighting function
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

__int64 __fastcall sub_180023F00(__int64 a1, __int64 a2)
{
  __int64 *v2; // rdi
  __int64 i; // rcx
  unsigned __int64 v4; // rax
  __int64 v5; // rax
  __int64 v6; // rax
  __int64 v7; // rax
  unsigned __int64 v9; // rax
  __int64 v10; // rax
  __int64 v11; // rax
  __int64 v12; // rax
  __int64 v13; // rax
  __int64 v14; // rax
  __int64 v15; // r9
  __int64 v16; // r9
  __int64 v17; // [rsp+0h] [rbp-3F8h] BYREF
  _BYTE v18[72]; // [rsp+28h] [rbp-3D0h] BYREF
  _BYTE v19[16]; // [rsp+70h] [rbp-388h] BYREF
  _BYTE v20[248]; // [rsp+80h] [rbp-378h] BYREF
  __int64 v21; // [rsp+178h] [rbp-280h]
  unsigned int j; // [rsp+180h] [rbp-278h]
  _BYTE v23[48]; // [rsp+188h] [rbp-270h] BYREF
  _BYTE v24[72]; // [rsp+1B8h] [rbp-240h] BYREF
  _BYTE v25[16]; // [rsp+200h] [rbp-1F8h] BYREF
  _BYTE v26[244]; // [rsp+210h] [rbp-1E8h] BYREF
  unsigned int k; // [rsp+304h] [rbp-F4h]
  _BYTE v28[40]; // [rsp+308h] [rbp-F0h] BYREF
  _BYTE v29[40]; // [rsp+330h] [rbp-C8h] BYREF
  _BYTE v30[48]; // [rsp+358h] [rbp-A0h] BYREF
  __int64 v31; // [rsp+388h] [rbp-70h]
  unsigned __int64 v32; // [rsp+390h] [rbp-68h]
  __int64 v33; // [rsp+398h] [rbp-60h]
  __int64 v34; // [rsp+3A0h] [rbp-58h]
  __int64 v35; // [rsp+3A8h] [rbp-50h]
  __int64 v36; // [rsp+3B0h] [rbp-48h]
  int v37; // [rsp+3B8h] [rbp-40h]
  unsigned __int64 v38; // [rsp+3C0h] [rbp-38h]
  __int64 v39; // [rsp+3C8h] [rbp-30h]
  __int64 v40; // [rsp+3D0h] [rbp-28h]
  __int64 v41; // [rsp+3D8h] [rbp-20h]
  __int64 v42; // [rsp+3E0h] [rbp-18h]

  v2 = &v17;
  for ( i = 252; i; --i )
  {
    *(_DWORD *)v2 = -858993460;
    v2 = (__int64 *)((char *)v2 + 4);
  }
  v31 = -2;
  sub_1800026FD(
    v18,
    "uniform vec3 grindBaseClr;\n"
    "uniform float grindResectPos;\n"
    "uniform vec3 grindResectClr;\n"
    "uniform vec3 grindExpandDefaultClr;\n"
    "uniform mat4 grindMatrix;\n");
  sub_180002487(v19, 3, 1);
  v21 = sub_180001DE8(*(_QWORD *)(a1 + 80));
  for ( j = 0; ; ++j )
  {
    v32 = (int)j;
    v4 = sub_180001DF2(v21);
    if ( v32 >= v4 )
      break;
    v33 = sub_1800017EE(v20, "uniform float expandLen");
    v5 = std::ostream::operator<<(v33, j);
    v6 = sub_1800017EE(v5, ";\n");
    v34 = sub_1800017EE(v6, "uniform vec3 expandClr");
    v7 = std::ostream::operator<<(v34, j);
    sub_1800017EE(v7, ";\n");
  }
  v35 = sub_180001ACD(v19, v23);
  v36 = v35;
  sub_180001E33(v18, v35);
  sub_180001B8B(v23);
  sub_1800026FD(
    v24,
    "    if (isface) {\n        clr = grindBaseClr;\n    } else {\n        clr = grindExpandDefaultClr;\n    }\n");
  if ( !(unsigned int)sub_1800018AC(*(_QWORD *)(a1 + 80)) )
  {
    sub_180002207(v18, "uniform vec4 grindGeomSize;\n");
    v37 = sub_180001424(*(_QWORD *)(a1 + 80));
    switch ( v37 )
    {
      case 0:
        sub_180002207(
          v18,
          "\n"
          "float sdSphere(in vec3 p, in vec4 checkSize) {\n"
          "    vec3 o = (grindMatrix * vec4(p, 1)).xyz;\n"
          "    return length(o.xyz) - checkSize.x;\n"
          "}\n"
          "\n");
        sub_180002207(v24, "    float dist = sdSphere(worldPos, grindGeomSize);\n");
        goto LABEL_21;
      case 1:
        sub_180002207(
          v18,
          "\n"
          "float sdBox(in vec3 p, in vec4 checkSize){\n"
          "    vec3 o = (grindMatrix * vec4(p, 1)).xyz;\n"
          "    vec3 b = checkSize.xyz;\n"
          "    vec3 q = abs(o.xyz) - b;\n"
          "    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);\n"
          "}\n"
          "\n");
        sub_180002207(v24, "    float dist = sdBox(worldPos, grindGeomSize);\n");
        goto LABEL_21;
      case 2:
        sub_180002207(
          v18,
          "\n"
          "float sdRoundBox(in vec3 p, in vec4 checkSize) {\n"
          "    vec3 o = (grindMatrix * vec4(p, 1)).xyz;\n"
          "    vec3 size = checkSize.xyz;\n"
          "    float r = checkSize.w;\n"
          "    vec3 q = abs(o.xyz) - size;\n"
          "    float outside = length(max(q, 0.0));\n"
          "    float inside = min(max(q.x, max(q.y, q.z)), 0.0);\n"
          "    return outside + inside - r;\n"
          "}\n"
          "\n");
        sub_180002207(v24, "    float dist = sdRoundBox(worldPos, grindGeomSize);\n");
        goto LABEL_21;
      case 3:
        sub_180002207(
          v18,
          "\n"
          "float sdCapsule( vec3 p, in vec4 checkSize ) {\n"
          "   vec3 o = (grindMatrix * vec4(p, 1)).xyz;\n"
          "   float r = checkSize.x;\n"
          "   float h = checkSize.y;\n"
          "   o.z -= clamp(o.z, -h, h);\n"
          "   return length(o) - r;\n"
          "}\n"
          "\n");
        sub_180002207(v24, "    float dist = sdCapsule(worldPos, grindGeomSize);\n");
        goto LABEL_21;
      case 4:
        sub_180002207(
          v18,
          "\n"
          "float sdPlane( vec3 p, in vec4 checkSize ) {\n"
          "   vec3 o = (grindMatrix * vec4(p, 1)).xyz;\n"
          "   return checkSize.x - o.z;\n"
          "}\n"
          "\n");
        sub_180002207(v24, "    float dist = sdPlane(worldPos, grindGeomSize);\n");
        goto LABEL_21;
      case 5:
      case 6:
        sub_180002207(
          v18,
          "\n"
          "layout(std430, binding = 1) buffer PolygonBuffer\n"
          "{\n"
          "    vec2 PG_Points[];\n"
          "};\n"
          "uniform int PGP_Count;\n"
          "float sdSegment2D(vec2 p, vec2 a, vec2 b) {\n"
          "    vec2 pa = p - a;\n"
          "    vec2 ba = b - a;\n"
          "    float baba = dot(ba, ba);\n"
          "    if (baba < 1e-8)\n"
          "       return length(pa);\n"
          "    float h = clamp(dot(pa, ba) / baba, 0.0, 1.0);\n"
          "    return length(pa - ba * h);\n"
          "}\n"
          "float sdPolygon2D(vec2 p) {\n"
          "    float d = 1e20;\n"
          "    if (PGP_Count < 3)\n"
          "        return d;\n"
          "    bool inside = false;\n"
          "    for (int i = 0; i < PGP_Count; ++i) {\n"
          "        vec2 a = PG_Points[i];\n"
          "        vec2 b = PG_Points[(i + 1) % PGP_Count];\n"
          "        d = min(d, sdSegment2D(p, a, b));\n"
          "        float py = p.y + 1e-6;\n"
          "        bool cond1 = (a.y > py) != (b.y > py);\n"
          "        if (cond1) {\n"
          "            float t = (py - a.y) / (b.y - a.y);\n"
          "            float x = mix(a.x, b.x, t);\n"
          "            if (p.x < x)\n"
          "                inside = !inside;\n"
          "        }\n"
          "    }\n"
          "    return inside ? -d : d;\n"
          "}\n");
        if ( (unsigned int)sub_180001424(*(_QWORD *)(a1 + 80)) == 5 )
        {
          sub_180002207(
            v18,
            "float sd2DAxisDilate(vec3 p, in vec4 checkSize) {\n"
            "    vec3 o = (grindMatrix * vec4(p, 1)).xyz;\n"
            "    float height = checkSize.x;\n"
            "    int axis = int(floor(checkSize.y+0.5));\n"
            "    float d2 = 1e20;\n"
            "    float dh = 1e20;\n"
            "    if (0 == axis){\n"
            "        d2 = sdPolygon2D(o.yz);\n"
            "        dh = abs(o.x) - height;\n"
            "    } else if (1 == axis){\n"
            "        d2 = sdPolygon2D(o.xz);\n"
            "        dh = abs(o.y) - height;\n"
            "    } else if (2 == axis){\n"
            "        d2 = sdPolygon2D(o.xy);\n"
            "        dh = abs(o.z) - height;\n"
            "    }\n"
            "    return max(d2, dh);\n"
            "}\n"
            "\n");
          sub_180002207(v24, "    float dist = sd2DAxisDilate(worldPos, grindGeomSize);\n");
        }
        else
        {
          sub_180002207(
            v18,
            "float sdRound2DAxisDilate(vec3 p, in vec4 checkSize) {\n"
            "    vec3 o = (grindMatrix * vec4(p, 1)).xyz;\n"
            "    float radius = checkSize.x;\n"
            "    float height = checkSize.y;\n"
            "    int axis = int(floor(checkSize.z+0.5));\n"
            "    float d2 = 1e20;\n"
            "    float dh = 1e20;\n"
            "    if (0 == axis){\n"
            "        d2 = sdPolygon2D(o.yz);\n"
            "        dh = abs(o.x) - height;\n"
            "    } else if (1 == axis){\n"
            "        d2 = sdPolygon2D(o.xz);\n"
            "        dh = abs(o.y) - height;\n"
            "    } else if (2 == axis){\n"
            "        d2 = sdPolygon2D(o.xy);\n"
            "        dh = abs(o.z) - height;\n"
            "    }\n"
            "    return length(max(vec2(d2, dh), 0.0)) - radius + min(max(d2, dh), 0.0);\n"
            "}\n"
            "\n");
          sub_180002207(v24, "    float dist = sdRound2DAxisDilate(worldPos, grindGeomSize);\n");
        }
        goto LABEL_21;
      default:
        goto LABEL_17;
    }
  }
  if ( (unsigned int)sub_1800018AC(*(_QWORD *)(a1 + 80)) == 1 )
  {
    sub_180002207(
      v18,
      "uniform sampler3D grindTex;\nuniform vec3 grindOrigin;\nuniform vec3 grindSpacing;\nuniform vec3 grindSize;\n\n");
    sub_180002207(
      v18,
      "\n"
      "vec3 grindWPToTP(vec3 worldPos) {\n"
      "   vec3 voxelCoords = ((worldPos - grindOrigin) / grindSpacing) + 0.5;\n"
      "   vec3 texCoords = voxelCoords / grindSize;\n"
      "   return clamp(texCoords, 0.0, 1.0);\n"
      "}\n"
      "\n"
      "float sdTex(in vec3 p) {\n"
      "    vec3 o = (grindMatrix * vec4(p, 1)).xyz;\n"
      "    vec3 tp = grindWPToTP(o.xyz);\n"
      "    return texture(grindTex, tp).r;\n"
      "}\n"
      "\n");
    sub_180002207(v24, "   float dist = sdTex(worldPos);\n");
LABEL_21:
    sub_180002207(
      v24,
      "    if (dist <= grindResectPos) {\n"
      "        clr = grindResectClr;\n"
      "    } else {\n"
      "        do {\n"
      "            if (isface) {\n"
      "                break;\n"
      "            }\n");
    sub_180002487(v25, 3, 1);
    for ( k = 0; ; ++k )
    {
      v38 = (int)k;
      v9 = sub_180001DF2(v21);
      if ( v38 >= v9 )
        break;
      v39 = sub_1800017EE(v26, "            if (dist <= expandLen");
      v10 = std::ostream::operator<<(v39, k);
      v11 = sub_1800017EE(v10, " + grindResectPos) {\n");
      v40 = sub_1800017EE(v11, "                clr = expandClr");
      v12 = std::ostream::operator<<(v40, k);
      v13 = sub_1800017EE(v12, ";\n");
      v14 = sub_1800017EE(v13, "                break;\n");
      sub_1800017EE(v14, "            }\n");
    }
    v41 = sub_180001ACD(v25, v28);
    v42 = v41;
    sub_180001E33(v24, v41);
    sub_180001B8B(v28);
    sub_180002207(v24, "        } while(false);\n    }\n\n");
    sub_1800026FD(v29, "//YHQ::GRIND::Dec");
    LOBYTE(v15) = 1;
    vtkShaderProgram::Substitute(a2, v29, v18, v15);
    sub_180001B8B(v29);
    sub_1800026FD(v30, "//YHQ::GRIND::Impl");
    LOBYTE(v16) = 1;
    vtkShaderProgram::Substitute(a2, v30, v24, v16);
    sub_180001B8B(v30);
    sub_1800025A9(v25);
    sub_180001B8B(v24);
    sub_1800025A9(v19);
    return sub_180001B8B(v18);
  }
  else
  {
LABEL_17:
    sub_180001B8B(v24);
    sub_1800025A9(v19);
    return sub_180001B8B(v18);
  }
}
// 1800244B1: variable 'v15' is possibly undefined
// 1800244F5: variable 'v16' is possibly undefined
// 1800017EE: using guessed type __int64 __fastcall sub_1800017EE(_QWORD, _QWORD);
// 180001ACD: using guessed type __int64 __fastcall sub_180001ACD(_QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001DF2: using guessed type __int64 __fastcall sub_180001DF2(_QWORD);
// 180001E33: using guessed type __int64 __fastcall sub_180001E33(_QWORD, _QWORD);
// 180002207: using guessed type __int64 sub_180002207(_QWORD, const char *, ...);
// 180002487: using guessed type __int64 __fastcall sub_180002487(_QWORD, _QWORD, _QWORD);
// 1800025A9: using guessed type __int64 __fastcall sub_1800025A9(_QWORD);
// 1800026FD: using guessed type __int64 sub_1800026FD(_QWORD, const char *, ...);
// 1800731D0: using guessed type __int64 __fastcall std::ostream::operator<<(_QWORD, _QWORD);
// 1800745E0: using guessed type __int64 __fastcall vtkShaderProgram::Substitute(_QWORD, _QWORD, _QWORD, _QWORD);

//----- (0000000180024590) ----------------------------------------------------
// Hidden C++ exception states: #wind=4
__int64 __fastcall sub_180024590(__int64 a1, __int64 a2)
{
  __int64 *v2; // rdi
  __int64 i; // rcx
  __int64 v4; // r9
  __int64 v5; // r9
  __int64 v7; // [rsp+0h] [rbp-108h] BYREF
  _BYTE v8[64]; // [rsp+28h] [rbp-E0h] BYREF
  _BYTE v9[48]; // [rsp+68h] [rbp-A0h] BYREF
  _BYTE v10[40]; // [rsp+98h] [rbp-70h] BYREF
  _BYTE v11[48]; // [rsp+C0h] [rbp-48h] BYREF
  __int64 v12; // [rsp+F0h] [rbp-18h]

  v2 = &v7;
  for ( i = 64; i; --i )
  {
    *(_DWORD *)v2 = -858993460;
    v2 = (__int64 *)((char *)v2 + 4);
  }
  v12 = -2;
  sub_1800026FD(
    v8,
    "uniform float ambientIntensity; // the material ambient\n"
    "uniform float diffuseIntensity; // the material diffuse\n"
    "uniform float specularIntensity; // the material specular intensity\n"
    "uniform float specularPowerUniform;\n");
  sub_1800026FD(
    v9,
    "  vec3 specularColor = specularIntensity * cutColor;\n"
    "  float specularPower = specularPowerUniform;\n"
    "  vec3 ambientColor = ambientIntensity * cutColor;\n"
    "  vec3 diffuseColor = diffuseIntensity * cutColor;\n"
    "  float opacity = 1;\n");
  sub_1800026FD(v10, "//VTK::Color::Dec");
  LOBYTE(v4) = 1;
  vtkShaderProgram::Substitute(a2, v10, v8, v4);
  sub_180001B8B(v10);
  sub_1800026FD(v11, "//VTK::Color::Impl");
  LOBYTE(v5) = 1;
  vtkShaderProgram::Substitute(a2, v11, v9, v5);
  sub_180001B8B(v11);
  sub_180001B8B(v9);
  return sub_180001B8B(v8);
}
// 180024628: variable 'v4' is possibly undefined
// 180024669: variable 'v5' is possibly undefined
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 1800026FD: using guessed type __int64 sub_1800026FD(_QWORD, const char *, ...);
// 1800745E0: using guessed type __int64 __fastcall vtkShaderProgram::Substitute(_QWORD, _QWORD, _QWORD, _QWORD);

//----- (00000001800246D0) ----------------------------------------------------
// Hidden C++ exception states: #wind=19
__int64 __fastcall sub_1800246D0(__int64 a1, int a2, int a3, __int64 a4)
{
  __int64 *v4; // rdi
  __int64 i; // rcx
  __int64 v6; // r9
  __int64 v7; // r9
  __int64 v8; // rax
  __int64 v9; // rax
  __int64 v10; // r9
  __int64 v11; // rax
  __int64 v12; // rax
  __int64 v13; // rax
  __int64 v14; // rax
  __int64 v15; // rax
  __int64 v16; // r9
  __int64 v17; // rax
  __int64 v18; // rax
  __int64 v19; // rax
  __int64 v20; // rax
  __int64 v21; // rax
  __int64 v22; // rax
  __int64 v23; // rax
  __int64 v24; // r9
  __int64 v25; // rax
  __int64 v26; // rax
  __int64 v27; // rax
  __int64 v28; // rax
  __int64 v29; // rax
  __int64 v30; // rax
  __int64 v31; // rax
  __int64 v32; // rax
  __int64 v33; // rax
  __int64 v34; // rax
  __int64 v35; // rax
  __int64 v36; // rax
  __int64 v37; // rax
  __int64 v38; // r9
  __int64 v40; // [rsp+0h] [rbp-5A8h] BYREF
  _QWORD v41[31]; // [rsp+30h] [rbp-578h] BYREF
  _BYTE v42[40]; // [rsp+128h] [rbp-480h] BYREF
  _BYTE v43[40]; // [rsp+150h] [rbp-458h] BYREF
  _BYTE v44[40]; // [rsp+178h] [rbp-430h] BYREF
  _BYTE v45[40]; // [rsp+1A0h] [rbp-408h] BYREF
  _BYTE v46[40]; // [rsp+1C8h] [rbp-3E0h] BYREF
  _BYTE v47[40]; // [rsp+1F0h] [rbp-3B8h] BYREF
  _BYTE v48[40]; // [rsp+218h] [rbp-390h] BYREF
  unsigned int j; // [rsp+240h] [rbp-368h]
  _BYTE v50[40]; // [rsp+248h] [rbp-360h] BYREF
  _BYTE v51[40]; // [rsp+270h] [rbp-338h] BYREF
  _BYTE v52[40]; // [rsp+298h] [rbp-310h] BYREF
  unsigned int k; // [rsp+2C0h] [rbp-2E8h]
  _BYTE v54[40]; // [rsp+2C8h] [rbp-2E0h] BYREF
  _BYTE v55[40]; // [rsp+2F0h] [rbp-2B8h] BYREF
  _BYTE v56[40]; // [rsp+318h] [rbp-290h] BYREF
  unsigned int m; // [rsp+340h] [rbp-268h]
  _BYTE v58[40]; // [rsp+348h] [rbp-260h] BYREF
  _BYTE v59[40]; // [rsp+370h] [rbp-238h] BYREF
  _BYTE v60[40]; // [rsp+398h] [rbp-210h] BYREF
  unsigned int n; // [rsp+3C0h] [rbp-1E8h]
  _BYTE v62[40]; // [rsp+3C8h] [rbp-1E0h] BYREF
  _BYTE v63[48]; // [rsp+3F0h] [rbp-1B8h] BYREF
  __int64 v64; // [rsp+420h] [rbp-188h]
  int v65; // [rsp+428h] [rbp-180h]
  __int64 v66; // [rsp+430h] [rbp-178h]
  __int64 v67; // [rsp+438h] [rbp-170h]
  __int64 v68; // [rsp+440h] [rbp-168h]
  char *v69; // [rsp+448h] [rbp-160h]
  __int64 v70; // [rsp+450h] [rbp-158h]
  __int64 v71; // [rsp+458h] [rbp-150h]
  __int64 v72; // [rsp+460h] [rbp-148h]
  __int64 v73; // [rsp+468h] [rbp-140h]
  __int64 v74; // [rsp+470h] [rbp-138h]
  char *v75; // [rsp+478h] [rbp-130h]
  __int64 v76; // [rsp+480h] [rbp-128h]
  __int64 v77; // [rsp+488h] [rbp-120h]
  __int64 v78; // [rsp+490h] [rbp-118h]
  __int64 v79; // [rsp+498h] [rbp-110h]
  __int64 v80; // [rsp+4A0h] [rbp-108h]
  __int64 v81; // [rsp+4A8h] [rbp-100h]
  __int64 v82; // [rsp+4B0h] [rbp-F8h]
  char *v83; // [rsp+4B8h] [rbp-F0h]
  __int64 v84; // [rsp+4C0h] [rbp-E8h]
  __int64 v85; // [rsp+4C8h] [rbp-E0h]
  __int64 v86; // [rsp+4D0h] [rbp-D8h]
  __int64 v87; // [rsp+4D8h] [rbp-D0h]
  __int64 v88; // [rsp+4E0h] [rbp-C8h]
  __int64 v89; // [rsp+4E8h] [rbp-C0h]
  __int64 v90; // [rsp+4F0h] [rbp-B8h]
  __int64 v91; // [rsp+4F8h] [rbp-B0h]
  __int64 v92; // [rsp+500h] [rbp-A8h]
  __int64 v93; // [rsp+508h] [rbp-A0h]
  char *v94; // [rsp+510h] [rbp-98h]
  __int64 v95; // [rsp+518h] [rbp-90h]
  __int64 v96; // [rsp+520h] [rbp-88h]
  __int64 v97; // [rsp+528h] [rbp-80h]
  __int64 v98; // [rsp+530h] [rbp-78h]
  __int64 v99; // [rsp+538h] [rbp-70h]
  __int64 v100; // [rsp+540h] [rbp-68h]
  __int64 v101; // [rsp+548h] [rbp-60h]
  __int64 v102; // [rsp+550h] [rbp-58h]
  __int64 v103; // [rsp+558h] [rbp-50h]
  __int64 v104; // [rsp+560h] [rbp-48h]
  __int64 v105; // [rsp+568h] [rbp-40h]
  __int64 v106; // [rsp+570h] [rbp-38h]
  __int64 v107; // [rsp+578h] [rbp-30h]
  __int64 v108; // [rsp+580h] [rbp-28h]
  __int64 v109; // [rsp+588h] [rbp-20h]

  v4 = &v40;
  for ( i = 360; i; --i )
  {
    *(_DWORD *)v4 = -858993460;
    v4 = (__int64 *)((char *)v4 + 4);
  }
  v64 = -2;
  sub_18000139D(v41, 2, 1);
  v65 = a2;
  if ( a2 )
  {
    switch ( v65 )
    {
      case 1:
        sub_1800026FD(v44, "uniform vec3 lightColor0;\n");
        sub_1800026FD(v45, "//VTK::Light::Dec");
        LOBYTE(v6) = 1;
        vtkShaderProgram::Substitute(a4, v45, v44, v6);
        sub_180001B8B(v45);
        sub_180001B8B(v44);
        sub_1800017EE(
          v41,
          "  float df = max(0.0,normal.z);\n"
          "  float sf = pow(df, specularPower);\n"
          "  vec3 diffuse = df * diffuseColor * lightColor0;\n"
          "  vec3 specular = sf * specularColor * lightColor0;\n"
          "  gl_FragData[0] = vec4(ambientColor + diffuse + specular, opacity);\n");
        v66 = sub_180002270(v41, v46);
        v67 = v66;
        v68 = v66;
        sub_1800026FD(v47, "//VTK::Light::Impl");
        LOBYTE(v7) = 1;
        vtkShaderProgram::Substitute(a4, v47, v68, v7);
        sub_180001B8B(v47);
        sub_180001B8B(v46);
        break;
      case 2:
        v69 = (char *)v41 + *(int *)(v41[0] + 4LL);
        std::ios::clear(v69, 0, 0);
        sub_1800026FD(v48, (const char *)&unk_18004FA55);
        sub_180002590(v41, v48);
        sub_180001B8B(v48);
        for ( j = 0; (int)j < a3; ++j )
        {
          v70 = sub_1800017EE(v41, "uniform vec3 lightColor");
          v8 = std::ostream::operator<<(v70, j);
          v71 = sub_1800017EE(v8, ";\n  uniform vec3 lightDirectionVC");
          v9 = std::ostream::operator<<(v71, j);
          sub_1800017EE(v9, "; // normalized\n");
        }
        v72 = sub_180002270(v41, v50);
        v73 = v72;
        v74 = v72;
        sub_1800026FD(v51, "//VTK::Light::Dec");
        LOBYTE(v10) = 1;
        vtkShaderProgram::Substitute(a4, v51, v74, v10);
        sub_180001B8B(v51);
        sub_180001B8B(v50);
        v75 = (char *)v41 + *(int *)(v41[0] + 4LL);
        std::ios::clear(v75, 0, 0);
        sub_1800026FD(v52, (const char *)&unk_18004FA56);
        sub_180002590(v41, v52);
        sub_180001B8B(v52);
        sub_1800017EE(v41, "  vec3 diffuse = vec3(0,0,0);\n  vec3 specular = vec3(0,0,0);\n  float df;\n  float sf;\n");
        for ( k = 0; (int)k < a3; ++k )
        {
          v76 = sub_1800017EE(v41, "    df = max(0.0, dot(normal, -lightDirectionVC");
          v11 = std::ostream::operator<<(v76, k);
          v77 = sub_1800017EE(v11, "));\n  diffuse += (df * lightColor");
          v12 = std::ostream::operator<<(v77, k);
          v13 = sub_1800017EE(v12, ");\n");
          v78 = sub_1800017EE(v13, "  sf = sign(df)*pow(max(0.0, dot( reflect(lightDirectionVC");
          v14 = std::ostream::operator<<(v78, k);
          v79 = sub_1800017EE(
                  v14,
                  ", normal), normalize(-vertexVC.xyz))), specularPower);\n  specular += (sf * lightColor");
          v15 = std::ostream::operator<<(v79, k);
          sub_1800017EE(v15, ");\n");
        }
        sub_1800017EE(
          v41,
          "  diffuse = diffuse * diffuseColor;\n"
          "  specular = specular * specularColor;\n"
          "  gl_FragData[0] = vec4(ambientColor + diffuse + specular, opacity);");
        v80 = sub_180002270(v41, v54);
        v81 = v80;
        v82 = v80;
        sub_1800026FD(v55, "//VTK::Light::Impl");
        LOBYTE(v16) = 1;
        vtkShaderProgram::Substitute(a4, v55, v82, v16);
        sub_180001B8B(v55);
        sub_180001B8B(v54);
        break;
      case 3:
        v83 = (char *)v41 + *(int *)(v41[0] + 4LL);
        std::ios::clear(v83, 0, 0);
        sub_1800026FD(v56, (const char *)&unk_18004FA57);
        sub_180002590(v41, v56);
        sub_180001B8B(v56);
        for ( m = 0; (int)m < a3; ++m )
        {
          v84 = sub_1800017EE(v41, "uniform vec3 lightColor");
          v17 = std::ostream::operator<<(v84, m);
          v85 = sub_1800017EE(v17, ";\nuniform vec3 lightDirectionVC");
          v18 = std::ostream::operator<<(v85, m);
          v86 = sub_1800017EE(v18, "; // normalized\nuniform vec3 lightPositionVC");
          v19 = std::ostream::operator<<(v86, m);
          v87 = sub_1800017EE(v19, ";\nuniform vec3 lightAttenuation");
          v20 = std::ostream::operator<<(v87, m);
          v88 = sub_1800017EE(v20, ";\nuniform float lightConeAngle");
          v21 = std::ostream::operator<<(v88, m);
          v89 = sub_1800017EE(v21, ";\nuniform float lightExponent");
          v22 = std::ostream::operator<<(v89, m);
          v90 = sub_1800017EE(v22, ";\nuniform int lightPositional");
          v23 = std::ostream::operator<<(v90, m);
          sub_1800017EE(v23, ";\n\n");
        }
        v91 = sub_180002270(v41, v58);
        v92 = v91;
        v93 = v91;
        sub_1800026FD(v59, "//VTK::Light::Dec");
        LOBYTE(v24) = 1;
        vtkShaderProgram::Substitute(a4, v59, v93, v24);
        sub_180001B8B(v59);
        sub_180001B8B(v58);
        v94 = (char *)v41 + *(int *)(v41[0] + 4LL);
        std::ios::clear(v94, 0, 0);
        sub_1800026FD(v60, (const char *)&unk_18004FA5B);
        sub_180002590(v41, v60);
        sub_180001B8B(v60);
        sub_1800017EE(
          v41,
          "  vec3 diffuse = vec3(0,0,0);\n"
          "  vec3 specular = vec3(0,0,0);\n"
          "  vec3 vertLightDirectionVC;\n"
          "  float attenuation;\n"
          "  float df;\n"
          "  float sf;\n");
        for ( n = 0; (int)n < a3; ++n )
        {
          v95 = sub_1800017EE(v41, "    attenuation = 1.0;\n    if (lightPositional");
          v25 = std::ostream::operator<<(v95, n);
          v96 = sub_1800017EE(v25, " == 0) {\n      vertLightDirectionVC = lightDirectionVC");
          v26 = std::ostream::operator<<(v96, n);
          v97 = sub_1800017EE(v26, "; }\n    else {\n      vertLightDirectionVC = vertexVC.xyz - lightPositionVC");
          v27 = std::ostream::operator<<(v97, n);
          v98 = sub_1800017EE(
                  v27,
                  ";\n"
                  "      float distanceVC = length(vertLightDirectionVC);\n"
                  "      vertLightDirectionVC = normalize(vertLightDirectionVC);\n"
                  "      attenuation = 1.0 /\n"
                  "        (lightAttenuation");
          v28 = std::ostream::operator<<(v98, n);
          v99 = sub_1800017EE(v28, ".x\n         + lightAttenuation");
          v29 = std::ostream::operator<<(v99, n);
          v100 = sub_1800017EE(v29, ".y * distanceVC\n         + lightAttenuation");
          v30 = std::ostream::operator<<(v100, n);
          v101 = sub_1800017EE(
                   v30,
                   ".z * distanceVC * distanceVC);\n"
                   "      // per OpenGL standard cone angle is 90 or less for a spot light\n"
                   "      if (lightConeAngle");
          v31 = std::ostream::operator<<(v101, n);
          v102 = sub_1800017EE(v31, " <= 90.0) {\n        float coneDot = dot(vertLightDirectionVC, lightDirectionVC");
          v32 = std::ostream::operator<<(v102, n);
          v103 = sub_1800017EE(
                   v32,
                   ");\n        // if inside the cone\n        if (coneDot >= cos(radians(lightConeAngle");
          v33 = std::ostream::operator<<(v103, n);
          v104 = sub_1800017EE(v33, "))) {\n          attenuation = attenuation * pow(coneDot, lightExponent");
          v34 = std::ostream::operator<<(v104, n);
          v35 = sub_1800017EE(v34, "); }\n        else {\n          attenuation = 0.0; }\n        }\n      }\n");
          v105 = sub_1800017EE(
                   v35,
                   "    df = max(0.0,attenuation*dot(normal, -vertLightDirectionVC));\n    diffuse += (df * lightColor");
          v36 = std::ostream::operator<<(v105, n);
          v106 = sub_1800017EE(
                   v36,
                   ");\n"
                   "    sf = sign(df)*attenuation*pow( max(0.0, dot( reflect(vertLightDirectionVC, normal), normalize(-ve"
                   "rtexVC.xyz))), specularPower);\n"
                   "      specular += (sf * lightColor");
          v37 = std::ostream::operator<<(v106, n);
          sub_1800017EE(v37, ");\n");
        }
        sub_1800017EE(
          v41,
          "  diffuse = diffuse * diffuseColor;\n"
          "  specular = specular * specularColor;\n"
          "  gl_FragData[0] = vec4(ambientColor + diffuse + specular, opacity);");
        v107 = sub_180002270(v41, v62);
        v108 = v107;
        v109 = v107;
        sub_1800026FD(v63, "//VTK::Light::Impl");
        LOBYTE(v38) = 1;
        vtkShaderProgram::Substitute(a4, v63, v109, v38);
        sub_180001B8B(v63);
        sub_180001B8B(v62);
        break;
    }
  }
  else
  {
    sub_1800026FD(v42, "  gl_FragData[0] = vec4(ambientColor + diffuseColor, opacity);\n  //VTK::Light::Impl\n");
    sub_1800026FD(v43, "//VTK::Light::Impl");
    vtkShaderProgram::Substitute(a4, v43, v42, 0);
    sub_180001B8B(v43);
    sub_180001B8B(v42);
  }
  return sub_1800023A6(v41);
}
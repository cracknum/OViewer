void __fastcall sub_18001DC80(vtkOpenGLRenderPass *a1, vtkRenderState *a2)
{
  vtkRenderer *Renderer; // rax
  
  v2 = &v8;
  for ( i = 156; i; --i )
  {
    *(_DWORD *)v2 = -858993460;
    v2 = (__int64 *)((char *)v2 + 4);
  }
  v16 = -2;
  vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v9);
  sub_18000139D(v10, 2, 1);
  sub_1800017EE(v10, "vtkGrindSimulatePass::Render");
  Renderer = vtkRenderState::GetRenderer(a2);
  RenderWindow = vtkRenderer::GetRenderWindow(Renderer);
  v18 = (*(__int64 (__fastcall **)(struct vtkRenderWindow *))(*(_QWORD *)RenderWindow + 584LL))(RenderWindow);
  v19 = sub_180002270(v10, v11);
  v20 = v19;
  v21 = v19;
  started = vtkRenderTimerLog::StartScopedEvent(v18, v15, v19);
  v23 = started;
  vtkRenderTimerLog::ScopedEventLogger::operator=(v9, started);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v15);
  sub_180001B8B(v11);
  sub_1800023A6(v10);
  if ( _GLEW_VERSION_4_3 )
  {
    if ( (unsigned __int8)sub_1800022B6(a1, a2) )
    {
      vtkOpenGLRenderPass::PreRender(a1, a2);
      sub_1800014EC(a1, a2);
      sub_1800022C0(a1);
      sub_180002004(a1);
      sub_180001B63(a1);
      sub_180001D20(a1);
      sub_1800011FE(a1);
      sub_18000106E(a1);
      vtkOpenGLRenderPass::PostRender(a1, a2);
    }
  }
  else
  {
    v12 = a1;
    if ( (unsigned int)vtkObject::GetGlobalWarningDisplay() )
    {
      vtkOStreamWrapper::UseEndl((const struct vtkOStreamWrapper::EndlType *)v13);
      vtkOStrStreamWrapper::vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v14);
      v24 = vtkOStreamWrapper::operator<<(
              v14,
              "ERROR: In d:\\develop\\yhq\\gcode\\ntcommonlib\\src\\grindsimulater\\src\\vtkgrindsimulatepass.cpp, line ");
      v25 = vtkOStreamWrapper::operator<<(v24, 79);
      vtkOStreamWrapper::operator<<(v25, "\n");
      if ( v12 )
      {
        ClassNameA = vtkObjectBase::GetClassNameA(v12);
        v26 = vtkOStreamWrapper::operator<<(v14, ClassNameA);
        v27 = vtkOStreamWrapper::operator<<(v26, " (");
        v28 = vtkOStreamWrapper::operator<<(v27, v12);
        vtkOStreamWrapper::operator<<(v28, "): ");
      }
      v29 = vtkOStreamWrapper::operator<<(v14, &unk_180052218);
      vtkOStreamWrapper::operator<<(v29, "\n\n");
      if ( v12 && vtkObject::HasObserver(v12, "ErrorEvent") )
      {
        v6 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v14);
        vtkObject::InvokeEvent(v12, "ErrorEvent", v6);
      }
      else
      {
        v7 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v14);
        vtkOutputWindowDisplayErrorText(v7);
      }
      v30 = vtkOStrStreamWrapper::rdbuf((vtkOStrStreamWrapper *)v14);
      vtkOStrStreamWrapper::freeze(v30, 0);
      vtkObject::BreakOnError();
      vtkOStrStreamWrapper::~vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v14);
    }
  }
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v9);
}

//----- (000000018001E2D0) ----------------------------------------------------
// Hidden C++ exception states: #wind=10
bool __fastcall sub_18001E2D0(__int64 a1, __int64 a2, __int64 a3, __int64 a4)
{
  __int64 *v4; // rdi
  __int64 i; // rcx
  __int64 v7; // r9
  __int64 v8; // r9
  __int64 v9; // r9
  __int64 v10; // r9
  __int64 v11; // r9
  __int64 v12; // [rsp+0h] [rbp-1D8h] BYREF
  _BYTE v13[40]; // [rsp+20h] [rbp-1B8h] BYREF
  _BYTE v14[40]; // [rsp+48h] [rbp-190h] BYREF
  _BYTE v15[40]; // [rsp+70h] [rbp-168h] BYREF
  _BYTE v16[40]; // [rsp+98h] [rbp-140h] BYREF
  _BYTE v17[40]; // [rsp+C0h] [rbp-118h] BYREF
  _BYTE v18[40]; // [rsp+E8h] [rbp-F0h] BYREF
  _BYTE v19[40]; // [rsp+110h] [rbp-C8h] BYREF
  _BYTE v20[40]; // [rsp+138h] [rbp-A0h] BYREF
  _BYTE v21[40]; // [rsp+160h] [rbp-78h] BYREF
  _BYTE v22[48]; // [rsp+188h] [rbp-50h] BYREF
  __int64 v23; // [rsp+1B8h] [rbp-20h]
  int v24; // [rsp+1C0h] [rbp-18h]

  v4 = &v12;
  for ( i = 116; i; --i )
  {
    *(_DWORD *)v4 = -858993460;
    v4 = (__int64 *)((char *)v4 + 4);
  }
  v23 = -2;
  v24 = *(_DWORD *)(a1 + 120);
  if ( v24 == 1 )
    return 1;
  if ( v24 != 2 )
    return v24 == 3;
  sub_1800026FD(v13, "//YHQ AB vs\n//VTK::Camera::Dec\nuniform mat4 MCWCMatrix;\nvarying vec4 worldPos;\n");
  sub_1800026FD(v14, "//VTK::Camera::Dec");
  LOBYTE(v7) = 1;
  vtkShaderProgram::Substitute(a2, v14, v13, v7);
  sub_180001B8B(v14);
  sub_180001B8B(v13);
  sub_1800026FD(v15, "worldPos = MCWCMatrix * vertexMC;\n   //VTK::PositionVC::Impl\n");
  sub_1800026FD(v16, "//VTK::PositionVC::Impl");
  LOBYTE(v8) = 1;
  vtkShaderProgram::Substitute(a2, v16, v15, v8);
  sub_180001B8B(v16);
  sub_180001B8B(v15);
  sub_1800026FD(v17, "#version 430 core\n//YHQ AB fs\n#define attribute in\n#define varying out\n");
  sub_1800026FD(v18, "//VTK::System::Dec");
  LOBYTE(v9) = 1;
  vtkShaderProgram::Substitute(a4, v18, v17, v9);
  sub_180001B8B(v18);
  sub_180001B8B(v17);
  sub_1800026FD(v19, aInVec4Vertexvc);
  sub_1800026FD(v20, "//VTK::PositionVC::Dec");
  LOBYTE(v10) = 1;
  vtkShaderProgram::Substitute(a4, v20, v19, v10);
  sub_180001B8B(v20);
  sub_180001B8B(v19);
  sub_1800026FD(v21, aIvec2CoordIvec);
  sub_1800026FD(v22, "//VTK::TCoord::Impl");
  LOBYTE(v11) = 1;
  vtkShaderProgram::Substitute(a4, v22, v21, v11);
  sub_180001B8B(v22);
  sub_180001B8B(v21);
  return 1;
}
// 18001E39A: variable 'v7' is possibly undefined
// 18001E3F5: variable 'v8' is possibly undefined
// 18001E459: variable 'v9' is possibly undefined
// 18001E4C0: variable 'v10' is possibly undefined
// 18001E527: variable 'v11' is possibly undefined
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 1800026FD: using guessed type __int64 sub_1800026FD(_QWORD, const char *, ...);
// 1800745E0: using guessed type __int64 __fastcall vtkShaderProgram::Substitute(_QWORD, _QWORD, _QWORD, _QWORD);

//----- (000000018001E580) ----------------------------------------------------
bool __fastcall sub_18001E580(__int64 a1)
{
  int *v1; // rdi
  __int64 i; // rcx
  int v4; // [rsp+0h] [rbp-18h] BYREF

  v1 = &v4;
  for ( i = 4; i; --i )
    *v1++ = -858993460;
  v4 = *(_DWORD *)(a1 + 120);
  return v4 == 1 || v4 == 2;
}

//----- (000000018001E5E0) ----------------------------------------------------
// Hidden C++ exception states: #wind=1
bool __fastcall sub_18001E5E0(_DWORD *a1, vtkShaderProgram *a2, __int64 a3, __int64 a4)
{
  __int64 *v4; // rdi
  __int64 i; // rcx
  vtkMatrix4x4 *v7; // rax
  struct vtkMatrix4x4 *v8; // rax
  __int64 v9; // [rsp+0h] [rbp-98h] BYREF
  _BYTE v10[16]; // [rsp+38h] [rbp-60h] BYREF
  __int64 v11; // [rsp+48h] [rbp-50h]
  int v12[3]; // [rsp+58h] [rbp-40h] BYREF
  char v13; // [rsp+64h] [rbp-34h]
  char v14; // [rsp+65h] [rbp-33h]
  __int64 v15; // [rsp+68h] [rbp-30h]
  int v16; // [rsp+70h] [rbp-28h]
  void (__fastcall *v17)(__int64, __int64); // [rsp+78h] [rbp-20h]
  __int64 v18; // [rsp+80h] [rbp-18h]

  v4 = &v9;
  for ( i = 36; i; --i )
  {
    *(_DWORD *)v4 = -858993460;
    v4 = (__int64 *)((char *)v4 + 4);
  }
  v15 = -2;
  v16 = a1[30];
  if ( v16 == 1 )
    return 1;
  if ( v16 != 2 )
    return v16 == 3;
  sub_180001A8C(v10);
  v11 = _RTDynamicCast(a4, 0, &vtkProp `RTTI Type Descriptor', &vtkProp3D `RTTI Type Descriptor', 0);
  if ( v11 )
  {
    v17 = *(void (__fastcall **)(__int64, __int64))(*(_QWORD *)v11 + 672LL);
    v18 = sub_180001852(v10);
    v17(v11, v18);
    v7 = (vtkMatrix4x4 *)sub_180002766(v10);
    vtkMatrix4x4::Transpose(v7);
    v8 = (struct vtkMatrix4x4 *)sub_180001852(v10);
    vtkShaderProgram::SetUniformMatrix(a2, "MCWCMatrix", v8);
    v12[0] = a1[24];
    v12[1] = a1[25];
    vtkShaderProgram::SetUniform2i(a2, "windowSize", v12);
    vtkShaderProgram::SetUniformi(a2, "maxLayers", 16);
    v14 = 1;
    sub_180001F32(v10);
    return v14;
  }
  else
  {
    v13 = 0;
    sub_180001F32(v10);
    return v13;
  }
}
// 180001852: using guessed type __int64 __fastcall sub_180001852(_QWORD);
// 180001A8C: using guessed type __int64 __fastcall sub_180001A8C(_QWORD);
// 180001F32: using guessed type __int64 __fastcall sub_180001F32(_QWORD);
// 180002766: using guessed type __int64 __fastcall sub_180002766(_QWORD);
// 180040B7E: using guessed type __int64 __fastcall _RTDynamicCast(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD);
// 180073AC8: using guessed type void vtkMatrix4x4::Transpose(vtkMatrix4x4 *__hidden this);
// 1800745E8: using guessed type bool vtkShaderProgram::SetUniformMatrix(vtkShaderProgram *__hidden this, const char *, struct vtkMatrix4x4 *);
// 180074610: using guessed type bool vtkShaderProgram::SetUniform2i(vtkShaderProgram *__hidden this, const char *, const int *const);
// 180074620: using guessed type bool vtkShaderProgram::SetUniformi(vtkShaderProgram *__hidden this, const char *, _DWORD);

//----- (000000018001E7B0) ----------------------------------------------------
unsigned __int64 __fastcall sub_18001E7B0(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  __int64 v4; // [rsp+0h] [rbp-28h] BYREF

  v1 = &v4;
  for ( i = 8; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  return vtkTimeStamp::GetMTime((vtkTimeStamp *)(a1 + 128));
}
// 180073818: using guessed type unsigned __int64 vtkTimeStamp::GetMTime(vtkTimeStamp *__hidden this);

//----- (000000018001E7F0) ----------------------------------------------------
__int64 __fastcall sub_18001E7F0(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  __int64 v4; // [rsp+0h] [rbp-38h] BYREF
  vtkInformation *v5; // [rsp+20h] [rbp-18h]
  struct vtkInformationStringKey *v6; // [rsp+28h] [rbp-10h]

  v1 = &v4;
  for ( i = 12; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  if ( !*(_QWORD *)(a1 + 72) )
  {
    *(_QWORD *)(a1 + 72) = vtkInformation::New(a1);
    v5 = *(vtkInformation **)(a1 + 72);
    v6 = vtkGrindInteractor::GrindRequest();
    vtkInformation::Set(v5, v6, (const char *)&unk_18004FA77);
  }
  return *(_QWORD *)(a1 + 72);
}
// 1800737B8: using guessed type void vtkInformation::Set(vtkInformation *__hidden this, struct vtkInformationStringKey *, const char *);
// 1800737C0: using guessed type __int64 __fastcall vtkInformation::New(_QWORD);

//----- (000000018001E880) ----------------------------------------------------
// Hidden C++ exception states: #wind=1
__int64 __fastcall sub_18001E880(__int64 a1)
{
  _DWORD *v1; // rdi
  __int64 i; // rcx
  _QWORD v4[7]; // [rsp+0h] [rbp-38h] BYREF

  v1 = v4;
  for ( i = 12; i; --i )
    *v1++ = -858993460;
  v4[4] = -2;
  vtkOpenGLRenderPass::vtkOpenGLRenderPass((vtkOpenGLRenderPass *)a1);
  *(_QWORD *)a1 = &vtkGrindSimulatePass::`vftable';
  *(_QWORD *)(a1 + 72) = 0;
  *(_QWORD *)(a1 + 80) = 0;
  *(_QWORD *)(a1 + 88) = 0;
  *(_DWORD *)(a1 + 96) = -1;
  *(_DWORD *)(a1 + 100) = -1;
  *(_QWORD *)(a1 + 112) = 0;
  *(_DWORD *)(a1 + 120) = -1;
  vtkTimeStamp::vtkTimeStamp((vtkTimeStamp *)(a1 + 128));
  *(_DWORD *)(a1 + 136) = 0;
  *(_QWORD *)(a1 + 144) = 0;
  *(_QWORD *)(a1 + 152) = 0;
  *(_DWORD *)(a1 + 160) = 0;
  *(_DWORD *)(a1 + 164) = 0;
  *(_DWORD *)(a1 + 168) = 0;
  *(_QWORD *)(a1 + 176) = 0;
  *(_QWORD *)(a1 + 184) = 0;
  *(_QWORD *)(a1 + 192) = 0;
  *(_QWORD *)(a1 + 200) = 0;
  *(_QWORD *)(a1 + 208) = 0;
  *(_QWORD *)(a1 + 216) = 0;
  *(_QWORD *)(a1 + 224) = 0;
  *(_QWORD *)(a1 + 232) = 0;
  *(_QWORD *)(a1 + 240) = 0;
  *(_QWORD *)(a1 + 248) = 0;
  *(_DWORD *)(a1 + 256) = 0;
  *(_DWORD *)(a1 + 260) = 0;
  *(_QWORD *)(a1 + 64) = vtkOpaquePass::New();
  return a1;
}
// 18005BC90: using guessed type void *vtkGrindSimulatePass::`vftable';
// 180073828: using guessed type vtkTimeStamp *vtkTimeStamp::vtkTimeStamp(vtkTimeStamp *__hidden this);
// 1800744C8: using guessed type vtkOpenGLRenderPass *vtkOpenGLRenderPass::vtkOpenGLRenderPass(vtkOpenGLRenderPass *__hidden this);
// 1800745A0: using guessed type __int64 vtkOpaquePass::New(void);

//----- (000000018001EA70) ----------------------------------------------------
void __fastcall sub_18001EA70(__int64 a1)
{
  _DWORD *v1; // rdi
  __int64 i; // rcx
  _QWORD v3[7]; // [rsp+0h] [rbp-58h] BYREF
  __int64 v4; // [rsp+38h] [rbp-20h]
  void (__fastcall *v5)(__int64, __int64); // [rsp+40h] [rbp-18h]

  v1 = v3;
  for ( i = 20; i; --i )
    *v1++ = -858993460;
  v3[4] = -2;
  *(_QWORD *)a1 = &vtkGrindSimulatePass::`vftable';
  sub_1800026E9(a1);
  if ( *(_DWORD *)(a1 + 256) )
  {
    v3[5] = _glewDeleteBuffers;
    _glewDeleteBuffers(1, a1 + 256);
    *(_DWORD *)(a1 + 256) = 0;
  }
  if ( *(_DWORD *)(a1 + 260) )
  {
    v3[6] = _glewDeleteBuffers;
    _glewDeleteBuffers(1, a1 + 260);
    *(_DWORD *)(a1 + 260) = 0;
  }
  sub_180001046(a1);
  if ( *(_QWORD *)(a1 + 64) )
    (*(void (__fastcall **)(_QWORD))(**(_QWORD **)(a1 + 64) + 16LL))(*(_QWORD *)(a1 + 64));
  if ( *(_QWORD *)(a1 + 152) )
  {
    v4 = *(_QWORD *)(a1 + 152);
    v5 = *(void (__fastcall **)(__int64, __int64))(**(_QWORD **)(a1 + 152) + 64LL);
    v5(v4, a1);
    *(_QWORD *)(a1 + 152) = 0;
  }
  vtkOpenGLRenderPass::~vtkOpenGLRenderPass((vtkOpenGLRenderPass *)a1);
}
// 180001046: using guessed type __int64 __fastcall sub_180001046(_QWORD);
// 1800026E9: using guessed type __int64 __fastcall sub_1800026E9(_QWORD);
// 18005BC90: using guessed type void *vtkGrindSimulatePass::`vftable';
// 1800744D0: using guessed type void vtkOpenGLRenderPass::~vtkOpenGLRenderPass(vtkOpenGLRenderPass *__hidden this);
// 180074778: using guessed type __int64 (__fastcall *_glewDeleteBuffers)(_QWORD, _QWORD);

//----- (000000018001EBE0) ----------------------------------------------------
bool __fastcall sub_18001EBE0(__int64 a1, vtkRenderState *a2)
{
  _DWORD *v2; // rdi
  __int64 i; // rcx
  struct vtkProp **PropArray; // rax
  _QWORD v6[7]; // [rsp+0h] [rbp-78h] BYREF
  struct vtkProp *v7; // [rsp+38h] [rbp-40h]
  int v8; // [rsp+40h] [rbp-38h]
  int PropArrayCount; // [rsp+44h] [rbp-34h]
  int j; // [rsp+48h] [rbp-30h]
  struct vtkProp *v11; // [rsp+50h] [rbp-28h]
  unsigned __int8 (__fastcall *v12)(struct vtkProp *, struct vtkInformation *); // [rsp+58h] [rbp-20h]
  struct vtkInformation *RequiredKeys; // [rsp+60h] [rbp-18h]

  v2 = v6;
  for ( i = 28; i; --i )
    *v2++ = -858993460;
  v6[6] = 0;
  v7 = 0;
  v8 = 0;
  PropArrayCount = vtkRenderState::GetPropArrayCount(a2);
  for ( j = 0; j < PropArrayCount; ++j )
  {
    PropArray = vtkRenderState::GetPropArray(a2);
    v11 = PropArray[j];
    v12 = *(unsigned __int8 (__fastcall **)(struct vtkProp *, struct vtkInformation *))(*(_QWORD *)v11 + 384LL);
    RequiredKeys = vtkRenderState::GetRequiredKeys(a2);
    if ( v12(v11, RequiredKeys) )
    {
      v7 = v11;
      ++v8;
    }
  }
  if ( v8 != 1 )
    return 0;
  *(_QWORD *)(a1 + 144) = _RTDynamicCast(v7, 0, &vtkProp `RTTI Type Descriptor', &vtkActor `RTTI Type Descriptor', 0);
  return *(_QWORD *)(a1 + 144) != 0;
}
// 180040B7E: using guessed type __int64 __fastcall _RTDynamicCast(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD);
// 18006B2A0: using guessed type void **vtkActor `RTTI Type Descriptor';
// 180074280: using guessed type struct vtkInformation *vtkRenderState::GetRequiredKeys(vtkRenderState *__hidden this);
// 180074288: using guessed type _DWORD vtkRenderState::GetPropArrayCount(vtkRenderState *__hidden this);
// 180074290: using guessed type struct vtkProp **vtkRenderState::GetPropArray(vtkRenderState *__hidden this);

//----- (000000018001ED30) ----------------------------------------------------
// Hidden C++ exception states: #wind=4
void __fastcall sub_18001ED30(__int64 a1, vtkRenderState *a2)
{
  __int64 *v2; // rdi
  __int64 i; // rcx
  __int64 v4; // [rsp+0h] [rbp-268h] BYREF
  vtkRenderer *Renderer; // [rsp+30h] [rbp-238h]
  int v6; // [rsp+48h] [rbp-220h] BYREF
  _DWORD v7[6]; // [rsp+4Ch] [rbp-21Ch] BYREF
  _BYTE v8[32]; // [rsp+64h] [rbp-204h] BYREF
  _BYTE v9[36]; // [rsp+84h] [rbp-1E4h] BYREF
  _BYTE v10[40]; // [rsp+A8h] [rbp-1C0h] BYREF
  _BYTE v11[248]; // [rsp+D0h] [rbp-198h] BYREF
  _BYTE v12[48]; // [rsp+1C8h] [rbp-A0h] BYREF
  _BYTE v13[8]; // [rsp+1F8h] [rbp-70h] BYREF
  __int64 v14; // [rsp+200h] [rbp-68h]
  void (__fastcall *v15)(vtkRenderer *, int *, _DWORD *, _BYTE *, _BYTE *); // [rsp+208h] [rbp-60h]
  struct vtkRenderWindow *RenderWindow; // [rsp+210h] [rbp-58h]
  __int64 v17; // [rsp+218h] [rbp-50h]
  __int64 v18; // [rsp+220h] [rbp-48h]
  __int64 v19; // [rsp+228h] [rbp-40h]
  __int64 v20; // [rsp+230h] [rbp-38h]
  __int64 started; // [rsp+238h] [rbp-30h]
  __int64 v22; // [rsp+240h] [rbp-28h]
  __int64 v23; // [rsp+248h] [rbp-20h]
  __int64 v24; // [rsp+250h] [rbp-18h]

  v2 = &v4;
  for ( i = 152; i; --i )
  {
    *(_DWORD *)v2 = -858993460;
    v2 = (__int64 *)((char *)v2 + 4);
  }
  v14 = -2;
  *(_QWORD *)(a1 + 112) = a2;
  Renderer = vtkRenderState::GetRenderer(a2);
  if ( vtkRenderState::GetFrameBuffer(a2) )
  {
    vtkRenderState::GetWindowSize(a2, &v6);
  }
  else
  {
    v15 = *(void (__fastcall **)(vtkRenderer *, int *, _DWORD *, _BYTE *, _BYTE *))(*(_QWORD *)Renderer + 640LL);
    v15(Renderer, &v6, v7, v8, v9);
  }
  RenderWindow = vtkRenderer::GetRenderWindow(Renderer);
  *(_QWORD *)(a1 + 104) = (*(__int64 (__fastcall **)(struct vtkRenderWindow *))(*(_QWORD *)RenderWindow + 584LL))(RenderWindow);
  vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v10);
  sub_18000139D(v11, 2, 1);
  sub_1800017EE(v11, "vtkGrindSimulatePass::Initialize");
  v17 = *(_QWORD *)(a1 + 104);
  v18 = sub_180002270(v11, v12);
  v19 = v18;
  v20 = v18;
  started = vtkRenderTimerLog::StartScopedEvent(v17, v13, v18);
  v22 = started;
  vtkRenderTimerLog::ScopedEventLogger::operator=(v10, started);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v13);
  sub_180001B8B(v12);
  sub_1800023A6(v11);
  if ( *(_DWORD *)(a1 + 160) && (v7[0] != *(_DWORD *)(a1 + 100) || v6 != *(_DWORD *)(a1 + 96)) )
    sub_1800026E9(a1);
  *(_DWORD *)(a1 + 96) = v6;
  *(_DWORD *)(a1 + 100) = v7[0];
  if ( !*(_QWORD *)(a1 + 152) )
    *(_QWORD *)(a1 + 152) = vtkOpenGLFramebufferObject::New();
  if ( !*(_DWORD *)(a1 + 160) )
  {
    sub_180001A7D(a1, a2);
    sub_1800010FA(a1);
    sub_18000191A(a1);
    sub_180001839(a1, a2);
  }
  if ( (unsigned __int8)sub_180001550(*(_QWORD *)(a1 + 88)) )
  {
    v23 = *(_QWORD *)(a1 + 88);
    sub_1800026D5(v23, 0);
    sub_180001F5F(*(_QWORD *)(a1 + 88));
  }
  else if ( (unsigned __int8)sub_180001DE3(*(_QWORD *)(a1 + 88)) )
  {
    v24 = *(_QWORD *)(a1 + 88);
    sub_18000249B(v24, 0);
    sub_180002608(a1);
  }
  sub_180001866(a1, a2);
  sub_180001A82(a1, a2);
  sub_180002310(a1);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v10);
}
// 1800010FA: using guessed type __int64 __fastcall sub_1800010FA(_QWORD);
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 180001550: using guessed type __int64 __fastcall sub_180001550(_QWORD);
// 1800017EE: using guessed type __int64 __fastcall sub_1800017EE(_QWORD, _QWORD);
// 180001839: using guessed type __int64 __fastcall sub_180001839(_QWORD, _QWORD);
// 180001866: using guessed type __int64 __fastcall sub_180001866(_QWORD, _QWORD);
// 18000191A: using guessed type __int64 __fastcall sub_18000191A(_QWORD);
// 180001A7D: using guessed type __int64 __fastcall sub_180001A7D(_QWORD, _QWORD);
// 180001A82: using guessed type __int64 __fastcall sub_180001A82(_QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001DE3: using guessed type __int64 __fastcall sub_180001DE3(_QWORD);
// 180001F5F: using guessed type __int64 __fastcall sub_180001F5F(_QWORD);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 180002310: using guessed type __int64 __fastcall sub_180002310(_QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 18000249B: using guessed type __int64 __fastcall sub_18000249B(_QWORD, _QWORD);
// 180002608: using guessed type __int64 __fastcall sub_180002608(_QWORD);
// 1800026D5: using guessed type __int64 __fastcall sub_1800026D5(_QWORD, _QWORD);
// 1800026E9: using guessed type __int64 __fastcall sub_1800026E9(_QWORD);
// 180074260: using guessed type __int64 __fastcall vtkRenderTimerLog::StartScopedEvent(_QWORD, _QWORD, _QWORD);
// 180074268: using guessed type void vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 180074270: using guessed type __int64 __fastcall vtkRenderTimerLog::ScopedEventLogger::operator=(_QWORD, _QWORD);
// 180074278: using guessed type _QWORD vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 180074298: using guessed type void vtkRenderState::GetWindowSize(vtkRenderState *__hidden this, int *const);
// 1800742A0: using guessed type struct vtkFrameBufferObjectBase *vtkRenderState::GetFrameBuffer(vtkRenderState *__hidden this);
// 1800742A8: using guessed type struct vtkRenderer *vtkRenderState::GetRenderer(vtkRenderState *__hidden this);
// 1800742E0: using guessed type struct vtkRenderWindow *vtkRenderer::GetRenderWindow(vtkRenderer *__hidden this);
// 1800744E8: using guessed type __int64 vtkOpenGLFramebufferObject::New(void);

//----- (000000018001F170) ----------------------------------------------------
void __fastcall sub_18001F170(__int64 a1, vtkRenderState *a2)
{
  __int64 *v2; // rdi
  __int64 i; // rcx
  vtkRenderer *Renderer; // rax
  __int64 v5; // [rsp+0h] [rbp-38h] BYREF
  vtkOpenGLFramebufferObject *v6; // [rsp+20h] [rbp-18h]
  struct vtkRenderWindow *RenderWindow; // [rsp+28h] [rbp-10h]

  v2 = &v5;
  for ( i = 12; i; --i )
  {
    *(_DWORD *)v2 = -858993460;
    v2 = (__int64 *)((char *)v2 + 4);
  }
  v6 = *(vtkOpenGLFramebufferObject **)(a1 + 152);
  Renderer = vtkRenderState::GetRenderer(a2);
  RenderWindow = vtkRenderer::GetRenderWindow(Renderer);
  vtkOpenGLFramebufferObject::SetContext(v6, RenderWindow);
}
// 1800742A8: using guessed type struct vtkRenderer *vtkRenderState::GetRenderer(vtkRenderState *__hidden this);
// 1800742E0: using guessed type struct vtkRenderWindow *vtkRenderer::GetRenderWindow(vtkRenderer *__hidden this);
// 1800744F0: using guessed type void vtkOpenGLFramebufferObject::SetContext(vtkOpenGLFramebufferObject *__hidden this, struct vtkRenderWindow *);

//----- (000000018001F1E0) ----------------------------------------------------
// Hidden C++ exception states: #wind=3
__int64 __fastcall sub_18001F1E0(unsigned int *a1)
{
  _DWORD *v1; // rdi
  __int64 i; // rcx
  __int64 result; // rax
  const char *ClassNameA; // rax
  char *v5; // rax
  const char *v6; // rax
  _DWORD v7[20]; // [rsp+0h] [rbp-368h] BYREF
  _BYTE v8[96]; // [rsp+50h] [rbp-318h] BYREF
  _BYTE v9[132]; // [rsp+B0h] [rbp-2B8h] BYREF
  int v10; // [rsp+134h] [rbp-234h]
  _BYTE v11[248]; // [rsp+150h] [rbp-218h] BYREF
  vtkObjectBase *v12; // [rsp+248h] [rbp-120h]
  _BYTE v13[36]; // [rsp+254h] [rbp-114h] BYREF
  _BYTE v14[48]; // [rsp+278h] [rbp-F0h] BYREF
  _BYTE v15[48]; // [rsp+2A8h] [rbp-C0h] BYREF
  __int64 v16; // [rsp+2D8h] [rbp-90h]
  __int64 (__fastcall *v17)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD); // [rsp+2E0h] [rbp-88h]
  __int64 v18; // [rsp+2E8h] [rbp-80h]
  __int64 v19; // [rsp+2F0h] [rbp-78h]
  __int64 v20; // [rsp+2F8h] [rbp-70h]
  __int64 v21; // [rsp+300h] [rbp-68h]
  __int64 v22; // [rsp+308h] [rbp-60h]
  __int64 v23; // [rsp+310h] [rbp-58h]
  __int64 v24; // [rsp+318h] [rbp-50h]
  __int64 v25; // [rsp+320h] [rbp-48h]
  __int64 v26; // [rsp+328h] [rbp-40h]
  __int64 v27; // [rsp+330h] [rbp-38h]
  __int64 v28; // [rsp+338h] [rbp-30h]
  __int64 v29; // [rsp+340h] [rbp-28h]
  vtkOStrStreamWrapper *v30; // [rsp+348h] [rbp-20h]

  v1 = v7;
  for ( i = 216; i; --i )
    *v1++ = -858993460;
  v16 = -2;
  glGenTextures(1, a1 + 40);
  glBindTexture(0xDE1u, a1[40]);
  v17 = _glewTexStorage2D;
  _glewTexStorage2D(3553, 1, 33334, a1[24], a1[25]);
  glBindTexture(0xDE1u, 0);
  v7[12] = 16;
  memset(v8, 0, 0x40u);
  memset(v9, 0, 0x80u);
  result = sub_18000134D(16, v8, v9);
  v10 = result;
  if ( (_DWORD)result )
  {
    sub_18000139D(v11, 2, 1);
    sub_180001CDA((unsigned int)v11, 16, v10, (unsigned int)v8, (__int64)v9);
    v12 = (vtkObjectBase *)a1;
    if ( (unsigned int)vtkObject::GetGlobalWarningDisplay() )
    {
      vtkOStreamWrapper::UseEndl((const struct vtkOStreamWrapper::EndlType *)v13);
      vtkOStrStreamWrapper::vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v14);
      v18 = vtkOStreamWrapper::operator<<(
              v14,
              "ERROR: In d:\\develop\\yhq\\gcode\\ntcommonlib\\src\\grindsimulater\\src\\vtkgrindsimulatepass.cpp, line ");
      v19 = vtkOStreamWrapper::operator<<(v18, 361);
      vtkOStreamWrapper::operator<<(v19, "\n");
      if ( v12 )
      {
        ClassNameA = vtkObjectBase::GetClassNameA(v12);
        v20 = vtkOStreamWrapper::operator<<(v14, ClassNameA);
        v21 = vtkOStreamWrapper::operator<<(v20, " (");
        v22 = vtkOStreamWrapper::operator<<(v21, v12);
        vtkOStreamWrapper::operator<<(v22, "): ");
      }
      v23 = vtkOStreamWrapper::operator<<(v14, &unk_18004FB17);
      v24 = vtkOStreamWrapper::operator<<(v23, "fail to init HeadPointerTexture");
      v25 = vtkOStreamWrapper::operator<<(v24, " ");
      v26 = sub_180002270(v11, v15);
      v27 = v26;
      v28 = sub_18000236F(v26);
      v29 = vtkOStreamWrapper::operator<<(v25, v28);
      vtkOStreamWrapper::operator<<(v29, "\n\n");
      sub_180001B8B(v15);
      if ( v12 && vtkObject::HasObserver(v12, "ErrorEvent") )
      {
        v5 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v14);
        vtkObject::InvokeEvent(v12, "ErrorEvent", v5);
      }
      else
      {
        v6 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v14);
        vtkOutputWindowDisplayErrorText(v6);
      }
      v30 = vtkOStrStreamWrapper::rdbuf((vtkOStrStreamWrapper *)v14);
      vtkOStrStreamWrapper::freeze(v30, 0);
      vtkObject::BreakOnError();
      vtkOStrStreamWrapper::~vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v14);
    }
    return sub_1800023A6(v11);
  }
  return result;
}
// 18000134D: using guessed type __int64 __fastcall sub_18000134D(_QWORD, _QWORD, _QWORD);
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001CDA: using guessed type __int64 __fastcall sub_180001CDA(_DWORD, _DWORD, _DWORD, _DWORD, __int64);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 18000236F: using guessed type __int64 __fastcall sub_18000236F(_QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 1800737C8: using guessed type _DWORD vtkObject::InvokeEvent(vtkObject *__hidden this, const char *, void *);
// 1800737D0: using guessed type _DWORD vtkObject::HasObserver(vtkObject *__hidden this, const char *);
// 1800737D8: using guessed type __int64 vtkObject::GetGlobalWarningDisplay(void);
// 1800737E0: using guessed type __int64 vtkObject::BreakOnError(void);
// 180073800: using guessed type void vtkOutputWindowDisplayErrorText(const char *);
// 180073810: using guessed type const char *vtkObjectBase::GetClassNameA(vtkObjectBase *__hidden this);
// 180073830: using guessed type void vtkOStrStreamWrapper::freeze(vtkOStrStreamWrapper *__hidden this, _DWORD);
// 180073838: using guessed type struct vtkOStrStreamWrapper *vtkOStrStreamWrapper::rdbuf(vtkOStrStreamWrapper *__hidden this);
// 180073840: using guessed type char *vtkOStrStreamWrapper::str(vtkOStrStreamWrapper *__hidden this);
// 180073848: using guessed type void vtkOStrStreamWrapper::~vtkOStrStreamWrapper(vtkOStrStreamWrapper *__hidden this);
// 180073850: using guessed type vtkOStrStreamWrapper *vtkOStrStreamWrapper::vtkOStrStreamWrapper(vtkOStrStreamWrapper *__hidden this);
// 180073858: using guessed type static void vtkOStreamWrapper::UseEndl(const struct vtkOStreamWrapper::EndlType *);
// 180073860: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180073868: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180073870: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180074798: using guessed type __int64 (__fastcall *_glewTexStorage2D)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD);

//----- (000000018001F610) ----------------------------------------------------
// Hidden C++ exception states: #wind=3
__int64 __fastcall sub_18001F610(vtkObjectBase *a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  __int64 result; // rax
  const char *ClassNameA; // rax
  char *v5; // rax
  const char *v6; // rax
  __int64 v7; // [rsp+0h] [rbp-388h] BYREF
  int v8; // [rsp+30h] [rbp-358h]
  __int64 v9; // [rsp+38h] [rbp-350h]
  __int64 v10; // [rsp+40h] [rbp-348h]
  int v11; // [rsp+48h] [rbp-340h]
  _BYTE v12[96]; // [rsp+60h] [rbp-328h] BYREF
  _BYTE v13[132]; // [rsp+C0h] [rbp-2C8h] BYREF
  int v14; // [rsp+144h] [rbp-244h]
  _BYTE v15[248]; // [rsp+160h] [rbp-228h] BYREF
  vtkObjectBase *v16; // [rsp+258h] [rbp-130h]
  _BYTE v17[36]; // [rsp+264h] [rbp-124h] BYREF
  _BYTE v18[48]; // [rsp+288h] [rbp-100h] BYREF
  _BYTE v19[48]; // [rsp+2B8h] [rbp-D0h] BYREF
  __int64 v20; // [rsp+2E8h] [rbp-A0h]
  __int64 (__fastcall *v21)(_QWORD, _QWORD); // [rsp+2F0h] [rbp-98h]
  __int64 (__fastcall *v22)(_QWORD, _QWORD); // [rsp+2F8h] [rbp-90h]
  __int64 (__fastcall *v23)(_QWORD, _QWORD, _QWORD, _QWORD); // [rsp+300h] [rbp-88h]
  __int64 (__fastcall *v24)(_QWORD, _QWORD); // [rsp+308h] [rbp-80h]
  __int64 v25; // [rsp+310h] [rbp-78h]
  __int64 v26; // [rsp+318h] [rbp-70h]
  __int64 v27; // [rsp+320h] [rbp-68h]
  __int64 v28; // [rsp+328h] [rbp-60h]
  __int64 v29; // [rsp+330h] [rbp-58h]
  __int64 v30; // [rsp+338h] [rbp-50h]
  __int64 v31; // [rsp+340h] [rbp-48h]
  __int64 v32; // [rsp+348h] [rbp-40h]
  __int64 v33; // [rsp+350h] [rbp-38h]
  __int64 v34; // [rsp+358h] [rbp-30h]
  __int64 v35; // [rsp+360h] [rbp-28h]
  __int64 v36; // [rsp+368h] [rbp-20h]
  vtkOStrStreamWrapper *v37; // [rsp+370h] [rbp-18h]

  v1 = &v7;
  for ( i = 224; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  v20 = -2;
  v8 = 16 * *((_DWORD *)a1 + 25) * *((_DWORD *)a1 + 24);
  v9 = 36;
  v10 = 36LL * v8;
  v21 = _glewGenBuffers;
  _glewGenBuffers(1, (char *)a1 + 164);
  v22 = _glewBindBuffer;
  _glewBindBuffer(37074, *((unsigned int *)a1 + 41));
  v23 = _glewBufferData;
  _glewBufferData(37074, v10, 0, 35048);
  v24 = _glewBindBuffer;
  _glewBindBuffer(37074, 0);
  v11 = 16;
  memset(v12, 0, 0x40u);
  memset(v13, 0, 0x80u);
  result = sub_18000134D(16, v12, v13);
  v14 = result;
  if ( (_DWORD)result )
  {
    sub_18000139D(v15, 2, 1);
    sub_180001CDA((unsigned int)v15, 16, v14, (unsigned int)v12, (__int64)v13);
    v16 = a1;
    if ( (unsigned int)vtkObject::GetGlobalWarningDisplay() )
    {
      vtkOStreamWrapper::UseEndl((const struct vtkOStreamWrapper::EndlType *)v17);
      vtkOStrStreamWrapper::vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v18);
      v25 = vtkOStreamWrapper::operator<<(
              v18,
              "ERROR: In d:\\develop\\yhq\\gcode\\ntcommonlib\\src\\grindsimulater\\src\\vtkgrindsimulatepass.cpp, line ");
      v26 = vtkOStreamWrapper::operator<<(v25, 373);
      vtkOStreamWrapper::operator<<(v26, "\n");
      if ( v16 )
      {
        ClassNameA = vtkObjectBase::GetClassNameA(v16);
        v27 = vtkOStreamWrapper::operator<<(v18, ClassNameA);
        v28 = vtkOStreamWrapper::operator<<(v27, " (");
        v29 = vtkOStreamWrapper::operator<<(v28, v16);
        vtkOStreamWrapper::operator<<(v29, "): ");
      }
      v30 = vtkOStreamWrapper::operator<<(v18, &unk_18004FB4D);
      v31 = vtkOStreamWrapper::operator<<(v30, "fail to init ABufferSSBO");
      v32 = vtkOStreamWrapper::operator<<(v31, " ");
      v33 = sub_180002270(v15, v19);
      v34 = v33;
      v35 = sub_18000236F(v33);
      v36 = vtkOStreamWrapper::operator<<(v32, v35);
      vtkOStreamWrapper::operator<<(v36, "\n\n");
      sub_180001B8B(v19);
      if ( v16 && vtkObject::HasObserver(v16, "ErrorEvent") )
      {
        v5 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v18);
        vtkObject::InvokeEvent(v16, "ErrorEvent", v5);
      }
      else
      {
        v6 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v18);
        vtkOutputWindowDisplayErrorText(v6);
      }
      v37 = vtkOStrStreamWrapper::rdbuf((vtkOStrStreamWrapper *)v18);
      vtkOStrStreamWrapper::freeze(v37, 0);
      vtkObject::BreakOnError();
      vtkOStrStreamWrapper::~vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v18);
    }
    return sub_1800023A6(v15);
  }
  return result;
}
// 18000134D: using guessed type __int64 __fastcall sub_18000134D(_QWORD, _QWORD, _QWORD);
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001CDA: using guessed type __int64 __fastcall sub_180001CDA(_DWORD, _DWORD, _DWORD, _DWORD, __int64);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 18000236F: using guessed type __int64 __fastcall sub_18000236F(_QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 1800737C8: using guessed type _DWORD vtkObject::InvokeEvent(vtkObject *__hidden this, const char *, void *);
// 1800737D0: using guessed type _DWORD vtkObject::HasObserver(vtkObject *__hidden this, const char *);
// 1800737D8: using guessed type __int64 vtkObject::GetGlobalWarningDisplay(void);
// 1800737E0: using guessed type __int64 vtkObject::BreakOnError(void);
// 180073800: using guessed type void vtkOutputWindowDisplayErrorText(const char *);
// 180073810: using guessed type const char *vtkObjectBase::GetClassNameA(vtkObjectBase *__hidden this);
// 180073830: using guessed type void vtkOStrStreamWrapper::freeze(vtkOStrStreamWrapper *__hidden this, _DWORD);
// 180073838: using guessed type struct vtkOStrStreamWrapper *vtkOStrStreamWrapper::rdbuf(vtkOStrStreamWrapper *__hidden this);
// 180073840: using guessed type char *vtkOStrStreamWrapper::str(vtkOStrStreamWrapper *__hidden this);
// 180073848: using guessed type void vtkOStrStreamWrapper::~vtkOStrStreamWrapper(vtkOStrStreamWrapper *__hidden this);
// 180073850: using guessed type vtkOStrStreamWrapper *vtkOStrStreamWrapper::vtkOStrStreamWrapper(vtkOStrStreamWrapper *__hidden this);
// 180073858: using guessed type static void vtkOStreamWrapper::UseEndl(const struct vtkOStreamWrapper::EndlType *);
// 180073860: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180073868: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180073870: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180074768: using guessed type __int64 (__fastcall *_glewBufferData)(_QWORD, _QWORD, _QWORD, _QWORD);
// 180074780: using guessed type __int64 (__fastcall *_glewGenBuffers)(_QWORD, _QWORD);
// 180074788: using guessed type __int64 (__fastcall *_glewBindBuffer)(_QWORD, _QWORD);

//----- (000000018001FA90) ----------------------------------------------------
// Hidden C++ exception states: #wind=3
__int64 __fastcall sub_18001FA90(unsigned int *a1)
{
  _DWORD *v1; // rdi
  __int64 i; // rcx
  __int64 result; // rax
  const char *ClassNameA; // rax
  char *v5; // rax
  const char *v6; // rax
  _DWORD v7[20]; // [rsp+0h] [rbp-378h] BYREF
  _BYTE v8[96]; // [rsp+50h] [rbp-328h] BYREF
  _BYTE v9[132]; // [rsp+B0h] [rbp-2C8h] BYREF
  int v10; // [rsp+134h] [rbp-244h]
  _BYTE v11[248]; // [rsp+150h] [rbp-228h] BYREF
  vtkObjectBase *v12; // [rsp+248h] [rbp-130h]
  _BYTE v13[36]; // [rsp+254h] [rbp-124h] BYREF
  _BYTE v14[48]; // [rsp+278h] [rbp-100h] BYREF
  _BYTE v15[48]; // [rsp+2A8h] [rbp-D0h] BYREF
  __int64 v16; // [rsp+2D8h] [rbp-A0h]
  __int64 (__fastcall *v17)(_QWORD, _QWORD); // [rsp+2E0h] [rbp-98h]
  __int64 (__fastcall *v18)(_QWORD, _QWORD); // [rsp+2E8h] [rbp-90h]
  __int64 (__fastcall *v19)(_QWORD, _QWORD, _QWORD, _QWORD); // [rsp+2F0h] [rbp-88h]
  __int64 (__fastcall *v20)(_QWORD, _QWORD); // [rsp+2F8h] [rbp-80h]
  __int64 v21; // [rsp+300h] [rbp-78h]
  __int64 v22; // [rsp+308h] [rbp-70h]
  __int64 v23; // [rsp+310h] [rbp-68h]
  __int64 v24; // [rsp+318h] [rbp-60h]
  __int64 v25; // [rsp+320h] [rbp-58h]
  __int64 v26; // [rsp+328h] [rbp-50h]
  __int64 v27; // [rsp+330h] [rbp-48h]
  __int64 v28; // [rsp+338h] [rbp-40h]
  __int64 v29; // [rsp+340h] [rbp-38h]
  __int64 v30; // [rsp+348h] [rbp-30h]
  __int64 v31; // [rsp+350h] [rbp-28h]
  __int64 v32; // [rsp+358h] [rbp-20h]
  vtkOStrStreamWrapper *v33; // [rsp+360h] [rbp-18h]

  v1 = v7;
  for ( i = 220; i; --i )
    *v1++ = -858993460;
  v16 = -2;
  v17 = _glewGenBuffers;
  _glewGenBuffers(1, a1 + 42);
  v18 = _glewBindBuffer;
  _glewBindBuffer(37568, a1[42]);
  v19 = _glewBufferData;
  _glewBufferData(37568, 4, 0, 35048);
  v20 = _glewBindBuffer;
  _glewBindBuffer(37568, 0);
  v7[12] = 16;
  memset(v8, 0, 0x40u);
  memset(v9, 0, 0x80u);
  result = sub_18000134D(16, v8, v9);
  v10 = result;
  if ( (_DWORD)result )
  {
    sub_18000139D(v11, 2, 1);
    sub_180001CDA((unsigned int)v11, 16, v10, (unsigned int)v8, (__int64)v9);
    v12 = (vtkObjectBase *)a1;
    if ( (unsigned int)vtkObject::GetGlobalWarningDisplay() )
    {
      vtkOStreamWrapper::UseEndl((const struct vtkOStreamWrapper::EndlType *)v13);
      vtkOStrStreamWrapper::vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v14);
      v21 = vtkOStreamWrapper::operator<<(
              v14,
              "ERROR: In d:\\develop\\yhq\\gcode\\ntcommonlib\\src\\grindsimulater\\src\\vtkgrindsimulatepass.cpp, line ");
      v22 = vtkOStreamWrapper::operator<<(v21, 382);
      vtkOStreamWrapper::operator<<(v22, "\n");
      if ( v12 )
      {
        ClassNameA = vtkObjectBase::GetClassNameA(v12);
        v23 = vtkOStreamWrapper::operator<<(v14, ClassNameA);
        v24 = vtkOStreamWrapper::operator<<(v23, " (");
        v25 = vtkOStreamWrapper::operator<<(v24, v12);
        vtkOStreamWrapper::operator<<(v25, "): ");
      }
      v26 = vtkOStreamWrapper::operator<<(v14, &unk_18004FB4E);
      v27 = vtkOStreamWrapper::operator<<(v26, "fail to init AtomicCounter");
      v28 = vtkOStreamWrapper::operator<<(v27, " ");
      v29 = sub_180002270(v11, v15);
      v30 = v29;
      v31 = sub_18000236F(v29);
      v32 = vtkOStreamWrapper::operator<<(v28, v31);
      vtkOStreamWrapper::operator<<(v32, "\n\n");
      sub_180001B8B(v15);
      if ( v12 && vtkObject::HasObserver(v12, "ErrorEvent") )
      {
        v5 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v14);
        vtkObject::InvokeEvent(v12, "ErrorEvent", v5);
      }
      else
      {
        v6 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v14);
        vtkOutputWindowDisplayErrorText(v6);
      }
      v33 = vtkOStrStreamWrapper::rdbuf((vtkOStrStreamWrapper *)v14);
      vtkOStrStreamWrapper::freeze(v33, 0);
      vtkObject::BreakOnError();
      vtkOStrStreamWrapper::~vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v14);
    }
    return sub_1800023A6(v11);
  }
  return result;
}
// 18000134D: using guessed type __int64 __fastcall sub_18000134D(_QWORD, _QWORD, _QWORD);
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001CDA: using guessed type __int64 __fastcall sub_180001CDA(_DWORD, _DWORD, _DWORD, _DWORD, __int64);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 18000236F: using guessed type __int64 __fastcall sub_18000236F(_QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 1800737C8: using guessed type _DWORD vtkObject::InvokeEvent(vtkObject *__hidden this, const char *, void *);
// 1800737D0: using guessed type _DWORD vtkObject::HasObserver(vtkObject *__hidden this, const char *);
// 1800737D8: using guessed type __int64 vtkObject::GetGlobalWarningDisplay(void);
// 1800737E0: using guessed type __int64 vtkObject::BreakOnError(void);
// 180073800: using guessed type void vtkOutputWindowDisplayErrorText(const char *);
// 180073810: using guessed type const char *vtkObjectBase::GetClassNameA(vtkObjectBase *__hidden this);
// 180073830: using guessed type void vtkOStrStreamWrapper::freeze(vtkOStrStreamWrapper *__hidden this, _DWORD);
// 180073838: using guessed type struct vtkOStrStreamWrapper *vtkOStrStreamWrapper::rdbuf(vtkOStrStreamWrapper *__hidden this);
// 180073840: using guessed type char *vtkOStrStreamWrapper::str(vtkOStrStreamWrapper *__hidden this);
// 180073848: using guessed type void vtkOStrStreamWrapper::~vtkOStrStreamWrapper(vtkOStrStreamWrapper *__hidden this);
// 180073850: using guessed type vtkOStrStreamWrapper *vtkOStrStreamWrapper::vtkOStrStreamWrapper(vtkOStrStreamWrapper *__hidden this);
// 180073858: using guessed type static void vtkOStreamWrapper::UseEndl(const struct vtkOStreamWrapper::EndlType *);
// 180073860: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180073868: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180073870: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180074768: using guessed type __int64 (__fastcall *_glewBufferData)(_QWORD, _QWORD, _QWORD, _QWORD);
// 180074780: using guessed type __int64 (__fastcall *_glewGenBuffers)(_QWORD, _QWORD);
// 180074788: using guessed type __int64 (__fastcall *_glewBindBuffer)(_QWORD, _QWORD);

//----- (000000018001FEE0) ----------------------------------------------------
bool __fastcall sub_18001FEE0(__int64 a1, vtkRenderState *a2)
{
  __int64 *v2; // rdi
  __int64 i; // rcx
  vtkRenderer *Renderer; // rax
  __int64 v6; // [rsp+0h] [rbp-48h] BYREF
  vtkTextureObject *v7; // [rsp+20h] [rbp-28h]
  struct vtkOpenGLRenderWindow *RenderWindow; // [rsp+28h] [rbp-20h]

  v2 = &v6;
  for ( i = 16; i; --i )
  {
    *(_DWORD *)v2 = -858993460;
    v2 = (__int64 *)((char *)v2 + 4);
  }
  *(_QWORD *)(a1 + 248) = vtkTextureObject::New(a1);
  v7 = *(vtkTextureObject **)(a1 + 248);
  Renderer = vtkRenderState::GetRenderer(a2);
  RenderWindow = vtkRenderer::GetRenderWindow(Renderer);
  vtkTextureObject::SetContext(v7, RenderWindow);
  return vtkTextureObject::AllocateDepth(
           *(vtkTextureObject **)(a1 + 248),
           *(_DWORD *)(a1 + 96),
           *(_DWORD *)(a1 + 100),
           6);
}
// 1800742A8: using guessed type struct vtkRenderer *vtkRenderState::GetRenderer(vtkRenderState *__hidden this);
// 1800742E0: using guessed type struct vtkRenderWindow *vtkRenderer::GetRenderWindow(vtkRenderer *__hidden this);
// 1800744A0: using guessed type __int64 __fastcall vtkTextureObject::New(_QWORD);
// 1800744A8: using guessed type void vtkTextureObject::SetContext(vtkTextureObject *__hidden this, struct vtkOpenGLRenderWindow *);
// 1800745B8: using guessed type bool vtkTextureObject::AllocateDepth(vtkTextureObject *__hidden this, unsigned int, unsigned int, _DWORD);

//----- (000000018001FF90) ----------------------------------------------------
__int64 __fastcall sub_18001FF90(__int64 a1, __int64 a2)
{
  __int64 *v2; // rdi
  __int64 i; // rcx
  __int64 result; // rax
  double v5; // xmm0_8
  __int64 v6; // [rsp+0h] [rbp-138h] BYREF
  double v7[6]; // [rsp+38h] [rbp-100h] BYREF
  double v8[6]; // [rsp+68h] [rbp-D0h] BYREF
  _DWORD v9[12]; // [rsp+98h] [rbp-A0h] BYREF
  _QWORD v10[4]; // [rsp+C8h] [rbp-70h] BYREF
  _QWORD v11[2]; // [rsp+E8h] [rbp-50h] BYREF
  double *v12; // [rsp+F8h] [rbp-40h]
  float v13; // [rsp+100h] [rbp-38h]
  int j; // [rsp+104h] [rbp-34h]
  __int64 v15; // [rsp+110h] [rbp-28h]

  v2 = &v6;
  for ( i = 76; i; --i )
  {
    *(_DWORD *)v2 = -858993460;
    v2 = (__int64 *)((char *)v2 + 4);
  }
  result = sub_1800012CB(*(_QWORD *)(a1 + 88));
  if ( (int)result >= 0 )
  {
    v10[0] = 0;
    v11[0] = 0;
    v15 = *(_QWORD *)(a1 + 88);
    result = sub_180001F0A(v15, (unsigned int)v10, (unsigned int)v11, (unsigned int)v8, (__int64)v7, (__int64)v9);
    if ( !v11[0] )
    {
      result = (*(__int64 (__fastcall **)(_QWORD))(**(_QWORD **)(a1 + 144) + 312LL))(*(_QWORD *)(a1 + 144));
      v12 = (double *)result;
      if ( result )
      {
        *v12 = *v12 - 1.0;
        v12[1] = v12[1] + 1.0;
        v12[2] = v12[2] - 1.0;
        v12[3] = v12[3] + 1.0;
        v12[4] = v12[4] - 1.0;
        v12[5] = v12[5] + 1.0;
        v13 = sub_1800022BB(*(_QWORD *)(a1 + 88));
        v7[0] = v13;
        v7[1] = v13;
        v7[2] = v13;
        for ( j = 0; j < 3; ++j )
        {
          v5 = ceil((v12[2 * j + 1] - v12[2 * j]) / v7[j]);
          v9[j] = (int)v5;
        }
        v8[0] = *v12;
        v8[1] = v12[2];
        v8[2] = v12[4];
        sub_180002176(a1, a2, v10, v9);
        sub_180002176(a1, a2, v11, v9);
        return sub_1800026F8(*(_QWORD *)(a1 + 88), v10[0], v11[0], (unsigned int)v8, (__int64)v7, (__int64)v9);
      }
    }
  }
  return result;
}
// 1800012CB: using guessed type __int64 __fastcall sub_1800012CB(_QWORD);
// 180001F0A: using guessed type __int64 __fastcall sub_180001F0A(_DWORD, _DWORD, _DWORD, _DWORD, __int64, __int64);
// 180002176: using guessed type __int64 __fastcall sub_180002176(_QWORD, _QWORD, _QWORD, _QWORD);
// 1800022BB: using guessed type float __fastcall sub_1800022BB(_QWORD);
// 1800026F8: using guessed type __int64 __fastcall sub_1800026F8(_DWORD, _DWORD, _DWORD, _DWORD, __int64, __int64);

//----- (00000001800203F0) ----------------------------------------------------
__int64 __fastcall sub_1800203F0(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  __int64 v4; // [rsp+0h] [rbp-28h] BYREF

  v1 = &v4;
  for ( i = 8; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  return sub_180001CC6(*(_QWORD *)(a1 + 88));
}
// 180001CC6: using guessed type __int64 __fastcall sub_180001CC6(_QWORD);

//----- (0000000180020430) ----------------------------------------------------
// Hidden C++ exception states: #wind=6
__int64 __fastcall sub_180020430(unsigned int *a1)
{
  _DWORD *v1; // rdi
  __int64 i; // rcx
  const char *ClassNameA; // rax
  char *v4; // rax
  const char *v5; // rax
  __int64 result; // rax
  const char *v7; // rax
  char *v8; // rax
  const char *v9; // rax
  _DWORD v10[20]; // [rsp+0h] [rbp-6A8h] BYREF
  _BYTE v11[96]; // [rsp+50h] [rbp-658h] BYREF
  _BYTE v12[132]; // [rsp+B0h] [rbp-5F8h] BYREF
  int v13; // [rsp+134h] [rbp-574h]
  _BYTE v14[248]; // [rsp+150h] [rbp-558h] BYREF
  vtkObjectBase *v15; // [rsp+248h] [rbp-460h]
  _BYTE v16[36]; // [rsp+254h] [rbp-454h] BYREF
  _BYTE v17[48]; // [rsp+278h] [rbp-430h] BYREF
  _BYTE v18[40]; // [rsp+2A8h] [rbp-400h] BYREF
  int v19; // [rsp+2D0h] [rbp-3D8h]
  _BYTE v20[96]; // [rsp+2F0h] [rbp-3B8h] BYREF
  _BYTE v21[132]; // [rsp+350h] [rbp-358h] BYREF
  int v22; // [rsp+3D4h] [rbp-2D4h]
  _BYTE v23[248]; // [rsp+3F0h] [rbp-2B8h] BYREF
  vtkObjectBase *v24; // [rsp+4E8h] [rbp-1C0h]
  _BYTE v25[36]; // [rsp+4F4h] [rbp-1B4h] BYREF
  _BYTE v26[48]; // [rsp+518h] [rbp-190h] BYREF
  _BYTE v27[48]; // [rsp+548h] [rbp-160h] BYREF
  __int64 v28; // [rsp+578h] [rbp-130h]
  __int64 (__fastcall *v29)(_QWORD, _QWORD); // [rsp+580h] [rbp-128h]
  __int64 (__fastcall *v30)(_QWORD, _QWORD); // [rsp+588h] [rbp-120h]
  __int64 (__fastcall *v31)(_QWORD, _QWORD, _QWORD, _QWORD); // [rsp+590h] [rbp-118h]
  __int64 (__fastcall *v32)(_QWORD, _QWORD); // [rsp+598h] [rbp-110h]
  __int64 v33; // [rsp+5A0h] [rbp-108h]
  __int64 v34; // [rsp+5A8h] [rbp-100h]
  __int64 v35; // [rsp+5B0h] [rbp-F8h]
  __int64 v36; // [rsp+5B8h] [rbp-F0h]
  __int64 v37; // [rsp+5C0h] [rbp-E8h]
  __int64 v38; // [rsp+5C8h] [rbp-E0h]
  __int64 v39; // [rsp+5D0h] [rbp-D8h]
  __int64 v40; // [rsp+5D8h] [rbp-D0h]
  __int64 v41; // [rsp+5E0h] [rbp-C8h]
  __int64 v42; // [rsp+5E8h] [rbp-C0h]
  __int64 v43; // [rsp+5F0h] [rbp-B8h]
  __int64 v44; // [rsp+5F8h] [rbp-B0h]
  vtkOStrStreamWrapper *v45; // [rsp+600h] [rbp-A8h]
  __int64 (__fastcall *v46)(_QWORD, _QWORD); // [rsp+608h] [rbp-A0h]
  __int64 (__fastcall *v47)(_QWORD, _QWORD); // [rsp+610h] [rbp-98h]
  __int64 (__fastcall *v48)(_QWORD, _QWORD, _QWORD, _QWORD); // [rsp+618h] [rbp-90h]
  __int64 (__fastcall *v49)(_QWORD, _QWORD); // [rsp+620h] [rbp-88h]
  __int64 v50; // [rsp+628h] [rbp-80h]
  __int64 v51; // [rsp+630h] [rbp-78h]
  __int64 v52; // [rsp+638h] [rbp-70h]
  __int64 v53; // [rsp+640h] [rbp-68h]
  __int64 v54; // [rsp+648h] [rbp-60h]
  __int64 v55; // [rsp+650h] [rbp-58h]
  __int64 v56; // [rsp+658h] [rbp-50h]
  __int64 v57; // [rsp+660h] [rbp-48h]
  __int64 v58; // [rsp+668h] [rbp-40h]
  __int64 v59; // [rsp+670h] [rbp-38h]
  __int64 v60; // [rsp+678h] [rbp-30h]
  __int64 v61; // [rsp+680h] [rbp-28h]
  vtkOStrStreamWrapper *v62; // [rsp+688h] [rbp-20h]

  v1 = v10;
  for ( i = 424; i; --i )
    *v1++ = -858993460;
  v28 = -2;
  if ( !a1[64] )
  {
    v29 = _glewGenBuffers;
    _glewGenBuffers(1, a1 + 64);
    v30 = _glewBindBuffer;
    _glewBindBuffer(37074, a1[64]);
    v31 = _glewBufferData;
    _glewBufferData(37074, 16000, 0, 35048);
    v32 = _glewBindBuffer;
    _glewBindBuffer(37074, 0);
    v10[12] = 16;
    memset(v11, 0, 0x40u);
    memset(v12, 0, 0x80u);
    v13 = sub_18000134D(16, v11, v12);
    if ( v13 )
    {
      sub_18000139D(v14, 2, 1);
      sub_180001CDA((unsigned int)v14, 16, v13, (unsigned int)v11, (__int64)v12);
      v15 = (vtkObjectBase *)a1;
      if ( (unsigned int)vtkObject::GetGlobalWarningDisplay() )
      {
        vtkOStreamWrapper::UseEndl((const struct vtkOStreamWrapper::EndlType *)v16);
        vtkOStrStreamWrapper::vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v17);
        v33 = vtkOStreamWrapper::operator<<(
                v17,
                "ERROR: In d:\\develop\\yhq\\gcode\\ntcommonlib\\src\\grindsimulater\\src\\vtkgrindsimulatepass.cpp, line ");
        v34 = vtkOStreamWrapper::operator<<(v33, 308);
        vtkOStreamWrapper::operator<<(v34, "\n");
        if ( v15 )
        {
          ClassNameA = vtkObjectBase::GetClassNameA(v15);
          v35 = vtkOStreamWrapper::operator<<(v17, ClassNameA);
          v36 = vtkOStreamWrapper::operator<<(v35, " (");
          v37 = vtkOStreamWrapper::operator<<(v36, v15);
          vtkOStreamWrapper::operator<<(v37, "): ");
        }
        v38 = vtkOStreamWrapper::operator<<(v17, &unk_18004FAF7);
        v39 = vtkOStreamWrapper::operator<<(v38, "fail to init AreaPolygonSSBO");
        v40 = vtkOStreamWrapper::operator<<(v39, " ");
        v41 = sub_180002270(v14, v18);
        v42 = v41;
        v43 = sub_18000236F(v41);
        v44 = vtkOStreamWrapper::operator<<(v40, v43);
        vtkOStreamWrapper::operator<<(v44, "\n\n");
        sub_180001B8B(v18);
        if ( v15 && vtkObject::HasObserver(v15, "ErrorEvent") )
        {
          v4 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v17);
          vtkObject::InvokeEvent(v15, "ErrorEvent", v4);
        }
        else
        {
          v5 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v17);
          vtkOutputWindowDisplayErrorText(v5);
        }
        v45 = vtkOStrStreamWrapper::rdbuf((vtkOStrStreamWrapper *)v17);
        vtkOStrStreamWrapper::freeze(v45, 0);
        vtkObject::BreakOnError();
        vtkOStrStreamWrapper::~vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v17);
      }
      sub_1800023A6(v14);
    }
  }
  result = (__int64)a1;
  if ( !a1[65] )
  {
    v46 = _glewGenBuffers;
    _glewGenBuffers(1, a1 + 65);
    v47 = _glewBindBuffer;
    _glewBindBuffer(37074, a1[65]);
    v48 = _glewBufferData;
    _glewBufferData(37074, 16000, 0, 35048);
    v49 = _glewBindBuffer;
    _glewBindBuffer(37074, 0);
    v19 = 16;
    memset(v20, 0, 0x40u);
    memset(v21, 0, 0x80u);
    result = sub_18000134D(16, v20, v21);
    v22 = result;
    if ( (_DWORD)result )
    {
      sub_18000139D(v23, 2, 1);
      sub_180001CDA((unsigned int)v23, 16, v22, (unsigned int)v20, (__int64)v21);
      v24 = (vtkObjectBase *)a1;
      if ( (unsigned int)vtkObject::GetGlobalWarningDisplay() )
      {
        vtkOStreamWrapper::UseEndl((const struct vtkOStreamWrapper::EndlType *)v25);
        vtkOStrStreamWrapper::vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v26);
        v50 = vtkOStreamWrapper::operator<<(
                v26,
                "ERROR: In d:\\develop\\yhq\\gcode\\ntcommonlib\\src\\grindsimulater\\src\\vtkgrindsimulatepass.cpp, line ");
        v51 = vtkOStreamWrapper::operator<<(v50, 316);
        vtkOStreamWrapper::operator<<(v51, "\n");
        if ( v24 )
        {
          v7 = vtkObjectBase::GetClassNameA(v24);
          v52 = vtkOStreamWrapper::operator<<(v26, v7);
          v53 = vtkOStreamWrapper::operator<<(v52, " (");
          v54 = vtkOStreamWrapper::operator<<(v53, v24);
          vtkOStreamWrapper::operator<<(v54, "): ");
        }
        v55 = vtkOStreamWrapper::operator<<(v26, &unk_18004FB15);
        v56 = vtkOStreamWrapper::operator<<(v55, "fail to init ToolPolygonSSBO");
        v57 = vtkOStreamWrapper::operator<<(v56, " ");
        v58 = sub_180002270(v23, v27);
        v59 = v58;
        v60 = sub_18000236F(v58);
        v61 = vtkOStreamWrapper::operator<<(v57, v60);
        vtkOStreamWrapper::operator<<(v61, "\n\n");
        sub_180001B8B(v27);
        if ( v24 && vtkObject::HasObserver(v24, "ErrorEvent") )
        {
          v8 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v26);
          vtkObject::InvokeEvent(v24, "ErrorEvent", v8);
        }
        else
        {
          v9 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v26);
          vtkOutputWindowDisplayErrorText(v9);
        }
        v62 = vtkOStrStreamWrapper::rdbuf((vtkOStrStreamWrapper *)v26);
        vtkOStrStreamWrapper::freeze(v62, 0);
        vtkObject::BreakOnError();
        vtkOStrStreamWrapper::~vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v26);
      }
      return sub_1800023A6(v23);
    }
  }
  return result;
}
// 18000134D: using guessed type __int64 __fastcall sub_18000134D(_QWORD, _QWORD, _QWORD);
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001CDA: using guessed type __int64 __fastcall sub_180001CDA(_DWORD, _DWORD, _DWORD, _DWORD, __int64);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 18000236F: using guessed type __int64 __fastcall sub_18000236F(_QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 1800737C8: using guessed type _DWORD vtkObject::InvokeEvent(vtkObject *__hidden this, const char *, void *);
// 1800737D0: using guessed type _DWORD vtkObject::HasObserver(vtkObject *__hidden this, const char *);
// 1800737D8: using guessed type __int64 vtkObject::GetGlobalWarningDisplay(void);
// 1800737E0: using guessed type __int64 vtkObject::BreakOnError(void);
// 180073800: using guessed type void vtkOutputWindowDisplayErrorText(const char *);
// 180073810: using guessed type const char *vtkObjectBase::GetClassNameA(vtkObjectBase *__hidden this);
// 180073830: using guessed type void vtkOStrStreamWrapper::freeze(vtkOStrStreamWrapper *__hidden this, _DWORD);
// 180073838: using guessed type struct vtkOStrStreamWrapper *vtkOStrStreamWrapper::rdbuf(vtkOStrStreamWrapper *__hidden this);
// 180073840: using guessed type char *vtkOStrStreamWrapper::str(vtkOStrStreamWrapper *__hidden this);
// 180073848: using guessed type void vtkOStrStreamWrapper::~vtkOStrStreamWrapper(vtkOStrStreamWrapper *__hidden this);
// 180073850: using guessed type vtkOStrStreamWrapper *vtkOStrStreamWrapper::vtkOStrStreamWrapper(vtkOStrStreamWrapper *__hidden this);
// 180073858: using guessed type static void vtkOStreamWrapper::UseEndl(const struct vtkOStreamWrapper::EndlType *);
// 180073860: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180073868: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180073870: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180074768: using guessed type __int64 (__fastcall *_glewBufferData)(_QWORD, _QWORD, _QWORD, _QWORD);
// 180074780: using guessed type __int64 (__fastcall *_glewGenBuffers)(_QWORD, _QWORD);
// 180074788: using guessed type __int64 (__fastcall *_glewBindBuffer)(_QWORD, _QWORD);

//----- (0000000180020C80) ----------------------------------------------------
// Hidden C++ exception states: #wind=8
void __fastcall sub_180020C80(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  const char *ClassNameA; // rax
  char *v4; // rax
  const char *v5; // rax
  __int64 v6; // [rsp+0h] [rbp-598h] BYREF
  _BYTE v7[40]; // [rsp+58h] [rbp-540h] BYREF
  _BYTE v8[248]; // [rsp+80h] [rbp-518h] BYREF
  _BYTE v9[48]; // [rsp+178h] [rbp-420h] BYREF
  _BYTE v10[60]; // [rsp+1A8h] [rbp-3F0h] BYREF
  _DWORD v11[11]; // [rsp+1E4h] [rbp-3B4h] BYREF
  _BYTE v12[96]; // [rsp+210h] [rbp-388h] BYREF
  _BYTE v13[132]; // [rsp+270h] [rbp-328h] BYREF
  int v14; // [rsp+2F4h] [rbp-2A4h]
  _BYTE v15[248]; // [rsp+310h] [rbp-288h] BYREF
  vtkObjectBase *v16; // [rsp+408h] [rbp-190h]
  _BYTE v17[36]; // [rsp+414h] [rbp-184h] BYREF
  _BYTE v18[48]; // [rsp+438h] [rbp-160h] BYREF
  _BYTE v19[48]; // [rsp+468h] [rbp-130h] BYREF
  _BYTE v20[8]; // [rsp+498h] [rbp-100h] BYREF
  _BYTE v21[4]; // [rsp+4A0h] [rbp-F8h] BYREF
  int v22; // [rsp+4A4h] [rbp-F4h] BYREF
  __int64 v23; // [rsp+4A8h] [rbp-F0h]
  __int64 v24; // [rsp+4B0h] [rbp-E8h]
  __int64 v25; // [rsp+4B8h] [rbp-E0h]
  __int64 v26; // [rsp+4C0h] [rbp-D8h]
  __int64 v27; // [rsp+4C8h] [rbp-D0h]
  __int64 started; // [rsp+4D0h] [rbp-C8h]
  __int64 v29; // [rsp+4D8h] [rbp-C0h]
  __int64 v30; // [rsp+4E0h] [rbp-B8h]
  __int64 v31; // [rsp+4E8h] [rbp-B0h]
  GLvoid *pixels; // [rsp+4F0h] [rbp-A8h]
  GLsizei height; // [rsp+4F8h] [rbp-A0h]
  GLsizei width; // [rsp+4FCh] [rbp-9Ch]
  __int64 (__fastcall *v35)(_QWORD, _QWORD); // [rsp+500h] [rbp-98h]
  __int64 (__fastcall *v36)(_QWORD, _QWORD, _QWORD, _QWORD); // [rsp+508h] [rbp-90h]
  __int64 (__fastcall *v37)(_QWORD, _QWORD); // [rsp+510h] [rbp-88h]
  __int64 v38; // [rsp+518h] [rbp-80h]
  __int64 v39; // [rsp+520h] [rbp-78h]
  __int64 v40; // [rsp+528h] [rbp-70h]
  __int64 v41; // [rsp+530h] [rbp-68h]
  __int64 v42; // [rsp+538h] [rbp-60h]
  __int64 v43; // [rsp+540h] [rbp-58h]
  __int64 v44; // [rsp+548h] [rbp-50h]
  __int64 v45; // [rsp+550h] [rbp-48h]
  __int64 v46; // [rsp+558h] [rbp-40h]
  __int64 v47; // [rsp+560h] [rbp-38h]
  __int64 v48; // [rsp+568h] [rbp-30h]
  __int64 v49; // [rsp+570h] [rbp-28h]
  vtkOStrStreamWrapper *v50; // [rsp+578h] [rbp-20h]
  vtkTextureObject *v51; // [rsp+580h] [rbp-18h]

  v1 = &v6;
  for ( i = 356; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  v23 = -2;
  vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v7);
  sub_18000139D(v8, 2, 1);
  sub_1800017EE(v8, "vtkGrindSimulatePass::Prepare");
  v24 = *(_QWORD *)(a1 + 104);
  v25 = sub_180002270(v8, v9);
  v26 = v25;
  v27 = v25;
  started = vtkRenderTimerLog::StartScopedEvent(v24, v20, v25);
  v29 = started;
  vtkRenderTimerLog::ScopedEventLogger::operator=(v7, started);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v20);
  sub_180001B8B(v9);
  sub_1800023A6(v8);
  v30 = sub_180001B59(v21);
  v22 = -1;
  v31 = *(_DWORD *)(a1 + 100) * *(_DWORD *)(a1 + 96);
  sub_18000247D(v10, v31, &v22, v30);
  glBindTexture(0xDE1u, *(_DWORD *)(a1 + 160));
  pixels = (GLvoid *)sub_180002649(v10);
  height = *(_DWORD *)(a1 + 100);
  width = *(_DWORD *)(a1 + 96);
  glTexSubImage2D(0xDE1u, 0, 0, 0, width, height, 0x8D94u, 0x1405u, pixels);
  glBindTexture(0xDE1u, 0);
  v11[0] = 0;
  v35 = _glewBindBuffer;
  _glewBindBuffer(37568, *(unsigned int *)(a1 + 168));
  v36 = _glewBufferSubData;
  _glewBufferSubData(37568, 0, 4, v11);
  v37 = _glewBindBuffer;
  _glewBindBuffer(37568, 0);
  v11[4] = 16;
  memset(v12, 0, 0x40u);
  memset(v13, 0, 0x80u);
  v14 = sub_18000134D(16, v12, v13);
  if ( v14 )
  {
    sub_18000139D(v15, 2, 1);
    sub_180001CDA((unsigned int)v15, 16, v14, (unsigned int)v12, (__int64)v13);
    v16 = (vtkObjectBase *)a1;
    if ( (unsigned int)vtkObject::GetGlobalWarningDisplay() )
    {
      vtkOStreamWrapper::UseEndl((const struct vtkOStreamWrapper::EndlType *)v17);
      vtkOStrStreamWrapper::vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v18);
      v38 = vtkOStreamWrapper::operator<<(
              v18,
              "ERROR: In d:\\develop\\yhq\\gcode\\ntcommonlib\\src\\grindsimulater\\src\\vtkgrindsimulatepass.cpp, line ");
      v39 = vtkOStreamWrapper::operator<<(v38, 351);
      vtkOStreamWrapper::operator<<(v39, "\n");
      if ( v16 )
      {
        ClassNameA = vtkObjectBase::GetClassNameA(v16);
        v40 = vtkOStreamWrapper::operator<<(v18, ClassNameA);
        v41 = vtkOStreamWrapper::operator<<(v40, " (");
        v42 = vtkOStreamWrapper::operator<<(v41, v16);
        vtkOStreamWrapper::operator<<(v42, "): ");
      }
      v43 = vtkOStreamWrapper::operator<<(v18, &unk_18004FB16);
      v44 = vtkOStreamWrapper::operator<<(v43, &unk_1800522E8);
      v45 = vtkOStreamWrapper::operator<<(v44, " ");
      v46 = sub_180002270(v15, v19);
      v47 = v46;
      v48 = sub_18000236F(v46);
      v49 = vtkOStreamWrapper::operator<<(v45, v48);
      vtkOStreamWrapper::operator<<(v49, "\n\n");
      sub_180001B8B(v19);
      if ( v16 && vtkObject::HasObserver(v16, "ErrorEvent") )
      {
        v4 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v18);
        vtkObject::InvokeEvent(v16, "ErrorEvent", v4);
      }
      else
      {
        v5 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v18);
        vtkOutputWindowDisplayErrorText(v5);
      }
      v50 = vtkOStrStreamWrapper::rdbuf((vtkOStrStreamWrapper *)v18);
      vtkOStrStreamWrapper::freeze(v50, 0);
      vtkObject::BreakOnError();
      vtkOStrStreamWrapper::~vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v18);
    }
    sub_1800023A6(v15);
  }
  v51 = *(vtkTextureObject **)(a1 + 248);
  vtkTextureObject::CopyFromFrameBuffer(v51, 0, 0, 0, 0, *(_DWORD *)(a1 + 96), *(_DWORD *)(a1 + 100));
  sub_180002329(v10);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v7);
}
// 18000134D: using guessed type __int64 __fastcall sub_18000134D(_QWORD, _QWORD, _QWORD);
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 1800017EE: using guessed type __int64 __fastcall sub_1800017EE(_QWORD, _QWORD);
// 180001B59: using guessed type __int64 __fastcall sub_180001B59(_QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001CDA: using guessed type __int64 __fastcall sub_180001CDA(_DWORD, _DWORD, _DWORD, _DWORD, __int64);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 180002329: using guessed type __int64 __fastcall sub_180002329(_QWORD);
// 18000236F: using guessed type __int64 __fastcall sub_18000236F(_QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 18000247D: using guessed type __int64 __fastcall sub_18000247D(_QWORD, _QWORD, _QWORD, _QWORD);
// 180002649: using guessed type __int64 __fastcall sub_180002649(_QWORD);
// 1800737C8: using guessed type _DWORD vtkObject::InvokeEvent(vtkObject *__hidden this, const char *, void *);
// 1800737D0: using guessed type _DWORD vtkObject::HasObserver(vtkObject *__hidden this, const char *);
// 1800737D8: using guessed type __int64 vtkObject::GetGlobalWarningDisplay(void);
// 1800737E0: using guessed type __int64 vtkObject::BreakOnError(void);
// 180073800: using guessed type void vtkOutputWindowDisplayErrorText(const char *);
// 180073810: using guessed type const char *vtkObjectBase::GetClassNameA(vtkObjectBase *__hidden this);
// 180073830: using guessed type void vtkOStrStreamWrapper::freeze(vtkOStrStreamWrapper *__hidden this, _DWORD);
// 180073838: using guessed type struct vtkOStrStreamWrapper *vtkOStrStreamWrapper::rdbuf(vtkOStrStreamWrapper *__hidden this);
// 180073840: using guessed type char *vtkOStrStreamWrapper::str(vtkOStrStreamWrapper *__hidden this);
// 180073848: using guessed type void vtkOStrStreamWrapper::~vtkOStrStreamWrapper(vtkOStrStreamWrapper *__hidden this);
// 180073850: using guessed type vtkOStrStreamWrapper *vtkOStrStreamWrapper::vtkOStrStreamWrapper(vtkOStrStreamWrapper *__hidden this);
// 180073858: using guessed type static void vtkOStreamWrapper::UseEndl(const struct vtkOStreamWrapper::EndlType *);
// 180073860: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180073868: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180073870: using guessed type __int64 __fastcall vtkOStreamWrapper::operator<<(_QWORD, _QWORD);
// 180074260: using guessed type __int64 __fastcall vtkRenderTimerLog::StartScopedEvent(_QWORD, _QWORD, _QWORD);
// 180074268: using guessed type void vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 180074270: using guessed type __int64 __fastcall vtkRenderTimerLog::ScopedEventLogger::operator=(_QWORD, _QWORD);
// 180074278: using guessed type _QWORD vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 1800745A8: using guessed type void vtkTextureObject::CopyFromFrameBuffer(vtkTextureObject *__hidden this, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD);
// 180074770: using guessed type __int64 (__fastcall *_glewBufferSubData)(_QWORD, _QWORD, _QWORD, _QWORD);
// 180074788: using guessed type __int64 (__fastcall *_glewBindBuffer)(_QWORD, _QWORD);

//----- (0000000180021330) ----------------------------------------------------
// Hidden C++ exception states: #wind=4
void __fastcall sub_180021330(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  __int64 v3; // [rsp+0h] [rbp-238h] BYREF
  _BYTE v4[40]; // [rsp+58h] [rbp-1E0h] BYREF
  _BYTE v5[248]; // [rsp+80h] [rbp-1B8h] BYREF
  _BYTE v6[48]; // [rsp+178h] [rbp-C0h] BYREF
  _BYTE v7[8]; // [rsp+1A8h] [rbp-90h] BYREF
  __int64 v8; // [rsp+1B0h] [rbp-88h]
  __int64 v9; // [rsp+1B8h] [rbp-80h]
  __int64 v10; // [rsp+1C0h] [rbp-78h]
  __int64 v11; // [rsp+1C8h] [rbp-70h]
  __int64 v12; // [rsp+1D0h] [rbp-68h]
  __int64 started; // [rsp+1D8h] [rbp-60h]
  __int64 v14; // [rsp+1E0h] [rbp-58h]
  vtkOpenGLFramebufferObject *v15; // [rsp+1E8h] [rbp-50h]
  __int64 (__fastcall *v16)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD, _DWORD, _DWORD); // [rsp+1F0h] [rbp-48h]
  __int64 (__fastcall *v17)(_QWORD, _QWORD, _QWORD); // [rsp+1F8h] [rbp-40h]
  __int64 (__fastcall *v18)(_QWORD, _QWORD, _QWORD); // [rsp+200h] [rbp-38h]
  __int64 (__fastcall *v19)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD, _DWORD, _DWORD); // [rsp+208h] [rbp-30h]
  __int64 (__fastcall *v20)(_QWORD, _QWORD, _QWORD); // [rsp+210h] [rbp-28h]
  __int64 (__fastcall *v21)(_QWORD, _QWORD, _QWORD); // [rsp+218h] [rbp-20h]

  v1 = &v3;
  for ( i = 140; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  v8 = -2;
  vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v4);
  sub_18000139D(v5, 2, 1);
  sub_1800017EE(v5, "vtkGrindSimulatePass::UpdateABuffer");
  v9 = *(_QWORD *)(a1 + 104);
  v10 = sub_180002270(v5, v6);
  v11 = v10;
  v12 = v10;
  started = vtkRenderTimerLog::StartScopedEvent(v9, v7, v10);
  v14 = started;
  vtkRenderTimerLog::ScopedEventLogger::operator=(v4, started);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v7);
  sub_180001B8B(v6);
  sub_1800023A6(v5);
  vtkOpenGLFramebufferObject::SaveCurrentBindingsAndBuffers(*(vtkOpenGLFramebufferObject **)(a1 + 152));
  vtkOpenGLFramebufferObject::Bind(*(vtkOpenGLFramebufferObject **)(a1 + 152));
  v15 = *(vtkOpenGLFramebufferObject **)(a1 + 152);
  vtkOpenGLFramebufferObject::PopulateFramebuffer(v15, *(_DWORD *)(a1 + 96), *(_DWORD *)(a1 + 100), 1, 1, 3, 0, 0, 0);
  glViewport(0, 0, *(_DWORD *)(a1 + 96), *(_DWORD *)(a1 + 100));
  glColorMask(0, 0, 0, 0);
  glDepthMask(0);
  glEnable(0xB71u);
  v16 = _glewBindImageTexture;
  _glewBindImageTexture(0, *(unsigned int *)(a1 + 160), 0, 0, 0, 35002, 33334);
  v17 = _glewBindBufferBase;
  _glewBindBufferBase(37074, 0, *(unsigned int *)(a1 + 164));
  v18 = _glewBindBufferBase;
  _glewBindBufferBase(37568, 0, *(unsigned int *)(a1 + 168));
  sub_180001C08(a1, 2);
  sub_180001AE1(a1);
  v19 = _glewBindImageTexture;
  _glewBindImageTexture(0, 0, 0, 0, 0, 35002, 33334);
  v20 = _glewBindBufferBase;
  _glewBindBufferBase(37074, 0, 0);
  v21 = _glewBindBufferBase;
  _glewBindBufferBase(37568, 0, 0);
  glColorMask(1u, 1u, 1u, 1u);
  glDepthMask(1u);
  vtkOpenGLFramebufferObject::RestorePreviousBindingsAndBuffers(*(vtkOpenGLFramebufferObject **)(a1 + 152));
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v4);
}
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 1800017EE: using guessed type __int64 __fastcall sub_1800017EE(_QWORD, _QWORD);
// 180001AE1: using guessed type __int64 __fastcall sub_180001AE1(_QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001C08: using guessed type double __fastcall sub_180001C08(_QWORD, _QWORD);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 180074260: using guessed type __int64 __fastcall vtkRenderTimerLog::StartScopedEvent(_QWORD, _QWORD, _QWORD);
// 180074268: using guessed type void vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 180074270: using guessed type __int64 __fastcall vtkRenderTimerLog::ScopedEventLogger::operator=(_QWORD, _QWORD);
// 180074278: using guessed type _QWORD vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 180074680: using guessed type bool vtkOpenGLFramebufferObject::PopulateFramebuffer(vtkOpenGLFramebufferObject *__hidden this, _DWORD, _DWORD, bool, _DWORD, _DWORD, bool, _DWORD, _DWORD);
// 180074688: using guessed type void vtkOpenGLFramebufferObject::RestorePreviousBindingsAndBuffers(vtkOpenGLFramebufferObject *__hidden this);
// 180074690: using guessed type void vtkOpenGLFramebufferObject::SaveCurrentBindingsAndBuffers(vtkOpenGLFramebufferObject *__hidden this);
// 180074698: using guessed type void vtkOpenGLFramebufferObject::Bind(vtkOpenGLFramebufferObject *__hidden this);
// 180074790: using guessed type __int64 (__fastcall *_glewBindImageTexture)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD, _DWORD, _DWORD);
// 1800747A0: using guessed type __int64 (__fastcall *_glewBindBufferBase)(_QWORD, _QWORD, _QWORD);

//----- (0000000180021700) ----------------------------------------------------
// Hidden C++ exception states: #wind=9
void __fastcall sub_180021700(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  vtkRenderer *Renderer; // rax
  __int64 v4; // r9
  __int64 v5; // r9
  __int64 v6; // r9
  __int64 v7; // r9
  __int64 v8; // r9
  __int64 v9; // [rsp+0h] [rbp-358h] BYREF
  _BYTE v10[48]; // [rsp+48h] [rbp-310h] BYREF
  char v11; // [rsp+78h] [rbp-2E0h] BYREF
  GLsizei width[12]; // [rsp+A8h] [rbp-2B0h] BYREF
  __int64 v13; // [rsp+D8h] [rbp-280h] BYREF
  __int64 v14; // [rsp+F8h] [rbp-260h] BYREF
  struct vtkRenderWindow *RenderWindow; // [rsp+108h] [rbp-250h]
  _BYTE v16[64]; // [rsp+118h] [rbp-240h] BYREF
  _BYTE v17[64]; // [rsp+158h] [rbp-200h] BYREF
  _BYTE v18[48]; // [rsp+198h] [rbp-1C0h] BYREF
  _BYTE v19[40]; // [rsp+1C8h] [rbp-190h] BYREF
  _BYTE v20[40]; // [rsp+1F0h] [rbp-168h] BYREF
  _BYTE v21[40]; // [rsp+218h] [rbp-140h] BYREF
  _BYTE v22[40]; // [rsp+240h] [rbp-118h] BYREF
  _BYTE v23[40]; // [rsp+268h] [rbp-F0h] BYREF
  _BYTE v24[40]; // [rsp+290h] [rbp-C8h] BYREF
  unsigned int v25; // [rsp+2B8h] [rbp-A0h]
  unsigned int v26; // [rsp+2BCh] [rbp-9Ch]
  GLsizei j; // [rsp+2C0h] [rbp-98h]
  __int64 v28; // [rsp+2D0h] [rbp-88h]
  __int64 v29; // [rsp+2D8h] [rbp-80h]
  __int64 v30; // [rsp+2E0h] [rbp-78h]
  __int64 v31; // [rsp+2E8h] [rbp-70h]
  __int64 (__fastcall *v32)(__int64, __int64, __int64, __int64, _QWORD); // [rsp+2F0h] [rbp-68h]
  __int64 v33; // [rsp+2F8h] [rbp-60h]
  __int64 v34; // [rsp+300h] [rbp-58h]
  __int64 v35; // [rsp+308h] [rbp-50h]
  __int64 v36; // [rsp+310h] [rbp-48h]
  void (__fastcall *v37)(__int64, __int64, _QWORD); // [rsp+318h] [rbp-40h]
  __int64 v38; // [rsp+320h] [rbp-38h]
  vtkShaderProgram *v39; // [rsp+328h] [rbp-30h]
  __int64 (__fastcall *v40)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD, _DWORD, _DWORD); // [rsp+330h] [rbp-28h]
  __int64 (__fastcall *v41)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD, _DWORD, _DWORD); // [rsp+338h] [rbp-20h]
  vtkShaderProgram *v42; // [rsp+340h] [rbp-18h]

  v1 = &v9;
  for ( i = 212; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  v28 = -2;
  if ( (unsigned __int8)sub_180001109(*(_QWORD *)(a1 + 88)) )
  {
    v29 = *(_QWORD *)(a1 + 88);
    sub_180001230(v29, 0);
    if ( (int)sub_1800012CB(*(_QWORD *)(a1 + 88)) >= 0 )
    {
      v13 = 0;
      v14 = 0;
      v30 = *(_QWORD *)(a1 + 88);
      sub_180001F0A(v30, (unsigned int)&v13, (unsigned int)&v14, (unsigned int)&v11, (__int64)v10, (__int64)width);
      vtkOpenGLFramebufferObject::SaveCurrentBindingsAndBuffers(*(vtkOpenGLFramebufferObject **)(a1 + 152));
      vtkOpenGLFramebufferObject::Bind(*(vtkOpenGLFramebufferObject **)(a1 + 152));
      glViewport(0, 0, width[0], width[1]);
      Renderer = vtkRenderState::GetRenderer(*(vtkRenderState **)(a1 + 112));
      RenderWindow = vtkRenderer::GetRenderWindow(Renderer);
      if ( *(_QWORD *)(a1 + 176) )
      {
        v36 = (*(__int64 (__fastcall **)(struct vtkRenderWindow *))(*(_QWORD *)RenderWindow + 1624LL))(RenderWindow);
        v37 = *(void (__fastcall **)(__int64, __int64, _QWORD))(*(_QWORD *)v36 + 144LL);
        v38 = *(_QWORD *)(a1 + 176);
        v37(v36, v38, 0);
      }
      else
      {
        sub_1800026FD(
          v16,
          "#version 430 core\n"
          "//YHQ MergeTool vs\n"
          "layout(location = 0) in vec4 position;\n"
          "void main() {\n"
          "   gl_Position = vec4(0.0, 0.0, 0.0, 1.0);\n"
          "}");
        sub_1800026FD(
          v17,
          "#version 430 core\n"
          "//YHQ MergeTool gs\n"
          "uniform int layer;\n"
          "layout(triangles) in;\n"
          "layout(triangle_strip, max_vertices = 6) out;\n"
          "void main() {\n"
          "   gl_Layer = layer;\n"
          "   gl_Position = vec4(-1.0, -1.0, 0.0, 1.0);\n"
          "   EmitVertex();\n"
          "   gl_Position = vec4(1.0, -1.0, 0.0, 1.0);\n"
          "   EmitVertex();\n"
          "   gl_Position = vec4(-1.0, 1.0, 0.0, 1.0);\n"
          "   EmitVertex();\n"
          "   EndPrimitive();\n"
          "   gl_Position = vec4(1.0, -1.0, 0.0, 1.0);\n"
          "   EmitVertex();\n"
          "   gl_Position = vec4(1.0, 1.0, 0.0, 1.0);\n"
          "   EmitVertex();\n"
          "   gl_Position = vec4(-1.0, 1.0, 0.0, 1.0);\n"
          "   EmitVertex();\n"
          "   EndPrimitive();\n"
          "}\n"
          "\n");
        vtkOpenGLRenderUtilities::GetFullScreenQuadFragmentShaderTemplate(v18);
        sub_1800026FD(v19, "#version 430 core\n//YHQ MergeTool fs\n\n");
        sub_1800026FD(v20, "//VTK::System::Dec");
        LOBYTE(v4) = 1;
        vtkShaderProgram::Substitute(v18, v20, v19, v4);
        sub_180001B8B(v20);
        sub_180001B8B(v19);
        sub_1800026FD(
          v21,
          "layout(r32f, binding = 0) uniform image3D texOld;\n"
          "layout(r32f, binding = 1) uniform image3D texNow;\n"
          "uniform float maxf;\n");
        sub_1800026FD(v22, "//VTK::FSQ::Decl");
        LOBYTE(v5) = 1;
        vtkShaderProgram::Substitute(v18, v22, v21, v5);
        sub_180001B8B(v22);
        sub_180001B8B(v21);
        sub_1800026FD(
          v23,
          "   ivec3 voxelCoord = ivec3(gl_FragCoord.x, gl_FragCoord.y, gl_Layer);\n"
          "   float odist = imageLoad(texOld, voxelCoord).x;\n"
          "   float ndist = imageLoad(texNow, voxelCoord).x;\n"
          "   if (ndist < odist) {\n"
          "       imageStore(texOld, voxelCoord, vec4(ndist, 0, 0, 0));\n"
          "   }\n"
          "   imageStore(texNow, voxelCoord, vec4(maxf, 0, 0, 0));\n");
        sub_1800026FD(v24, "//VTK::FSQ::Impl");
        LOBYTE(v6) = 1;
        vtkShaderProgram::Substitute(v18, v24, v23, v6);
        sub_180001B8B(v24);
        sub_180001B8B(v23);
        v31 = (*(__int64 (__fastcall **)(struct vtkRenderWindow *))(*(_QWORD *)RenderWindow + 1624LL))(RenderWindow);
        v32 = *(__int64 (__fastcall **)(__int64, __int64, __int64, __int64, _QWORD))(*(_QWORD *)v31 + 160LL);
        v33 = sub_18000236F(v17);
        v34 = sub_18000236F(v18);
        v35 = sub_18000236F(v16);
        *(_QWORD *)(a1 + 176) = v32(v31, v35, v34, v33, 0);
        sub_180001B8B(v18);
        sub_180001B8B(v17);
        sub_180001B8B(v16);
      }
      if ( *(_QWORD *)(a1 + 176) )
      {
        if ( !*(_QWORD *)(a1 + 184) )
        {
          *(_QWORD *)(a1 + 192) = vtkOpenGLBufferObject::New();
          *(_QWORD *)(a1 + 184) = vtkOpenGLVertexArrayObject::New();
          sub_1800016BD(*(_QWORD *)(a1 + 192), *(_QWORD *)(a1 + 184), *(_QWORD *)(a1 + 176));
        }
        vtkOpenGLVertexArrayObject::Bind(*(vtkOpenGLVertexArrayObject **)(a1 + 184));
        sub_180001C08(a1, 0);
        v39 = *(vtkShaderProgram **)(a1 + 176);
        vtkShaderProgram::SetUniformf(v39, "maxf", 9.9999997e37);
        v25 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)v13 + 208LL))(v13);
        v40 = _glewBindImageTexture;
        LOBYTE(v7) = 1;
        _glewBindImageTexture(0, v25, 0, v7, 0, 35002, 33326);
        v26 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)v14 + 208LL))(v14);
        v41 = _glewBindImageTexture;
        LOBYTE(v8) = 1;
        _glewBindImageTexture(1, v26, 0, v8, 0, 35002, 33326);
        for ( j = 0; j < width[2]; ++j )
        {
          v42 = *(vtkShaderProgram **)(a1 + 176);
          vtkShaderProgram::SetUniformi(v42, "layer", j);
          vtkOpenGLRenderUtilities::DrawFullScreenQuad();
        }
        vtkOpenGLVertexArrayObject::Release(*(vtkOpenGLVertexArrayObject **)(a1 + 184));
        vtkOpenGLFramebufferObject::RestorePreviousBindingsAndBuffers(*(vtkOpenGLFramebufferObject **)(a1 + 152));
      }
    }
  }
}
// 18002190E: variable 'v4' is possibly undefined
// 180021975: variable 'v5' is possibly undefined
// 1800219DC: variable 'v6' is possibly undefined
// 180021C84: variable 'v7' is possibly undefined
// 180021CE7: variable 'v8' is possibly undefined
// 180001109: using guessed type __int64 __fastcall sub_180001109(_QWORD);
// 180001230: using guessed type __int64 __fastcall sub_180001230(_QWORD, _QWORD);
// 1800012CB: using guessed type __int64 __fastcall sub_1800012CB(_QWORD);
// 1800016BD: using guessed type __int64 __fastcall sub_1800016BD(_QWORD, _QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001C08: using guessed type double __fastcall sub_180001C08(_QWORD, _QWORD);
// 180001F0A: using guessed type __int64 __fastcall sub_180001F0A(_DWORD, _DWORD, _DWORD, _DWORD, __int64, __int64);
// 18000236F: using guessed type __int64 __fastcall sub_18000236F(_QWORD);
// 1800026FD: using guessed type __int64 sub_1800026FD(_QWORD, const char *, ...);
// 1800742A8: using guessed type struct vtkRenderer *vtkRenderState::GetRenderer(vtkRenderState *__hidden this);
// 1800742E0: using guessed type struct vtkRenderWindow *vtkRenderer::GetRenderWindow(vtkRenderer *__hidden this);
// 1800745E0: using guessed type __int64 __fastcall vtkShaderProgram::Substitute(_QWORD, _QWORD, _QWORD, _QWORD);
// 180074618: using guessed type __m128 vtkShaderProgram::SetUniformf(vtkShaderProgram *__hidden this, const char *, float);
// 180074620: using guessed type bool vtkShaderProgram::SetUniformi(vtkShaderProgram *__hidden this, const char *, _DWORD);
// 180074630: using guessed type void vtkOpenGLVertexArrayObject::Release(vtkOpenGLVertexArrayObject *__hidden this);
// 180074638: using guessed type void vtkOpenGLVertexArrayObject::Bind(vtkOpenGLVertexArrayObject *__hidden this);
// 180074640: using guessed type __int64 vtkOpenGLVertexArrayObject::New(void);
// 180074648: using guessed type __int64 vtkOpenGLRenderUtilities::DrawFullScreenQuad(void);
// 180074658: using guessed type __int64 __fastcall vtkOpenGLRenderUtilities::GetFullScreenQuadFragmentShaderTemplate(_QWORD);
// 180074678: using guessed type __int64 vtkOpenGLBufferObject::New(void);
// 180074688: using guessed type void vtkOpenGLFramebufferObject::RestorePreviousBindingsAndBuffers(vtkOpenGLFramebufferObject *__hidden this);
// 180074690: using guessed type void vtkOpenGLFramebufferObject::SaveCurrentBindingsAndBuffers(vtkOpenGLFramebufferObject *__hidden this);
// 180074698: using guessed type void vtkOpenGLFramebufferObject::Bind(vtkOpenGLFramebufferObject *__hidden this);
// 180074790: using guessed type __int64 (__fastcall *_glewBindImageTexture)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD, _DWORD, _DWORD);

//----- (0000000180021DC0) ----------------------------------------------------
// Hidden C++ exception states: #wind=15
__int64 __fastcall sub_180021DC0(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  __int64 v4; // rax
  __int64 v5; // rax
  vtkRenderer *Renderer; // rax
  __int64 v7; // r9
  __int64 v8; // r9
  __int64 v9; // r9
  __int64 v10; // r9
  __int64 v11; // rax
  unsigned __int64 v12; // rax
  vtkMatrix4x4 *v13; // rax
  vtkMatrix4x4 *v14; // rax
  __int64 v15; // [rsp+0h] [rbp-608h] BYREF
  _BYTE v16[64]; // [rsp+48h] [rbp-5C0h] BYREF
  _BYTE v17[40]; // [rsp+88h] [rbp-580h] BYREF
  _BYTE v18[248]; // [rsp+B0h] [rbp-558h] BYREF
  _BYTE v19[48]; // [rsp+1A8h] [rbp-460h] BYREF
  double v20[6]; // [rsp+1D8h] [rbp-430h] BYREF
  double v21[6]; // [rsp+208h] [rbp-400h] BYREF
  GLsizei width; // [rsp+238h] [rbp-3D0h] BYREF
  GLsizei v23; // [rsp+23Ch] [rbp-3CCh]
  int v24; // [rsp+240h] [rbp-3C8h]
  __int64 v25; // [rsp+268h] [rbp-3A0h] BYREF
  __int64 v26; // [rsp+288h] [rbp-380h] BYREF
  struct vtkRenderWindow *RenderWindow; // [rsp+298h] [rbp-370h]
  _BYTE v28[64]; // [rsp+2A8h] [rbp-360h] BYREF
  _BYTE v29[64]; // [rsp+2E8h] [rbp-320h] BYREF
  _BYTE v30[48]; // [rsp+328h] [rbp-2E0h] BYREF
  _BYTE v31[40]; // [rsp+358h] [rbp-2B0h] BYREF
  _BYTE v32[40]; // [rsp+380h] [rbp-288h] BYREF
  _BYTE v33[40]; // [rsp+3A8h] [rbp-260h] BYREF
  _BYTE v34[40]; // [rsp+3D0h] [rbp-238h] BYREF
  _BYTE v35[40]; // [rsp+3F8h] [rbp-210h] BYREF
  _BYTE v36[56]; // [rsp+420h] [rbp-1E8h] BYREF
  double v37[3]; // [rsp+458h] [rbp-1B0h] BYREF
  unsigned int v38; // [rsp+474h] [rbp-194h]
  int j; // [rsp+478h] [rbp-190h]
  const double *v40; // [rsp+480h] [rbp-188h]
  _BYTE v41[12]; // [rsp+498h] [rbp-170h] BYREF
  int k; // [rsp+4A4h] [rbp-164h]
  _BYTE v43[8]; // [rsp+4B0h] [rbp-158h] BYREF
  __int64 v44; // [rsp+4B8h] [rbp-150h]
  __int64 v45; // [rsp+4C0h] [rbp-148h]
  __int64 v46; // [rsp+4C8h] [rbp-140h]
  __int64 v47; // [rsp+4D0h] [rbp-138h]
  __int64 v48; // [rsp+4D8h] [rbp-130h]
  __int64 v49; // [rsp+4E0h] [rbp-128h]
  __int64 started; // [rsp+4E8h] [rbp-120h]
  __int64 v51; // [rsp+4F0h] [rbp-118h]
  __int64 v52; // [rsp+4F8h] [rbp-110h]
  __int64 (__fastcall *v53)(_QWORD, _QWORD, _QWORD); // [rsp+500h] [rbp-108h]
  __int64 (__fastcall *v54)(_QWORD, _QWORD); // [rsp+508h] [rbp-100h]
  void (__fastcall *v55)(__int64, _QWORD, __int64, __int64); // [rsp+510h] [rbp-F8h]
  __int64 v56; // [rsp+518h] [rbp-F0h]
  __int64 v57; // [rsp+520h] [rbp-E8h]
  __int64 (__fastcall *v58)(_QWORD, _QWORD); // [rsp+528h] [rbp-E0h]
  __int64 v59; // [rsp+530h] [rbp-D8h]
  __int64 (__fastcall *v60)(__int64, __int64, __int64, __int64, _QWORD); // [rsp+538h] [rbp-D0h]
  __int64 v61; // [rsp+540h] [rbp-C8h]
  __int64 v62; // [rsp+548h] [rbp-C0h]
  __int64 v63; // [rsp+550h] [rbp-B8h]
  __int64 v64; // [rsp+558h] [rbp-B0h]
  void (__fastcall *v65)(__int64, __int64, _QWORD); // [rsp+560h] [rbp-A8h]
  __int64 v66; // [rsp+568h] [rbp-A0h]
  vtkShaderProgram *v67; // [rsp+570h] [rbp-98h]
  vtkShaderProgram *v68; // [rsp+578h] [rbp-90h]
  vtkShaderProgram *v69; // [rsp+580h] [rbp-88h]
  __int64 (__fastcall *v70)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD, _DWORD, _DWORD); // [rsp+588h] [rbp-80h]
  vtkShaderProgram *v71; // [rsp+590h] [rbp-78h]
  int v72; // [rsp+598h] [rbp-70h]
  vtkShaderProgram *v73; // [rsp+5A0h] [rbp-68h]
  const double *v74; // [rsp+5A8h] [rbp-60h]
  vtkShaderProgram *v75; // [rsp+5B0h] [rbp-58h]
  int v76; // [rsp+5B8h] [rbp-50h]
  unsigned __int64 v77; // [rsp+5C0h] [rbp-48h]
  vtkMatrix4x4 *v78; // [rsp+5C8h] [rbp-40h]
  vtkShaderProgram *v79; // [rsp+5D0h] [rbp-38h]
  struct vtkMatrix4x4 *v80; // [rsp+5D8h] [rbp-30h]
  vtkShaderProgram *v81; // [rsp+5E0h] [rbp-28h]
  __int64 (__fastcall *v82)(_QWORD, _QWORD, _QWORD); // [rsp+5E8h] [rbp-20h]

  v1 = &v15;
  for ( i = 384; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  v44 = -2;
  sub_18000177B(v16);
  v45 = *(_QWORD *)(a1 + 88);
  sub_1800016EF(v45, v16, 10);
  if ( !sub_1800025C2(v16) )
    return sub_1800016F4(v16);
  vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v17);
  sub_18000139D(v18, 2, 1);
  sub_1800017EE(v18, "vtkGrindSimulatePass::UpdateToolGrid");
  v46 = *(_QWORD *)(a1 + 104);
  v47 = sub_180002270(v18, v19);
  v48 = v47;
  v49 = v47;
  started = vtkRenderTimerLog::StartScopedEvent(v46, v43, v47);
  v51 = started;
  vtkRenderTimerLog::ScopedEventLogger::operator=(v17, started);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v43);
  sub_180001B8B(v19);
  sub_1800023A6(v18);
  if ( (int)sub_1800012CB(*(_QWORD *)(a1 + 88)) < 0 )
    goto LABEL_7;
  v25 = 0;
  v26 = 0;
  v52 = *(_QWORD *)(a1 + 88);
  sub_180001F0A(v52, (unsigned int)&v25, (unsigned int)&v26, (unsigned int)v21, (__int64)v20, (__int64)&width);
  vtkOpenGLFramebufferObject::SaveCurrentBindingsAndBuffers(*(vtkOpenGLFramebufferObject **)(a1 + 152));
  vtkOpenGLFramebufferObject::Bind(*(vtkOpenGLFramebufferObject **)(a1 + 152));
  glViewport(0, 0, width, v23);
  if ( (unsigned int)sub_1800023F6(*(_QWORD *)(a1 + 88)) == 5 || (unsigned int)sub_1800023F6(*(_QWORD *)(a1 + 88)) == 6 )
  {
    v53 = _glewBindBufferBase;
    _glewBindBufferBase(37074, 0, *(unsigned int *)(a1 + 260));
    v54 = _glewBindBuffer;
    _glewBindBuffer(37074, *(unsigned int *)(a1 + 260));
    v55 = (void (__fastcall *)(__int64, _QWORD, __int64, __int64))_glewBufferSubData;
    v4 = sub_1800025F4(*(_QWORD *)(a1 + 88));
    v56 = sub_18000275C(v4);
    v5 = sub_1800025F4(*(_QWORD *)(a1 + 88));
    v57 = 4 * sub_1800027B1(v5);
    v55(37074, 0, v57, v56);
    v58 = _glewBindBuffer;
    _glewBindBuffer(37074, 0);
  }
  Renderer = vtkRenderState::GetRenderer(*(vtkRenderState **)(a1 + 112));
  RenderWindow = vtkRenderer::GetRenderWindow(Renderer);
  if ( *(_QWORD *)(a1 + 200) )
  {
    v64 = (*(__int64 (__fastcall **)(struct vtkRenderWindow *))(*(_QWORD *)RenderWindow + 1624LL))(RenderWindow);
    v65 = *(void (__fastcall **)(__int64, __int64, _QWORD))(*(_QWORD *)v64 + 144LL);
    v66 = *(_QWORD *)(a1 + 200);
    v65(v64, v66, 0);
  }
  else
  {
    sub_1800026FD(
      v28,
      "#version 430 core\n"
      "//YHQ UpdateTool fs\n"
      "layout(location = 0) in vec4 position;\n"
      "void main() {\n"
      "   gl_Position = vec4(0.0, 0.0, 0.0, 1.0);\n"
      "}");
    sub_1800026FD(
      v29,
      "#version 430 core\n"
      "//YHQ UpdateTool gs\n"
      "uniform int layer;\n"
      "layout(triangles) in;\n"
      "layout(triangle_strip, max_vertices = 6) out;\n"
      "void main() {\n"
      "   gl_Layer = layer;\n"
      "   gl_Position = vec4(-1.0, -1.0, 0.0, 1.0);\n"
      "   EmitVertex();\n"
      "   gl_Position = vec4(1.0, -1.0, 0.0, 1.0);\n"
      "   EmitVertex();\n"
      "   gl_Position = vec4(-1.0, 1.0, 0.0, 1.0);\n"
      "   EmitVertex();\n"
      "   EndPrimitive();\n"
      "   gl_Position = vec4(1.0, -1.0, 0.0, 1.0);\n"
      "   EmitVertex();\n"
      "   gl_Position = vec4(1.0, 1.0, 0.0, 1.0);\n"
      "   EmitVertex();\n"
      "   gl_Position = vec4(-1.0, 1.0, 0.0, 1.0);\n"
      "   EmitVertex();\n"
      "   EndPrimitive();\n"
      "}\n"
      "\n");
    vtkOpenGLRenderUtilities::GetFullScreenQuadFragmentShaderTemplate(v30);
    sub_1800026FD(v31, "#version 430 core\n//YHQ UpdateTool fs\n\n");
    sub_1800026FD(v32, "//VTK::System::Dec");
    LOBYTE(v7) = 1;
    vtkShaderProgram::Substitute(v30, v32, v31, v7);
    sub_180001B8B(v32);
    sub_180001B8B(v31);
    sub_1800026FD(
      v33,
      "layout(r32f, binding = 0) uniform image3D texDist;\n"
      "layout(std430, binding = 0) buffer PolygonBuffer\n"
      "{\n"
      "    vec2 PG_Points[];\n"
      "};\n"
      "uniform int PGP_Count;\n"
      "uniform vec3 gridOrigin;\n"
      "uniform vec3 gridSpacing;\n"
      "uniform vec3 gridSize;\n"
      "\n"
      "vec3 texCoordToworldPos(vec3 tc) {\n"
      "   vec3 voxelCoords = tc * gridSize;\n"
      "   vec3 worldPos = (voxelCoords - 0.5) * gridSpacing + gridOrigin;\n"
      "   return worldPos;\n"
      "}\n"
      "\n"
      "//tool parameters\n"
      "uniform int toolType;\n"
      "uniform mat4 toolMatrix;\n"
      "uniform vec3 toolSize;\n"
      "float sdSphere(in vec3 p) {\n"
      "    vec3 o = (toolMatrix * vec4(p, 1)).xyz;\n"
      "    return length(o.xyz) - toolSize.x;\n"
      "}\n"
      "float sdSemiSphere(in vec3 p) {\n"
      "    vec3 o = (toolMatrix * vec4(p, 1)).xyz;\n"
      "    float r = toolSize.x;\n"
      "    float d1 = length(o.xyz) - r;\n"
      "    float d2 = -o.z;\n"
      "    return max(d1, d2);\n"
      "}\n"
      "float sdBox(in vec3 p){\n"
      "    vec3 o = (toolMatrix * vec4(p, 1)).xyz;\n"
      "    vec3 b = toolSize;\n"
      "    vec3 q = abs(o.xyz) - b;\n"
      "    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);\n"
      "}\n"
      "float sdCylinder(in vec3 p) {\n"
      "    vec3 o = (toolMatrix * vec4(p, 1)).xyz;\n"
      "    float r = toolSize.x;\n"
      "    float h = toolSize.y;\n"
      "    vec2 d = abs(vec2(length(o.xy),o.z)) - vec2(r,h);\n"
      "    return min(max(d.x,d.y),0.0) + length(max(d,0.0));\n"
      "}\n"
      "float sdCapsule(in vec3 p) {\n"
      "   vec3 o = (toolMatrix * vec4(p, 1)).xyz;\n"
      "   float r = toolSize.x;\n"
      "   float h = toolSize.y;\n"
      "   o.z -= clamp(o.z, -h, h);\n"
      "   return length(o) - r;\n"
      "}\n"
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
      "}\n"
      "float sd2DAxisDilate(in vec3 p) {\n"
      "    vec3 o = (toolMatrix * vec4(p, 1)).xyz;\n"
      "    float height = toolSize.x;\n"
      "    int axis = int(floor(toolSize.y+0.5));\n"
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
      "float sdRound2DAxisDilate(in vec3 p) {\n"
      "    vec3 o = (toolMatrix * vec4(p, 1)).xyz;\n"
      "    float radius = toolSize.x;\n"
      "    float height = toolSize.y;\n"
      "    int axis = int(floor(toolSize.z+0.5));\n"
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
      "    return length(max(vec2(d2, dh), 0.0)) - radius + min(max(d2, dh), 0.0); \n"
      "}\n");
    sub_1800026FD(v34, "//VTK::FSQ::Decl");
    LOBYTE(v8) = 1;
    vtkShaderProgram::Substitute(v30, v34, v33, v8);
    sub_180001B8B(v34);
    sub_180001B8B(v33);
    sub_1800026FD(
      v35,
      "   vec3 toolTexCoord = vec3(gl_FragCoord.x / gridSize.x, gl_FragCoord.y / gridSize.y, (gl_Layer + 0.5) / gridSize."
      "z);\n"
      "   ivec3 voxelCoord = ivec3(gl_FragCoord.x, gl_FragCoord.y, gl_Layer);\n"
      "   float odist = imageLoad(texDist, voxelCoord).x;\n"
      "   vec3 worldPos = texCoordToworldPos(toolTexCoord);\n"
      "   float ndist = 0;\n"
      "   if (toolType == 0) {\n"
      "       ndist = sdSphere(worldPos);\n"
      "   } else if (toolType == 1) {\n"
      "       ndist = sdSemiSphere(worldPos);\n"
      "   } else if (toolType == 2) {\n"
      "       ndist = sdBox(worldPos);\n"
      "   } else if (toolType == 3) {\n"
      "       ndist = sdCylinder(worldPos);\n"
      "   } else if (toolType == 4) {\n"
      "       ndist = sdCapsule(worldPos);\n"
      "   } else if (toolType == 5) {\n"
      "       ndist = sd2DAxisDilate(worldPos);\n"
      "   } else if (toolType == 6) {\n"
      "       ndist = sdRound2DAxisDilate(worldPos);\n"
      "   } else {\n"
      "       discard;\n"
      "       return;\n"
      "   }\n"
      "   if (ndist < odist) {\n"
      "       imageStore(texDist, voxelCoord, vec4(ndist, 0, 0, 0));\n"
      "   }\n");
    sub_1800026FD(v36, "//VTK::FSQ::Impl");
    LOBYTE(v9) = 1;
    vtkShaderProgram::Substitute(v30, v36, v35, v9);
    sub_180001B8B(v36);
    sub_180001B8B(v35);
    v59 = (*(__int64 (__fastcall **)(struct vtkRenderWindow *))(*(_QWORD *)RenderWindow + 1624LL))(RenderWindow);
    v60 = *(__int64 (__fastcall **)(__int64, __int64, __int64, __int64, _QWORD))(*(_QWORD *)v59 + 160LL);
    v61 = sub_18000236F(v29);
    v62 = sub_18000236F(v30);
    v63 = sub_18000236F(v28);
    *(_QWORD *)(a1 + 200) = v60(v59, v63, v62, v61, 0);
    sub_180001B8B(v30);
    sub_180001B8B(v29);
    sub_180001B8B(v28);
  }
  if ( *(_QWORD *)(a1 + 200) )
  {
    if ( !*(_QWORD *)(a1 + 208) )
    {
      *(_QWORD *)(a1 + 216) = vtkOpenGLBufferObject::New();
      *(_QWORD *)(a1 + 208) = vtkOpenGLVertexArrayObject::New();
      sub_1800016BD(*(_QWORD *)(a1 + 216), *(_QWORD *)(a1 + 208), *(_QWORD *)(a1 + 200));
    }
    vtkOpenGLVertexArrayObject::Bind(*(vtkOpenGLVertexArrayObject **)(a1 + 208));
    sub_180001C08(a1, 1);
    v67 = *(vtkShaderProgram **)(a1 + 200);
    vtkShaderProgram::SetUniform3f(v67, "gridOrigin", v21);
    v68 = *(vtkShaderProgram **)(a1 + 200);
    vtkShaderProgram::SetUniform3f(v68, "gridSpacing", v20);
    v37[0] = (double)width;
    v37[1] = (double)v23;
    v37[2] = (double)v24;
    v69 = *(vtkShaderProgram **)(a1 + 200);
    vtkShaderProgram::SetUniform3f(v69, "gridSize", v37);
    v38 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)v26 + 208LL))(v26);
    v70 = _glewBindImageTexture;
    LOBYTE(v10) = 1;
    _glewBindImageTexture(0, v38, 0, v10, 0, 35002, 33326);
    v71 = *(vtkShaderProgram **)(a1 + 200);
    v72 = sub_1800023F6(*(_QWORD *)(a1 + 88));
    vtkShaderProgram::SetUniformi(v71, "toolType", v72);
    v73 = *(vtkShaderProgram **)(a1 + 200);
    v74 = (const double *)sub_180001F19(*(_QWORD *)(a1 + 88));
    vtkShaderProgram::SetUniform3f(v73, "toolSize", v74);
    if ( (unsigned int)sub_1800023F6(*(_QWORD *)(a1 + 88)) == 5
      || (unsigned int)sub_1800023F6(*(_QWORD *)(a1 + 88)) == 6 )
    {
      v75 = *(vtkShaderProgram **)(a1 + 200);
      v11 = sub_1800025F4(*(_QWORD *)(a1 + 88));
      v76 = (int)sub_1800027B1(v11) / 2;
      vtkShaderProgram::SetUniformi(v75, "PGP_Count", v76);
    }
    for ( j = 0; ; ++j )
    {
      v77 = j;
      v12 = sub_1800025C2(v16);
      if ( v77 >= v12 )
        break;
      v40 = (const double *)sub_1800011EF(v16, j);
      sub_180001A8C(v41);
      v78 = (vtkMatrix4x4 *)sub_180002766(v41);
      vtkMatrix4x4::DeepCopy(v78, v40);
      v13 = (vtkMatrix4x4 *)sub_180002766(v41);
      vtkMatrix4x4::Invert(v13);
      v14 = (vtkMatrix4x4 *)sub_180002766(v41);
      vtkMatrix4x4::Transpose(v14);
      v79 = *(vtkShaderProgram **)(a1 + 200);
      v80 = (struct vtkMatrix4x4 *)sub_180001852(v41);
      vtkShaderProgram::SetUniformMatrix(v79, "toolMatrix", v80);
      for ( k = 0; k < v24; ++k )
      {
        v81 = *(vtkShaderProgram **)(a1 + 200);
        vtkShaderProgram::SetUniformi(v81, "layer", k);
        vtkOpenGLRenderUtilities::DrawFullScreenQuad();
      }
      sub_180001F32(v41);
    }
    vtkOpenGLVertexArrayObject::Release(*(vtkOpenGLVertexArrayObject **)(a1 + 208));
    if ( (unsigned int)sub_1800023F6(*(_QWORD *)(a1 + 88)) == 5
      || (unsigned int)sub_1800023F6(*(_QWORD *)(a1 + 88)) == 6 )
    {
      v82 = _glewBindBufferBase;
      _glewBindBufferBase(37074, 0, 0);
    }
    vtkOpenGLFramebufferObject::RestorePreviousBindingsAndBuffers(*(vtkOpenGLFramebufferObject **)(a1 + 152));
    vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v17);
    return sub_1800016F4(v16);
  }
  else
  {
LABEL_7:
    vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v17);
    return sub_1800016F4(v16);
  }
}
// 180022231: variable 'v7' is possibly undefined
// 180022298: variable 'v8' is possibly undefined
// 1800222FF: variable 'v9' is possibly undefined
// 18002267C: variable 'v10' is possibly undefined
// 1800011EF: using guessed type __int64 __fastcall sub_1800011EF(_QWORD, _QWORD);
// 1800012CB: using guessed type __int64 __fastcall sub_1800012CB(_QWORD);
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 1800016BD: using guessed type __int64 __fastcall sub_1800016BD(_QWORD, _QWORD, _QWORD);
// 1800016EF: using guessed type __int64 __fastcall sub_1800016EF(_QWORD, _QWORD, _QWORD);
// 1800016F4: using guessed type __int64 __fastcall sub_1800016F4(_QWORD);
// 18000177B: using guessed type __int64 __fastcall sub_18000177B(_QWORD);
// 1800017EE: using guessed type __int64 __fastcall sub_1800017EE(_QWORD, _QWORD);
// 180001852: using guessed type __int64 __fastcall sub_180001852(_QWORD);
// 180001A8C: using guessed type __int64 __fastcall sub_180001A8C(_QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001C08: using guessed type double __fastcall sub_180001C08(_QWORD, _QWORD);
// 180001F0A: using guessed type __int64 __fastcall sub_180001F0A(_DWORD, _DWORD, _DWORD, _DWORD, __int64, __int64);
// 180001F19: using guessed type __int64 __fastcall sub_180001F19(_QWORD);
// 180001F32: using guessed type __int64 __fastcall sub_180001F32(_QWORD);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 18000236F: using guessed type __int64 __fastcall sub_18000236F(_QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 1800023F6: using guessed type __int64 __fastcall sub_1800023F6(_QWORD);
// 1800025C2: using guessed type __int64 __fastcall sub_1800025C2(_QWORD);
// 1800025F4: using guessed type __int64 __fastcall sub_1800025F4(_QWORD);
// 1800026FD: using guessed type __int64 sub_1800026FD(_QWORD, const char *, ...);
// 18000275C: using guessed type __int64 __fastcall sub_18000275C(_QWORD);
// 180002766: using guessed type __int64 __fastcall sub_180002766(_QWORD);
// 1800027B1: using guessed type __int64 __fastcall sub_1800027B1(_QWORD);
// 180073AC8: using guessed type void vtkMatrix4x4::Transpose(vtkMatrix4x4 *__hidden this);
// 180073AD0: using guessed type void vtkMatrix4x4::Invert(vtkMatrix4x4 *__hidden this);
// 180073AE8: using guessed type void vtkMatrix4x4::DeepCopy(vtkMatrix4x4 *__hidden this, const double *const);
// 180074260: using guessed type __int64 __fastcall vtkRenderTimerLog::StartScopedEvent(_QWORD, _QWORD, _QWORD);
// 180074268: using guessed type void vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 180074270: using guessed type __int64 __fastcall vtkRenderTimerLog::ScopedEventLogger::operator=(_QWORD, _QWORD);
// 180074278: using guessed type _QWORD vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 1800742A8: using guessed type struct vtkRenderer *vtkRenderState::GetRenderer(vtkRenderState *__hidden this);
// 1800742E0: using guessed type struct vtkRenderWindow *vtkRenderer::GetRenderWindow(vtkRenderer *__hidden this);
// 1800745E0: using guessed type __int64 __fastcall vtkShaderProgram::Substitute(_QWORD, _QWORD, _QWORD, _QWORD);
// 1800745E8: using guessed type bool vtkShaderProgram::SetUniformMatrix(vtkShaderProgram *__hidden this, const char *, struct vtkMatrix4x4 *);
// 180074600: using guessed type bool vtkShaderProgram::SetUniform3f(vtkShaderProgram *__hidden this, const char *, const double *const);
// 180074620: using guessed type bool vtkShaderProgram::SetUniformi(vtkShaderProgram *__hidden this, const char *, _DWORD);
// 180074630: using guessed type void vtkOpenGLVertexArrayObject::Release(vtkOpenGLVertexArrayObject *__hidden this);
// 180074638: using guessed type void vtkOpenGLVertexArrayObject::Bind(vtkOpenGLVertexArrayObject *__hidden this);
// 180074640: using guessed type __int64 vtkOpenGLVertexArrayObject::New(void);
// 180074648: using guessed type __int64 vtkOpenGLRenderUtilities::DrawFullScreenQuad(void);
// 180074658: using guessed type __int64 __fastcall vtkOpenGLRenderUtilities::GetFullScreenQuadFragmentShaderTemplate(_QWORD);
// 180074678: using guessed type __int64 vtkOpenGLBufferObject::New(void);
// 180074688: using guessed type void vtkOpenGLFramebufferObject::RestorePreviousBindingsAndBuffers(vtkOpenGLFramebufferObject *__hidden this);
// 180074690: using guessed type void vtkOpenGLFramebufferObject::SaveCurrentBindingsAndBuffers(vtkOpenGLFramebufferObject *__hidden this);
// 180074698: using guessed type void vtkOpenGLFramebufferObject::Bind(vtkOpenGLFramebufferObject *__hidden this);
// 180074770: using guessed type __int64 (__fastcall *_glewBufferSubData)(_QWORD, _QWORD, _QWORD, _QWORD);
// 180074788: using guessed type __int64 (__fastcall *_glewBindBuffer)(_QWORD, _QWORD);
// 180074790: using guessed type __int64 (__fastcall *_glewBindImageTexture)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD, _DWORD, _DWORD);
// 1800747A0: using guessed type __int64 (__fastcall *_glewBindBufferBase)(_QWORD, _QWORD, _QWORD);

//----- (0000000180022A00) ----------------------------------------------------
// Hidden C++ exception states: #wind=13
void __fastcall sub_180022A00(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  __int64 v3; // rax
  __int64 v4; // rax
  vtkRenderer *Renderer; // rax
  __int64 v6; // r9
  __int64 v7; // r9
  __int64 v8; // r9
  vtkRenderer *v9; // rax
  struct vtkCamera *ActiveCamera; // rax
  __int64 v11; // [rsp+0h] [rbp-678h] BYREF
  _BYTE v12[40]; // [rsp+48h] [rbp-630h] BYREF
  _BYTE v13[248]; // [rsp+70h] [rbp-608h] BYREF
  _BYTE v14[48]; // [rsp+168h] [rbp-510h] BYREF
  double v15[6]; // [rsp+198h] [rbp-4E0h] BYREF
  double v16[6]; // [rsp+1C8h] [rbp-4B0h] BYREF
  _DWORD v17[12]; // [rsp+1F8h] [rbp-480h] BYREF
  vtkTextureObject *v18; // [rsp+228h] [rbp-450h] BYREF
  vtkTextureObject *v19; // [rsp+248h] [rbp-430h] BYREF
  _DWORD v20[8]; // [rsp+264h] [rbp-414h] BYREF
  _DWORD v21[5]; // [rsp+284h] [rbp-3F4h] BYREF
  struct vtkRenderWindow *RenderWindow; // [rsp+298h] [rbp-3E0h]
  _BYTE v23[48]; // [rsp+2A8h] [rbp-3D0h] BYREF
  _BYTE v24[40]; // [rsp+2D8h] [rbp-3A0h] BYREF
  _BYTE v25[40]; // [rsp+300h] [rbp-378h] BYREF
  _BYTE v26[40]; // [rsp+328h] [rbp-350h] BYREF
  _BYTE v27[40]; // [rsp+350h] [rbp-328h] BYREF
  _BYTE v28[40]; // [rsp+378h] [rbp-300h] BYREF
  _BYTE v29[40]; // [rsp+3A0h] [rbp-2D8h] BYREF
  _BYTE v30[40]; // [rsp+3C8h] [rbp-2B0h] BYREF
  _BYTE v31[40]; // [rsp+3F0h] [rbp-288h] BYREF
  __int64 v32; // [rsp+418h] [rbp-260h]
  _BYTE v33[32]; // [rsp+428h] [rbp-250h] BYREF
  struct vtkMatrix4x4 *v34; // [rsp+448h] [rbp-230h] BYREF
  struct vtkMatrix3x3 *v35; // [rsp+468h] [rbp-210h] BYREF
  _BYTE v36[32]; // [rsp+488h] [rbp-1F0h] BYREF
  double v37[6]; // [rsp+4A8h] [rbp-1D0h] BYREF
  int v38[6]; // [rsp+4D8h] [rbp-1A0h] BYREF
  _BYTE v39[8]; // [rsp+4F0h] [rbp-188h] BYREF
  __int64 v40; // [rsp+4F8h] [rbp-180h]
  __int64 v41; // [rsp+500h] [rbp-178h]
  __int64 v42; // [rsp+508h] [rbp-170h]
  __int64 v43; // [rsp+510h] [rbp-168h]
  __int64 v44; // [rsp+518h] [rbp-160h]
  __int64 started; // [rsp+520h] [rbp-158h]
  __int64 v46; // [rsp+528h] [rbp-150h]
  __int64 v47; // [rsp+530h] [rbp-148h]
  __int64 (__fastcall *v48)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD, _DWORD, _DWORD); // [rsp+538h] [rbp-140h]
  __int64 (__fastcall *v49)(_QWORD, _QWORD, _QWORD); // [rsp+540h] [rbp-138h]
  __int64 (__fastcall *v50)(_QWORD, _QWORD, _QWORD); // [rsp+548h] [rbp-130h]
  __int64 (__fastcall *v51)(_QWORD, _QWORD); // [rsp+550h] [rbp-128h]
  void (__fastcall *v52)(__int64, _QWORD, __int64, __int64); // [rsp+558h] [rbp-120h]
  __int64 v53; // [rsp+560h] [rbp-118h]
  __int64 v54; // [rsp+568h] [rbp-110h]
  __int64 (__fastcall *v55)(_QWORD, _QWORD); // [rsp+570h] [rbp-108h]
  __int64 v56; // [rsp+578h] [rbp-100h]
  __int64 (__fastcall *v57)(__int64, __int64, __int64, __int64, _QWORD); // [rsp+580h] [rbp-F8h]
  __int64 FullScreenQuadGeometryShader; // [rsp+588h] [rbp-F0h]
  __int64 v59; // [rsp+590h] [rbp-E8h]
  __int64 v60; // [rsp+598h] [rbp-E0h]
  __int64 v61; // [rsp+5A0h] [rbp-D8h]
  __int64 FullScreenQuadVertexShader; // [rsp+5A8h] [rbp-D0h]
  __int64 v63; // [rsp+5B0h] [rbp-C8h]
  __int64 v64; // [rsp+5B8h] [rbp-C0h]
  void (__fastcall *v65)(__int64, struct vtkRenderer *, struct vtkMatrix4x4 **, struct vtkMatrix3x3 **, _BYTE *, _BYTE *); // [rsp+5C0h] [rbp-B8h]
  struct vtkRenderer *v66; // [rsp+5C8h] [rbp-B0h]
  vtkShaderProgram *v67; // [rsp+5D0h] [rbp-A8h]
  vtkShaderProgram *v68; // [rsp+5D8h] [rbp-A0h]
  vtkShaderProgram *v69; // [rsp+5E0h] [rbp-98h]
  int v70; // [rsp+5E8h] [rbp-90h]
  vtkShaderProgram *v71; // [rsp+5F0h] [rbp-88h]
  int TextureUnit; // [rsp+5F8h] [rbp-80h]
  vtkShaderProgram *v73; // [rsp+600h] [rbp-78h]
  int v74; // [rsp+608h] [rbp-70h]
  vtkShaderProgram *v75; // [rsp+610h] [rbp-68h]
  int v76; // [rsp+618h] [rbp-60h]
  vtkShaderProgram *v77; // [rsp+620h] [rbp-58h]
  vtkShaderProgram *v78; // [rsp+628h] [rbp-50h]
  vtkShaderProgram *v79; // [rsp+630h] [rbp-48h]
  vtkShaderProgram *v80; // [rsp+638h] [rbp-40h]
  vtkShaderProgram *v81; // [rsp+640h] [rbp-38h]
  __int64 (__fastcall *v82)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD, _DWORD, _DWORD); // [rsp+648h] [rbp-30h]
  __int64 (__fastcall *v83)(_QWORD, _QWORD, _QWORD); // [rsp+650h] [rbp-28h]
  __int64 (__fastcall *v84)(_QWORD, _QWORD, _QWORD); // [rsp+658h] [rbp-20h]

  v1 = &v11;
  for ( i = 412; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  v40 = -2;
  vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v12);
  sub_18000139D(v13, 2, 1);
  sub_1800017EE(v13, "vtkGrindSimulatePass::CompositeSDF");
  v41 = *(_QWORD *)(a1 + 104);
  v42 = sub_180002270(v13, v14);
  v43 = v42;
  v44 = v42;
  started = vtkRenderTimerLog::StartScopedEvent(v41, v39, v42);
  v46 = started;
  vtkRenderTimerLog::ScopedEventLogger::operator=(v12, started);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v39);
  sub_180001B8B(v14);
  sub_1800023A6(v13);
  if ( (int)sub_1800012CB(*(_QWORD *)(a1 + 88)) >= 0 )
  {
    v18 = 0;
    v19 = 0;
    v47 = *(_QWORD *)(a1 + 88);
    sub_180001F0A(v47, (unsigned int)&v18, (unsigned int)&v19, (unsigned int)v16, (__int64)v15, (__int64)v17);
    glViewport(0, 0, *(_DWORD *)(a1 + 96), *(_DWORD *)(a1 + 100));
    sub_1800016B3(a1, v20, v21);
    v48 = _glewBindImageTexture;
    _glewBindImageTexture(0, *(unsigned int *)(a1 + 160), 0, 0, 0, 35000, 33334);
    v49 = _glewBindBufferBase;
    _glewBindBufferBase(37074, 0, *(unsigned int *)(a1 + 164));
    if ( !(unsigned int)sub_1800018AC(*(_QWORD *)(a1 + 80))
      && ((unsigned int)sub_180001424(*(_QWORD *)(a1 + 80)) == 5
       || (unsigned int)sub_180001424(*(_QWORD *)(a1 + 80)) == 6) )
    {
      v50 = _glewBindBufferBase;
      _glewBindBufferBase(37074, 1, *(unsigned int *)(a1 + 256));
      v51 = _glewBindBuffer;
      _glewBindBuffer(37074, *(unsigned int *)(a1 + 256));
      v52 = (void (__fastcall *)(__int64, _QWORD, __int64, __int64))_glewBufferSubData;
      v3 = sub_18000257C(*(_QWORD *)(a1 + 80));
      v53 = sub_18000275C(v3);
      v4 = sub_18000257C(*(_QWORD *)(a1 + 80));
      v54 = 4 * sub_1800027B1(v4);
      v52(37074, 0, v54, v53);
      v55 = _glewBindBuffer;
      _glewBindBuffer(37074, 0);
    }
    Renderer = vtkRenderState::GetRenderer(*(vtkRenderState **)(a1 + 112));
    RenderWindow = vtkRenderer::GetRenderWindow(Renderer);
    vtkOpenGLRenderUtilities::GetFullScreenQuadFragmentShaderTemplate(v23);
    sub_1800026FD(v24, "#version 430 core\n//YHQ Composite fs\n");
    sub_1800026FD(v25, "//VTK::System::Dec");
    LOBYTE(v6) = 1;
    vtkShaderProgram::Substitute(v23, v25, v24, v6);
    sub_180001B8B(v25);
    sub_180001B8B(v24);
    sub_1800026FD(v26, aStructAbuffern);
    sub_1800026FD(v27, "//VTK::FSQ::Decl");
    LOBYTE(v7) = 1;
    vtkShaderProgram::Substitute(v23, v27, v26, v7);
    sub_180001B8B(v27);
    sub_180001B8B(v26);
    sub_1800026FD(v28, aIvec2CoordIvec_0);
    sub_1800026FD(v29, "//VTK::FSQ::Impl");
    LOBYTE(v8) = 1;
    vtkShaderProgram::Substitute(v23, v29, v28, v8);
    sub_180001B8B(v29);
    sub_180001B8B(v28);
    sub_18000209F(a1, v23);
    sub_180001ED8(a1, v23);
    sub_1800022D4(a1, v20[0], v21[0], v23);
    v56 = (*(__int64 (__fastcall **)(struct vtkRenderWindow *))(*(_QWORD *)RenderWindow + 1624LL))(RenderWindow);
    v57 = *(__int64 (__fastcall **)(__int64, __int64, __int64, __int64, _QWORD))(*(_QWORD *)v56 + 160LL);
    FullScreenQuadGeometryShader = vtkOpenGLRenderUtilities::GetFullScreenQuadGeometryShader(v31);
    v59 = FullScreenQuadGeometryShader;
    v60 = sub_18000236F(FullScreenQuadGeometryShader);
    v61 = sub_18000236F(v23);
    FullScreenQuadVertexShader = vtkOpenGLRenderUtilities::GetFullScreenQuadVertexShader(v30);
    v63 = FullScreenQuadVertexShader;
    v64 = sub_18000236F(FullScreenQuadVertexShader);
    *(_QWORD *)(a1 + 224) = v57(v56, v64, v61, v60, 0);
    sub_180001B8B(v30);
    sub_180001B8B(v31);
    if ( *(_QWORD *)(a1 + 224) )
    {
      if ( !*(_QWORD *)(a1 + 232) )
      {
        *(_QWORD *)(a1 + 240) = vtkOpenGLBufferObject::New();
        *(_QWORD *)(a1 + 232) = vtkOpenGLVertexArrayObject::New();
        sub_1800016BD(*(_QWORD *)(a1 + 240), *(_QWORD *)(a1 + 232), *(_QWORD *)(a1 + 224));
      }
      vtkTextureObject::Activate(v18);
      vtkTextureObject::Activate(v19);
      vtkTextureObject::Activate(*(vtkTextureObject **)(a1 + 248));
      v9 = vtkRenderState::GetRenderer(*(vtkRenderState **)(a1 + 112));
      ActiveCamera = vtkRenderer::GetActiveCamera(v9);
      v32 = _RTDynamicCast(
              ActiveCamera,
              0,
              &vtkCamera `RTTI Type Descriptor',
              &vtkOpenGLCamera `RTTI Type Descriptor',
              0);
      sub_1800015E6(a1);
      sub_180002351(a1);
      sub_1800014DD(a1, v20[0]);
      v65 = *(void (__fastcall **)(__int64, struct vtkRenderer *, struct vtkMatrix4x4 **, struct vtkMatrix3x3 **, _BYTE *, _BYTE *))(*(_QWORD *)v32 + 848LL);
      v66 = vtkRenderState::GetRenderer(*(vtkRenderState **)(a1 + 112));
      v65(v32, v66, &v34, &v35, v36, v33);
      v67 = *(vtkShaderProgram **)(a1 + 224);
      vtkShaderProgram::SetUniformMatrix(v67, "wcvcMatrix", v34);
      v68 = *(vtkShaderProgram **)(a1 + 224);
      vtkShaderProgram::SetUniformMatrix(v68, "normalMatrix", v35);
      v69 = *(vtkShaderProgram **)(a1 + 224);
      v70 = (*(__int64 (__fastcall **)(__int64))(*(_QWORD *)v32 + 248LL))(v32);
      vtkShaderProgram::SetUniformi(v69, "cameraParallel", v70);
      v71 = *(vtkShaderProgram **)(a1 + 224);
      TextureUnit = vtkTextureObject::GetTextureUnit(*(vtkTextureObject **)(a1 + 248));
      vtkShaderProgram::SetUniformi(v71, "opaqueDepthTex", TextureUnit);
      v73 = *(vtkShaderProgram **)(a1 + 224);
      v74 = vtkTextureObject::GetTextureUnit(v18);
      vtkShaderProgram::SetUniformi(v73, "toolOldTex", v74);
      v75 = *(vtkShaderProgram **)(a1 + 224);
      v76 = vtkTextureObject::GetTextureUnit(v19);
      vtkShaderProgram::SetUniformi(v75, "toolNowTex", v76);
      v77 = *(vtkShaderProgram **)(a1 + 224);
      vtkShaderProgram::SetUniform3f(v77, "gridOrigin", v16);
      v78 = *(vtkShaderProgram **)(a1 + 224);
      vtkShaderProgram::SetUniform3f(v78, "gridSpacing", v15);
      v37[0] = (double)v17[0];
      v37[1] = (double)v17[1];
      v37[2] = (double)v17[2];
      v79 = *(vtkShaderProgram **)(a1 + 224);
      vtkShaderProgram::SetUniform3f(v79, "gridSize", v37);
      v38[0] = *(_DWORD *)(a1 + 96);
      v38[1] = *(_DWORD *)(a1 + 100);
      v80 = *(vtkShaderProgram **)(a1 + 224);
      vtkShaderProgram::SetUniform2i(v80, "windowSize", v38);
      v81 = *(vtkShaderProgram **)(a1 + 224);
      vtkShaderProgram::SetUniformi(v81, "maxLayers", 16);
      vtkOpenGLVertexArrayObject::Bind(*(vtkOpenGLVertexArrayObject **)(a1 + 232));
      sub_180001C08(a1, 3);
      vtkOpenGLRenderUtilities::DrawFullScreenQuad();
      vtkOpenGLVertexArrayObject::Release(*(vtkOpenGLVertexArrayObject **)(a1 + 232));
      vtkTextureObject::Deactivate(v18);
      vtkTextureObject::Deactivate(v19);
      vtkTextureObject::Deactivate(*(vtkTextureObject **)(a1 + 248));
      v82 = _glewBindImageTexture;
      _glewBindImageTexture(0, 0, 0, 0, 0, 35000, 33334);
      v83 = _glewBindBufferBase;
      _glewBindBufferBase(37074, 0, 0);
      if ( !(unsigned int)sub_1800018AC(*(_QWORD *)(a1 + 80))
        && ((unsigned int)sub_180001424(*(_QWORD *)(a1 + 80)) == 5
         || (unsigned int)sub_180001424(*(_QWORD *)(a1 + 80)) == 6) )
      {
        v84 = _glewBindBufferBase;
        _glewBindBufferBase(37074, 1, 0);
      }
    }
    sub_180001B8B(v23);
    vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v12);
  }
  else
  {
    vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v12);
  }
}
// 180022E37: variable 'v6' is possibly undefined
// 180022E9E: variable 'v7' is possibly undefined
// 180022F05: variable 'v8' is possibly undefined
// 1800012CB: using guessed type __int64 __fastcall sub_1800012CB(_QWORD);
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 1800014DD: using guessed type __int64 __fastcall sub_1800014DD(_QWORD, _QWORD);
// 1800015E6: using guessed type __int64 __fastcall sub_1800015E6(_QWORD);
// 1800016B3: using guessed type __int64 __fastcall sub_1800016B3(_QWORD, _QWORD, _QWORD);
// 1800016BD: using guessed type __int64 __fastcall sub_1800016BD(_QWORD, _QWORD, _QWORD);
// 1800017EE: using guessed type __int64 __fastcall sub_1800017EE(_QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001C08: using guessed type double __fastcall sub_180001C08(_QWORD, _QWORD);
// 180001ED8: using guessed type __int64 __fastcall sub_180001ED8(_QWORD, _QWORD);
// 180001F0A: using guessed type __int64 __fastcall sub_180001F0A(_DWORD, _DWORD, _DWORD, _DWORD, __int64, __int64);
// 18000209F: using guessed type __int64 __fastcall sub_18000209F(_QWORD, _QWORD);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 1800022D4: using guessed type __int64 __fastcall sub_1800022D4(_QWORD, _QWORD, _QWORD, _QWORD);
// 180002351: using guessed type __int64 __fastcall sub_180002351(_QWORD);
// 18000236F: using guessed type __int64 __fastcall sub_18000236F(_QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 1800026FD: using guessed type __int64 sub_1800026FD(_QWORD, const char *, ...);
// 18000275C: using guessed type __int64 __fastcall sub_18000275C(_QWORD);
// 1800027B1: using guessed type __int64 __fastcall sub_1800027B1(_QWORD);
// 180040B7E: using guessed type __int64 __fastcall _RTDynamicCast(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD);
// 180074260: using guessed type __int64 __fastcall vtkRenderTimerLog::StartScopedEvent(_QWORD, _QWORD, _QWORD);
// 180074268: using guessed type void vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 180074270: using guessed type __int64 __fastcall vtkRenderTimerLog::ScopedEventLogger::operator=(_QWORD, _QWORD);
// 180074278: using guessed type _QWORD vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 1800742A8: using guessed type struct vtkRenderer *vtkRenderState::GetRenderer(vtkRenderState *__hidden this);
// 1800742B0: using guessed type struct vtkCamera *vtkRenderer::GetActiveCamera(vtkRenderer *__hidden this);
// 1800742E0: using guessed type struct vtkRenderWindow *vtkRenderer::GetRenderWindow(vtkRenderer *__hidden this);
// 1800745C0: using guessed type void vtkTextureObject::Deactivate(vtkTextureObject *__hidden this);
// 1800745C8: using guessed type void vtkTextureObject::Activate(vtkTextureObject *__hidden this);
// 1800745D0: using guessed type _DWORD vtkTextureObject::GetTextureUnit(vtkTextureObject *__hidden this);
// 1800745E0: using guessed type __int64 __fastcall vtkShaderProgram::Substitute(_QWORD, _QWORD, _QWORD, _QWORD);
// 1800745E8: using guessed type bool vtkShaderProgram::SetUniformMatrix(vtkShaderProgram *__hidden this, const char *, struct vtkMatrix4x4 *);
// 1800745F0: using guessed type bool vtkShaderProgram::SetUniformMatrix(vtkShaderProgram *__hidden this, const char *, struct vtkMatrix3x3 *);
// 180074600: using guessed type bool vtkShaderProgram::SetUniform3f(vtkShaderProgram *__hidden this, const char *, const double *const);
// 180074610: using guessed type bool vtkShaderProgram::SetUniform2i(vtkShaderProgram *__hidden this, const char *, const int *const);
// 180074620: using guessed type bool vtkShaderProgram::SetUniformi(vtkShaderProgram *__hidden this, const char *, _DWORD);
// 180074630: using guessed type void vtkOpenGLVertexArrayObject::Release(vtkOpenGLVertexArrayObject *__hidden this);
// 180074638: using guessed type void vtkOpenGLVertexArrayObject::Bind(vtkOpenGLVertexArrayObject *__hidden this);
// 180074640: using guessed type __int64 vtkOpenGLVertexArrayObject::New(void);
// 180074648: using guessed type __int64 vtkOpenGLRenderUtilities::DrawFullScreenQuad(void);
// 180074650: using guessed type __int64 __fastcall vtkOpenGLRenderUtilities::GetFullScreenQuadGeometryShader(_QWORD);
// 180074658: using guessed type __int64 __fastcall vtkOpenGLRenderUtilities::GetFullScreenQuadFragmentShaderTemplate(_QWORD);
// 180074660: using guessed type __int64 __fastcall vtkOpenGLRenderUtilities::GetFullScreenQuadVertexShader(_QWORD);
// 180074678: using guessed type __int64 vtkOpenGLBufferObject::New(void);
// 180074770: using guessed type __int64 (__fastcall *_glewBufferSubData)(_QWORD, _QWORD, _QWORD, _QWORD);
// 180074788: using guessed type __int64 (__fastcall *_glewBindBuffer)(_QWORD, _QWORD);
// 180074790: using guessed type __int64 (__fastcall *_glewBindImageTexture)(_QWORD, _QWORD, _QWORD, _QWORD, _DWORD, _DWORD, _DWORD);
// 1800747A0: using guessed type __int64 (__fastcall *_glewBindBufferBase)(_QWORD, _QWORD, _QWORD);

//----- (00000001800236E0) ----------------------------------------------------
// Hidden C++ exception states: #wind=4
void __fastcall sub_1800236E0(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  __int64 v3; // [rsp+0h] [rbp-1C8h] BYREF
  _BYTE v4[40]; // [rsp+28h] [rbp-1A0h] BYREF
  _BYTE v5[248]; // [rsp+50h] [rbp-178h] BYREF
  _BYTE v6[48]; // [rsp+148h] [rbp-80h] BYREF
  _BYTE v7[8]; // [rsp+178h] [rbp-50h] BYREF
  __int64 v8; // [rsp+180h] [rbp-48h]
  __int64 v9; // [rsp+188h] [rbp-40h]
  __int64 v10; // [rsp+190h] [rbp-38h]
  __int64 v11; // [rsp+198h] [rbp-30h]
  __int64 v12; // [rsp+1A0h] [rbp-28h]
  __int64 started; // [rsp+1A8h] [rbp-20h]
  __int64 v14; // [rsp+1B0h] [rbp-18h]

  v1 = &v3;
  for ( i = 112; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  v8 = -2;
  vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v4);
  sub_18000139D(v5, 2, 1);
  sub_1800017EE(v5, "vtkGrindSimulatePass::Finalize");
  v9 = *(_QWORD *)(a1 + 104);
  v10 = sub_180002270(v5, v6);
  v11 = v10;
  v12 = v10;
  started = vtkRenderTimerLog::StartScopedEvent(v9, v7, v10);
  v14 = started;
  vtkRenderTimerLog::ScopedEventLogger::operator=(v4, started);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v7);
  sub_180001B8B(v6);
  sub_1800023A6(v5);
  sub_180001C08(a1, 0xFFFFFFFFLL);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v4);
}
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 1800017EE: using guessed type __int64 __fastcall sub_1800017EE(_QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001C08: using guessed type double __fastcall sub_180001C08(_QWORD, _QWORD);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 180074260: using guessed type __int64 __fastcall vtkRenderTimerLog::StartScopedEvent(_QWORD, _QWORD, _QWORD);
// 180074268: using guessed type void vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 180074270: using guessed type __int64 __fastcall vtkRenderTimerLog::ScopedEventLogger::operator=(_QWORD, _QWORD);
// 180074278: using guessed type _QWORD vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);

//----- (0000000180023870) ----------------------------------------------------
void __fastcall sub_180023870(__int64 a1, int a2)
{
  __int64 *v2; // rdi
  __int64 i; // rcx
  __int64 v4; // [rsp+0h] [rbp-28h] BYREF

  v2 = &v4;
  for ( i = 8; i; --i )
  {
    *(_DWORD *)v2 = -858993460;
    v2 = (__int64 *)((char *)v2 + 4);
  }
  if ( a2 != *(_DWORD *)(a1 + 120) )
  {
    *(_DWORD *)(a1 + 120) = a2;
    vtkTimeStamp::Modified((vtkTimeStamp *)(a1 + 128));
  }
}
// 180073820: using guessed type void vtkTimeStamp::Modified(vtkTimeStamp *__hidden this);

//----- (00000001800238D0) ----------------------------------------------------
__int64 __fastcall sub_1800238D0(__int64 a1)
{
  _DWORD *v1; // rdi
  __int64 i; // rcx
  _QWORD v4[7]; // [rsp+0h] [rbp-38h] BYREF

  v1 = v4;
  for ( i = 12; i; --i )
    *v1++ = -858993460;
  if ( *(_DWORD *)(a1 + 160) )
  {
    glDeleteTextures(1, (const GLuint *)(a1 + 160));
    *(_DWORD *)(a1 + 160) = 0;
  }
  if ( *(_DWORD *)(a1 + 164) )
  {
    v4[4] = _glewDeleteBuffers;
    _glewDeleteBuffers(1, a1 + 164);
    *(_DWORD *)(a1 + 164) = 0;
  }
  if ( *(_DWORD *)(a1 + 168) )
  {
    v4[5] = _glewDeleteBuffers;
    _glewDeleteBuffers(1, a1 + 168);
    *(_DWORD *)(a1 + 168) = 0;
  }
  if ( *(_QWORD *)(a1 + 248) )
  {
    (*(void (__fastcall **)(_QWORD))(**(_QWORD **)(a1 + 248) + 16LL))(*(_QWORD *)(a1 + 248));
    *(_QWORD *)(a1 + 248) = 0;
  }
  if ( *(_QWORD *)(a1 + 184) )
  {
    (*(void (__fastcall **)(_QWORD))(**(_QWORD **)(a1 + 184) + 16LL))(*(_QWORD *)(a1 + 184));
    *(_QWORD *)(a1 + 184) = 0;
  }
  if ( *(_QWORD *)(a1 + 192) )
  {
    (*(void (__fastcall **)(_QWORD))(**(_QWORD **)(a1 + 192) + 16LL))(*(_QWORD *)(a1 + 192));
    *(_QWORD *)(a1 + 192) = 0;
  }
  if ( *(_QWORD *)(a1 + 208) )
  {
    (*(void (__fastcall **)(_QWORD))(**(_QWORD **)(a1 + 208) + 16LL))(*(_QWORD *)(a1 + 208));
    *(_QWORD *)(a1 + 208) = 0;
  }
  if ( *(_QWORD *)(a1 + 216) )
  {
    (*(void (__fastcall **)(_QWORD))(**(_QWORD **)(a1 + 216) + 16LL))(*(_QWORD *)(a1 + 216));
    *(_QWORD *)(a1 + 216) = 0;
  }
  if ( *(_QWORD *)(a1 + 232) )
  {
    (*(void (__fastcall **)(_QWORD))(**(_QWORD **)(a1 + 232) + 16LL))(*(_QWORD *)(a1 + 232));
    *(_QWORD *)(a1 + 232) = 0;
  }
  if ( *(_QWORD *)(a1 + 240) )
  {
    (*(void (__fastcall **)(_QWORD))(**(_QWORD **)(a1 + 240) + 16LL))(*(_QWORD *)(a1 + 240));
    *(_QWORD *)(a1 + 240) = 0;
  }
  *(_QWORD *)(a1 + 176) = 0;
  *(_QWORD *)(a1 + 200) = 0;
  *(_QWORD *)(a1 + 224) = 0;
  return a1;
}
// 180074778: using guessed type __int64 (__fastcall *_glewDeleteBuffers)(_QWORD, _QWORD);

//----- (0000000180023BA0) ----------------------------------------------------
// Hidden C++ exception states: #wind=4
void __fastcall sub_180023BA0(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  __int64 v3; // [rsp+0h] [rbp-1D8h] BYREF
  _BYTE v4[40]; // [rsp+28h] [rbp-1B0h] BYREF
  _BYTE v5[248]; // [rsp+50h] [rbp-188h] BYREF
  _BYTE v6[48]; // [rsp+148h] [rbp-90h] BYREF
  _BYTE v7[8]; // [rsp+178h] [rbp-60h] BYREF
  __int64 v8; // [rsp+180h] [rbp-58h]
  __int64 v9; // [rsp+188h] [rbp-50h]
  __int64 v10; // [rsp+190h] [rbp-48h]
  __int64 v11; // [rsp+198h] [rbp-40h]
  __int64 v12; // [rsp+1A0h] [rbp-38h]
  __int64 started; // [rsp+1A8h] [rbp-30h]
  __int64 v14; // [rsp+1B0h] [rbp-28h]
  __int64 v15; // [rsp+1B8h] [rbp-20h]
  void (__fastcall *v16)(__int64, _QWORD); // [rsp+1C0h] [rbp-18h]

  v1 = &v3;
  for ( i = 116; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  v8 = -2;
  vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v4);
  sub_18000139D(v5, 2, 1);
  sub_1800017EE(v5, "vtkGrindSimulatePass::RenderOpaquePass");
  v9 = *(_QWORD *)(a1 + 104);
  v10 = sub_180002270(v5, v6);
  v11 = v10;
  v12 = v10;
  started = vtkRenderTimerLog::StartScopedEvent(v9, v7, v10);
  v14 = started;
  vtkRenderTimerLog::ScopedEventLogger::operator=(v4, started);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v7);
  sub_180001B8B(v6);
  sub_1800023A6(v5);
  v15 = *(_QWORD *)(a1 + 64);
  v16 = *(void (__fastcall **)(__int64, _QWORD))(**(_QWORD **)(a1 + 64) + 144LL);
  v16(v15, *(_QWORD *)(a1 + 112));
  ++*(_DWORD *)(a1 + 136);
  vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger((vtkRenderTimerLog::ScopedEventLogger *)v4);
}
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 1800017EE: using guessed type __int64 __fastcall sub_1800017EE(_QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 180074260: using guessed type __int64 __fastcall vtkRenderTimerLog::StartScopedEvent(_QWORD, _QWORD, _QWORD);
// 180074268: using guessed type void vtkRenderTimerLog::ScopedEventLogger::~ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);
// 180074270: using guessed type __int64 __fastcall vtkRenderTimerLog::ScopedEventLogger::operator=(_QWORD, _QWORD);
// 180074278: using guessed type _QWORD vtkRenderTimerLog::ScopedEventLogger::ScopedEventLogger(vtkRenderTimerLog::ScopedEventLogger *__hidden this);

//----- (0000000180023D90) ----------------------------------------------------
struct vtkLight *__fastcall sub_180023D90(__int64 a1, int *a2, int *a3)
{
  __int64 *v3; // rdi
  __int64 i; // rcx
  vtkRenderer *Renderer; // rax
  struct vtkLight *result; // rax
  __int64 v7; // [rsp+0h] [rbp-58h] BYREF
  vtkLightCollection *Lights; // [rsp+20h] [rbp-38h]
  struct vtkLight *v9; // [rsp+28h] [rbp-30h]
  void *v10; // [rsp+38h] [rbp-20h] BYREF
  float v11; // [rsp+44h] [rbp-14h]

  v3 = &v7;
  for ( i = 20; i; --i )
  {
    *(_DWORD *)v3 = -858993460;
    v3 = (__int64 *)((char *)v3 + 4);
  }
  *a2 = 0;
  *a3 = 0;
  Renderer = vtkRenderState::GetRenderer(*(vtkRenderState **)(a1 + 112));
  Lights = vtkRenderer::GetLights(Renderer);
  vtkCollection::InitTraversal(Lights, &v10);
  while ( 1 )
  {
    result = vtkLightCollection::GetNextLight(Lights, &v10);
    v9 = result;
    if ( !result )
      break;
    v11 = (float)(*(int (__fastcall **)(struct vtkLight *))(*(_QWORD *)v9 + 344LL))(v9);
    if ( v11 > 0.0 )
    {
      ++*a3;
      if ( !*a2 )
        *a2 = 1;
    }
    if ( *a2 == 1 && (*a3 > 1 || (*(unsigned int (__fastcall **)(struct vtkLight *))(*(_QWORD *)v9 + 504LL))(v9) != 1) )
      *a2 = 2;
    if ( *a2 < 3 )
    {
      if ( (*(unsigned int (__fastcall **)(struct vtkLight *))(*(_QWORD *)v9 + 376LL))(v9) )
        *a2 = 3;
    }
  }
  return result;
}
// 180073790: using guessed type void vtkCollection::InitTraversal(vtkCollection *__hidden this, void **);
// 180074258: using guessed type struct vtkLight *vtkLightCollection::GetNextLight(vtkLightCollection *__hidden this, void **);
// 1800742A8: using guessed type struct vtkRenderer *vtkRenderState::GetRenderer(vtkRenderState *__hidden this);
// 1800742B8: using guessed type struct vtkLightCollection *vtkRenderer::GetLights(vtkRenderer *__hidden this);

//----- (0000000180023F00) ----------------------------------------------------
// Hidden C++ exception states: #wind=8
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
// 18002482E: variable 'v6' is possibly undefined
// 1800248CB: variable 'v7' is possibly undefined
// 180024A55: variable 'v10' is possibly undefined
// 180024C67: variable 'v16' is possibly undefined
// 180024ED1: variable 'v24' is possibly undefined
// 180025243: variable 'v38' is possibly undefined
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 1800017EE: using guessed type __int64 __fastcall sub_1800017EE(_QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 180002590: using guessed type __int64 __fastcall sub_180002590(_QWORD, _QWORD);
// 1800026FD: using guessed type __int64 sub_1800026FD(_QWORD, const char *, ...);
// 1800731D0: using guessed type __int64 __fastcall std::ostream::operator<<(_QWORD, _QWORD);
// 180073228: using guessed type __int64 __fastcall std::ios::clear(_QWORD, _QWORD, _QWORD);
// 1800745E0: using guessed type __int64 __fastcall vtkShaderProgram::Substitute(_QWORD, _QWORD, _QWORD, _QWORD);

//----- (00000001800252A0) ----------------------------------------------------
// Hidden C++ exception states: #wind=7
__int64 __fastcall sub_1800252A0(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  const float *v3; // rax
  float v4; // xmm0_4
  const float *v5; // rax
  const float *v6; // rax
  float v7; // xmm0_4
  __int64 v8; // rax
  float v9; // xmm0_4
  float v10; // xmm0_4
  float v11; // xmm0_4
  float v12; // xmm0_4
  __int64 v13; // rax
  int v14; // eax
  int TextureUnit; // eax
  vtkMatrix4x4 *v16; // rax
  vtkMatrix4x4 *v17; // rax
  struct vtkMatrix4x4 *v18; // rax
  __int64 v20; // [rsp+0h] [rbp-538h] BYREF
  vtkShaderProgram *v21; // [rsp+30h] [rbp-508h]
  __int64 v22; // [rsp+38h] [rbp-500h]
  unsigned int v23; // [rsp+40h] [rbp-4F8h]
  _BYTE v24[56]; // [rsp+58h] [rbp-4E0h] BYREF
  _BYTE v25[16]; // [rsp+90h] [rbp-4A8h] BYREF
  _BYTE v26[248]; // [rsp+A0h] [rbp-498h] BYREF
  _BYTE v27[56]; // [rsp+198h] [rbp-3A0h] BYREF
  _BYTE v28[16]; // [rsp+1D0h] [rbp-368h] BYREF
  _BYTE v29[248]; // [rsp+1E0h] [rbp-358h] BYREF
  _BYTE v30[48]; // [rsp+2D8h] [rbp-260h] BYREF
  _BYTE v31[32]; // [rsp+308h] [rbp-230h] BYREF
  double v32[8]; // [rsp+328h] [rbp-210h] BYREF
  float v33[12]; // [rsp+368h] [rbp-1D0h] BYREF
  vtkTextureObject *v34; // [rsp+398h] [rbp-1A0h] BYREF
  _DWORD v35[12]; // [rsp+3B8h] [rbp-180h] BYREF
  double v36[6]; // [rsp+3E8h] [rbp-150h] BYREF
  double v37[6]; // [rsp+418h] [rbp-120h] BYREF
  double v38[5]; // [rsp+448h] [rbp-F0h] BYREF
  bool v39; // [rsp+470h] [rbp-C8h]
  _BYTE v40[24]; // [rsp+478h] [rbp-C0h] BYREF
  __int64 v41; // [rsp+490h] [rbp-A8h]
  __int64 v42; // [rsp+498h] [rbp-A0h]
  __int64 v43; // [rsp+4A0h] [rbp-98h]
  __int64 v44; // [rsp+4A8h] [rbp-90h]
  float v45; // [rsp+4B0h] [rbp-88h]
  __int64 v46; // [rsp+4B8h] [rbp-80h]
  __int64 v47; // [rsp+4C0h] [rbp-78h]
  const char *v48; // [rsp+4C8h] [rbp-70h]
  __int64 v49; // [rsp+4D0h] [rbp-68h]
  const float *v50; // [rsp+4D8h] [rbp-60h]
  __int64 v51; // [rsp+4E0h] [rbp-58h]
  __int64 v52; // [rsp+4E8h] [rbp-50h]
  const char *v53; // [rsp+4F0h] [rbp-48h]
  _QWORD *v54; // [rsp+4F8h] [rbp-40h]
  __int64 v55; // [rsp+500h] [rbp-38h]
  vtkMatrix4x4 *v56; // [rsp+508h] [rbp-30h]
  __int64 v57; // [rsp+510h] [rbp-28h]
  vtkMatrix4x4 *v58; // [rsp+518h] [rbp-20h]

  v1 = &v20;
  for ( i = 332; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  v41 = -2;
  v21 = *(vtkShaderProgram **)(a1 + 224);
  v3 = (const float *)sub_180001F69(*(_QWORD *)(a1 + 80));
  vtkShaderProgram::SetUniform3f(v21, "grindBaseClr", v3);
  v4 = sub_180001B13(*(_QWORD *)(a1 + 80));
  vtkShaderProgram::SetUniformf(v21, "grindResectPos", v4);
  v5 = (const float *)sub_180001483(*(_QWORD *)(a1 + 80));
  vtkShaderProgram::SetUniform3f(v21, "grindResectClr", v5);
  v6 = (const float *)sub_1800011F9(*(_QWORD *)(a1 + 80));
  vtkShaderProgram::SetUniform3f(v21, "grindExpandDefaultClr", v6);
  v22 = sub_180001DE8(*(_QWORD *)(a1 + 80));
  v23 = 0;
  sub_1800027AC(v22, v24);
  while ( 1 )
  {
    v42 = sub_180002419(v22, v40);
    v43 = v42;
    v39 = sub_1800015C8((__int64)v24, v42);
    sub_1800016C2(v40);
    if ( !v39 )
      break;
    sub_180002487(v25, 3, 1);
    v44 = sub_1800017EE(v26, "expandLen");
    std::ostream::operator<<(v44, v23);
    v7 = *(double *)sub_18000246E(v24);
    v45 = v7;
    v46 = sub_180001ACD(v25, v27);
    v47 = v46;
    v48 = (const char *)sub_18000236F(v46);
    vtkShaderProgram::SetUniformf(v21, v48, v45);
    sub_180001B8B(v27);
    sub_180002487(v28, 3, 1);
    v49 = sub_1800017EE(v29, "expandClr");
    std::ostream::operator<<(v49, v23);
    v8 = sub_18000246E(v24);
    v50 = (const float *)sub_1800010D2(v8 + 8);
    v51 = sub_180001ACD(v28, v30);
    v52 = v51;
    v53 = (const char *)sub_18000236F(v51);
    vtkShaderProgram::SetUniform3f(v21, v53, v50);
    sub_180001B8B(v30);
    ++v23;
    sub_1800025A9(v28);
    sub_1800025A9(v25);
    sub_180001258(v24);
  }
  sub_1800016C2(v24);
  sub_180001A8C(v31);
  if ( (unsigned int)sub_1800018AC(*(_QWORD *)(a1 + 80)) )
  {
    if ( (unsigned int)sub_1800018AC(*(_QWORD *)(a1 + 80)) == 1 )
    {
      v34 = 0;
      v57 = *(_QWORD *)(a1 + 80);
      v58 = (vtkMatrix4x4 *)sub_180001852(v31);
      sub_1800012DA(v57, &v34, v35, v36, v37, v58);
      if ( v34 )
      {
        vtkTextureObject::Activate(v34);
        TextureUnit = vtkTextureObject::GetTextureUnit(v34);
        vtkShaderProgram::SetUniformi(v21, "grindTex", TextureUnit);
      }
      vtkShaderProgram::SetUniform3f(v21, "grindOrigin", v36);
      vtkShaderProgram::SetUniform3f(v21, "grindSpacing", v37);
      v38[0] = (double)v35[0];
      v38[1] = (double)v35[1];
      v38[2] = (double)v35[2];
      vtkShaderProgram::SetUniform3f(v21, "grindSize", v38);
    }
  }
  else
  {
    v54 = *(_QWORD **)(a1 + 80);
    sub_1800019E7(v54, v32);
    v9 = v32[0];
    v33[0] = v9;
    v10 = v32[1];
    v33[1] = v10;
    v11 = v32[2];
    v33[2] = v11;
    v12 = v32[3];
    v33[3] = v12;
    vtkShaderProgram::SetUniform4f(v21, "grindGeomSize", v33);
    v55 = *(_QWORD *)(a1 + 80);
    v56 = (vtkMatrix4x4 *)sub_180001852(v31);
    sub_1800016A9(v55, v56);
    if ( (unsigned int)sub_180001424(*(_QWORD *)(a1 + 80)) == 5
      || (unsigned int)sub_180001424(*(_QWORD *)(a1 + 80)) == 6 )
    {
      v13 = sub_18000257C(*(_QWORD *)(a1 + 80));
      v14 = sub_1800027B1(v13);
      vtkShaderProgram::SetUniformi(v21, "PGP_Count", v14 / 2);
    }
  }
  v16 = (vtkMatrix4x4 *)sub_180002766(v31);
  vtkMatrix4x4::Invert(v16);
  v17 = (vtkMatrix4x4 *)sub_180002766(v31);
  vtkMatrix4x4::Transpose(v17);
  v18 = (struct vtkMatrix4x4 *)sub_180001852(v31);
  vtkShaderProgram::SetUniformMatrix(v21, "grindMatrix", v18);
  return sub_180001F32(v31);
}
// 1800010D2: using guessed type __int64 __fastcall sub_1800010D2(_QWORD);
// 180001258: using guessed type __int64 __fastcall sub_180001258(_QWORD);
// 1800016C2: using guessed type __int64 __fastcall sub_1800016C2(_QWORD);
// 1800017EE: using guessed type __int64 __fastcall sub_1800017EE(_QWORD, _QWORD);
// 180001852: using guessed type __int64 __fastcall sub_180001852(_QWORD);
// 180001A8C: using guessed type __int64 __fastcall sub_180001A8C(_QWORD);
// 180001ACD: using guessed type __int64 __fastcall sub_180001ACD(_QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180001F32: using guessed type __int64 __fastcall sub_180001F32(_QWORD);
// 18000236F: using guessed type __int64 __fastcall sub_18000236F(_QWORD);
// 180002419: using guessed type __int64 __fastcall sub_180002419(_QWORD, _QWORD);
// 18000246E: using guessed type __int64 __fastcall sub_18000246E(_QWORD);
// 180002487: using guessed type __int64 __fastcall sub_180002487(_QWORD, _QWORD, _QWORD);
// 1800025A9: using guessed type __int64 __fastcall sub_1800025A9(_QWORD);
// 180002766: using guessed type __int64 __fastcall sub_180002766(_QWORD);
// 1800027AC: using guessed type __int64 __fastcall sub_1800027AC(_QWORD, _QWORD);
// 1800027B1: using guessed type __int64 __fastcall sub_1800027B1(_QWORD);
// 1800731D0: using guessed type __int64 __fastcall std::ostream::operator<<(_QWORD, _QWORD);
// 180073AC8: using guessed type void vtkMatrix4x4::Transpose(vtkMatrix4x4 *__hidden this);
// 180073AD0: using guessed type void vtkMatrix4x4::Invert(vtkMatrix4x4 *__hidden this);
// 1800745C8: using guessed type void vtkTextureObject::Activate(vtkTextureObject *__hidden this);
// 1800745D0: using guessed type _DWORD vtkTextureObject::GetTextureUnit(vtkTextureObject *__hidden this);
// 1800745E8: using guessed type bool vtkShaderProgram::SetUniformMatrix(vtkShaderProgram *__hidden this, const char *, struct vtkMatrix4x4 *);
// 1800745F8: using guessed type bool vtkShaderProgram::SetUniform4f(vtkShaderProgram *__hidden this, const char *, const float *const);
// 180074600: using guessed type bool vtkShaderProgram::SetUniform3f(vtkShaderProgram *__hidden this, const char *, const double *const);
// 180074608: using guessed type bool vtkShaderProgram::SetUniform3f(vtkShaderProgram *__hidden this, const char *, const float *const);
// 180074618: using guessed type __m128 vtkShaderProgram::SetUniformf(vtkShaderProgram *__hidden this, const char *, float);
// 180074620: using guessed type bool vtkShaderProgram::SetUniformi(vtkShaderProgram *__hidden this, const char *, _DWORD);

//----- (0000000180025970) ----------------------------------------------------
__m128 __fastcall sub_180025970(__int64 a1)
{
  __int64 *v1; // rdi
  __int64 i; // rcx
  float v3; // xmm0_4
  float v4; // xmm0_4
  float v5; // xmm0_4
  float v6; // xmm0_4
  __int64 v8; // [rsp+0h] [rbp-58h] BYREF
  struct vtkProperty *Property; // [rsp+20h] [rbp-38h]
  double v10; // [rsp+28h] [rbp-30h]
  double v11; // [rsp+30h] [rbp-28h]
  double v12; // [rsp+38h] [rbp-20h]
  double v13; // [rsp+40h] [rbp-18h]
  vtkShaderProgram *v14; // [rsp+48h] [rbp-10h]

  v1 = &v8;
  for ( i = 20; i; --i )
  {
    *(_DWORD *)v1 = -858993460;
    v1 = (__int64 *)((char *)v1 + 4);
  }
  Property = vtkActor::GetProperty(*(vtkActor **)(a1 + 144));
  v10 = (*(double (__fastcall **)(struct vtkProperty *))(*(_QWORD *)Property + 368LL))(Property);
  v11 = (*(double (__fastcall **)(struct vtkProperty *))(*(_QWORD *)Property + 400LL))(Property);
  v12 = (*(double (__fastcall **)(struct vtkProperty *))(*(_QWORD *)Property + 432LL))(Property);
  v13 = (*(double (__fastcall **)(struct vtkProperty *))(*(_QWORD *)Property + 464LL))(Property);
  v14 = *(vtkShaderProgram **)(a1 + 224);
  v3 = v10;
  vtkShaderProgram::SetUniformf(v14, "ambientIntensity", v3);
  v4 = v11;
  vtkShaderProgram::SetUniformf(v14, "diffuseIntensity", v4);
  v5 = v12;
  vtkShaderProgram::SetUniformf(v14, "specularIntensity", v5);
  v6 = v13;
  return vtkShaderProgram::SetUniformf(v14, "specularPowerUniform", v6);
}
// 1800742C0: using guessed type struct vtkProperty *vtkActor::GetProperty(vtkActor *__hidden this);
// 180074618: using guessed type __m128 vtkShaderProgram::SetUniformf(vtkShaderProgram *__hidden this, const char *, float);

//----- (0000000180025AA0) ----------------------------------------------------
// Hidden C++ exception states: #wind=17
__int64 __fastcall sub_180025AA0(__int64 a1, int a2)
{
  __int64 *v2; // rdi
  __int64 v3; // rcx
  __int64 result; // rax
  vtkRenderer *Renderer; // rax
  vtkRenderer *v6; // rax
  float v7; // xmm0_4
  float v8; // xmm0_4
  float v9; // xmm0_4
  const char *v10; // rax
  float v11; // xmm0_4
  float v12; // xmm0_4
  float v13; // xmm0_4
  const char *v14; // rax
  float v15; // xmm0_4
  float v16; // xmm0_4
  float v17; // xmm0_4
  float v18; // xmm0_4
  float v19; // xmm0_4
  float v20; // xmm0_4
  const char *v21; // rax
  const char *v22; // rax
  float v23; // xmm0_4
  float v24; // xmm0_4
  __int64 v25; // [rsp+0h] [rbp-698h] BYREF
  vtkShaderProgram *v26; // [rsp+20h] [rbp-678h]
  struct vtkCamera *ActiveCamera; // [rsp+28h] [rbp-670h]
  vtkAbstractTransform *v28; // [rsp+30h] [rbp-668h]
  unsigned int v29; // [rsp+38h] [rbp-660h]
  vtkLightCollection *Lights; // [rsp+40h] [rbp-658h]
  vtkLight *NextLight; // [rsp+48h] [rbp-650h]
  void *v32[4]; // [rsp+58h] [rbp-640h] BYREF
  float v33[12]; // [rsp+78h] [rbp-620h] BYREF
  float v34[12]; // [rsp+A8h] [rbp-5F0h] BYREF
  _BYTE v35[64]; // [rsp+D8h] [rbp-5C0h] BYREF
  _BYTE v36[64]; // [rsp+118h] [rbp-580h] BYREF
  _BYTE v37[64]; // [rsp+158h] [rbp-540h] BYREF
  _BYTE v38[64]; // [rsp+198h] [rbp-500h] BYREF
  _BYTE v39[64]; // [rsp+1D8h] [rbp-4C0h] BYREF
  _BYTE v40[64]; // [rsp+218h] [rbp-480h] BYREF
  _BYTE v41[72]; // [rsp+258h] [rbp-440h] BYREF
  _BYTE v42[244]; // [rsp+2A0h] [rbp-3F8h] BYREF
  float v43; // [rsp+394h] [rbp-304h]
  _BYTE v44[48]; // [rsp+398h] [rbp-300h] BYREF
  _BYTE v45[48]; // [rsp+3C8h] [rbp-2D0h] BYREF
  double *v46; // [rsp+3F8h] [rbp-2A0h]
  double v47; // [rsp+400h] [rbp-298h]
  _BYTE v48[40]; // [rsp+408h] [rbp-290h] BYREF
  const double *TransformedFocalPoint; // [rsp+430h] [rbp-268h]
  const double *TransformedPosition; // [rsp+438h] [rbp-260h]
  double v51[4]; // [rsp+448h] [rbp-250h] BYREF
  double *v52; // [rsp+468h] [rbp-230h]
  _BYTE v53[56]; // [rsp+470h] [rbp-228h] BYREF
  float v54[12]; // [rsp+4A8h] [rbp-1F0h] BYREF
  float v55[8]; // [rsp+4D8h] [rbp-1C0h] BYREF
  double *v56; // [rsp+4F8h] [rbp-1A0h]
  double *v57; // [rsp+500h] [rbp-198h]
  _BYTE v58[40]; // [rsp+508h] [rbp-190h] BYREF
  _BYTE v59[40]; // [rsp+530h] [rbp-168h] BYREF
  _BYTE v60[40]; // [rsp+558h] [rbp-140h] BYREF
  _BYTE v61[40]; // [rsp+580h] [rbp-118h] BYREF
  _BYTE v62[48]; // [rsp+5A8h] [rbp-F0h] BYREF
  __int64 v63; // [rsp+5D8h] [rbp-C0h]
  __int64 v64; // [rsp+5E0h] [rbp-B8h]
  __int64 v65; // [rsp+5E8h] [rbp-B0h]
  __int64 v66; // [rsp+5F0h] [rbp-A8h]
  __int64 v67; // [rsp+5F8h] [rbp-A0h]
  __int64 v68; // [rsp+600h] [rbp-98h]
  __int64 v69; // [rsp+608h] [rbp-90h]
  int v70; // [rsp+610h] [rbp-88h]
  __int64 v71; // [rsp+618h] [rbp-80h]
  __int64 v72; // [rsp+620h] [rbp-78h]
  const char *v73; // [rsp+628h] [rbp-70h]
  __int64 v74; // [rsp+630h] [rbp-68h]
  __int64 v75; // [rsp+638h] [rbp-60h]
  float v76; // [rsp+640h] [rbp-58h]
  __int64 v77; // [rsp+648h] [rbp-50h]
  __int64 v78; // [rsp+650h] [rbp-48h]
  const char *v79; // [rsp+658h] [rbp-40h]
  float v80; // [rsp+660h] [rbp-38h]
  __int64 v81; // [rsp+668h] [rbp-30h]
  __int64 v82; // [rsp+670h] [rbp-28h]
  const char *v83; // [rsp+678h] [rbp-20h]

  v2 = &v25;
  v3 = 420;
  result = 3435973836LL;
  while ( v3 )
  {
    *(_DWORD *)v2 = -858993460;
    v2 = (__int64 *)((char *)v2 + 4);
    --v3;
  }
  v63 = -2;
  if ( a2 >= 1 )
  {
    v26 = *(vtkShaderProgram **)(a1 + 224);
    Renderer = vtkRenderState::GetRenderer(*(vtkRenderState **)(a1 + 112));
    ActiveCamera = vtkRenderer::GetActiveCamera(Renderer);
    v28 = (vtkAbstractTransform *)(*(__int64 (__fastcall **)(struct vtkCamera *))(*(_QWORD *)ActiveCamera + 640LL))(ActiveCamera);
    v29 = 0;
    v6 = vtkRenderState::GetRenderer(*(vtkRenderState **)(a1 + 112));
    Lights = vtkRenderer::GetLights(v6);
    sub_1800026FD(v35, "lightColor");
    sub_1800026FD(v36, "lightDirectionVC");
    sub_1800026FD(v37, "lightAttenuation");
    sub_1800026FD(v38, "lightPositional");
    sub_1800026FD(v39, "lightPositionVC");
    sub_1800026FD(v40, "lightExponent");
    sub_1800026FD(v41, "lightConeAngle");
    sub_18000139D(v42, 2, 1);
    vtkCollection::InitTraversal(Lights, v32);
    while ( 1 )
    {
      NextLight = vtkLightCollection::GetNextLight(Lights, v32);
      if ( !NextLight )
        break;
      v43 = (float)(*(int (__fastcall **)(vtkLight *))(*(_QWORD *)NextLight + 344LL))(NextLight);
      if ( v43 > 0.0 )
      {
        sub_1800026FD(v44, (const char *)&unk_18004FA5F);
        sub_180002590(v42, v44);
        sub_180001B8B(v44);
        std::ostream::operator<<(v42, v29);
        sub_180002270(v42, v45);
        v46 = (double *)(*(__int64 (__fastcall **)(vtkLight *))(*(_QWORD *)NextLight + 216LL))(NextLight);
        v47 = (*(double (__fastcall **)(vtkLight *))(*(_QWORD *)NextLight + 328LL))(NextLight);
        v7 = *v46 * v47;
        v33[0] = v7;
        v8 = v46[1] * v47;
        v33[1] = v8;
        v9 = v46[2] * v47;
        v33[2] = v9;
        v64 = sub_1800023BA(v48, v35, v45);
        v65 = v64;
        v10 = (const char *)sub_18000236F(v64);
        vtkShaderProgram::SetUniform3f(v26, v10, v33);
        sub_180001B8B(v48);
        if ( a2 >= 2 )
        {
          TransformedFocalPoint = vtkLight::GetTransformedFocalPoint(NextLight);
          TransformedPosition = vtkLight::GetTransformedPosition(NextLight);
          vtkMath::Subtract(TransformedFocalPoint, TransformedPosition, v51);
          vtkMath::Normalize(v51);
          v52 = vtkLinearTransform::TransformNormal(v28, v51);
          v11 = *v52;
          v34[0] = v11;
          v12 = v52[1];
          v34[1] = v12;
          v13 = v52[2];
          v34[2] = v13;
          v66 = sub_1800023BA(v53, v36, v45);
          v67 = v66;
          v14 = (const char *)sub_18000236F(v66);
          vtkShaderProgram::SetUniform3f(v26, v14, v34);
          sub_180001B8B(v53);
          if ( a2 >= 3 )
          {
            v56 = (double *)(*(__int64 (__fastcall **)(vtkLight *))(*(_QWORD *)NextLight + 472LL))(NextLight);
            v15 = *v56;
            v54[0] = v15;
            v16 = v56[1];
            v54[1] = v16;
            v17 = v56[2];
            v54[2] = v17;
            v57 = vtkAbstractTransform::TransformPoint(v28, TransformedPosition);
            v18 = *v57;
            v55[0] = v18;
            v19 = v57[1];
            v55[1] = v19;
            v20 = v57[2];
            v55[2] = v20;
            v68 = sub_1800023BA(v58, v37, v45);
            v69 = v68;
            v21 = (const char *)sub_18000236F(v68);
            vtkShaderProgram::SetUniform3f(v26, v21, v54);
            sub_180001B8B(v58);
            v70 = (*(__int64 (__fastcall **)(vtkLight *))(*(_QWORD *)NextLight + 376LL))(NextLight);
            v71 = sub_1800023BA(v59, v38, v45);
            v72 = v71;
            v73 = (const char *)sub_18000236F(v71);
            vtkShaderProgram::SetUniformi(v26, v73, v70);
            sub_180001B8B(v59);
            v74 = sub_1800023BA(v60, v39, v45);
            v75 = v74;
            v22 = (const char *)sub_18000236F(v74);
            vtkShaderProgram::SetUniform3f(v26, v22, v55);
            sub_180001B8B(v60);
            v23 = (*(double (__fastcall **)(vtkLight *))(*(_QWORD *)NextLight + 424LL))(NextLight);
            v76 = v23;
            v77 = sub_1800023BA(v61, v40, v45);
            v78 = v77;
            v79 = (const char *)sub_18000236F(v77);
            vtkShaderProgram::SetUniformf(v26, v79, v76);
            sub_180001B8B(v61);
            v24 = (*(double (__fastcall **)(vtkLight *))(*(_QWORD *)NextLight + 440LL))(NextLight);
            v80 = v24;
            v81 = sub_1800023BA(v62, v41, v45);
            v82 = v81;
            v83 = (const char *)sub_18000236F(v81);
            vtkShaderProgram::SetUniformf(v26, v83, v80);
            sub_180001B8B(v62);
          }
        }
        ++v29;
        sub_180001B8B(v45);
      }
    }
    sub_1800023A6(v42);
    sub_180001B8B(v41);
    sub_180001B8B(v40);
    sub_180001B8B(v39);
    sub_180001B8B(v38);
    sub_180001B8B(v37);
    sub_180001B8B(v36);
    return sub_180001B8B(v35);
  }
  return result;
}
// 18000139D: using guessed type __int64 __fastcall sub_18000139D(_QWORD, _QWORD, _QWORD);
// 180001B8B: using guessed type __int64 __fastcall sub_180001B8B(_QWORD);
// 180002270: using guessed type __int64 __fastcall sub_180002270(_QWORD, _QWORD);
// 18000236F: using guessed type __int64 __fastcall sub_18000236F(_QWORD);
// 1800023A6: using guessed type __int64 __fastcall sub_1800023A6(_QWORD);
// 1800023BA: using guessed type __int64 __fastcall sub_1800023BA(_QWORD, _QWORD, _QWORD);
// 180002590: using guessed type __int64 __fastcall sub_180002590(_QWORD, _QWORD);
// 1800026FD: using guessed type __int64 sub_1800026FD(_QWORD, const char *, ...);
// 1800731D0: using guessed type __int64 __fastcall std::ostream::operator<<(_QWORD, _QWORD);
// 180073790: using guessed type void vtkCollection::InitTraversal(vtkCollection *__hidden this, void **);
// 180073798: using guessed type static double vtkMath::Normalize(double *const);
// 1800737A0: using guessed type static void vtkMath::Subtract(const double *const, const double *const, double *const);
// 180073B60: using guessed type double *vtkLinearTransform::TransformNormal(vtkLinearTransform *__hidden this, const double *const);
// 180073B68: using guessed type double *vtkAbstractTransform::TransformPoint(vtkAbstractTransform *__hidden this, const double *const);
// 180074248: using guessed type double *vtkLight::GetTransformedFocalPoint(vtkLight *__hidden this);
// 180074250: using guessed type double *vtkLight::GetTransformedPosition(vtkLight *__hidden this);
// 180074258: using guessed type struct vtkLight *vtkLightCollection::GetNextLight(vtkLightCollection *__hidden this, void **);
// 1800742A8: using guessed type struct vtkRenderer *vtkRenderState::GetRenderer(vtkRenderState *__hidden this);
// 1800742B0: using guessed type struct vtkCamera *vtkRenderer::GetActiveCamera(vtkRenderer *__hidden this);
// 1800742B8: using guessed type struct vtkLightCollection *vtkRenderer::GetLights(vtkRenderer *__hidden this);
// 180074608: using guessed type bool vtkShaderProgram::SetUniform3f(vtkShaderProgram *__hidden this, const char *, const float *const);
// 180074618: using guessed type __m128 vtkShaderProgram::SetUniformf(vtkShaderProgram *__hidden this, const char *, float);
// 180074620: using guessed type bool vtkShaderProgram::SetUniformi(vtkShaderProgram *__hidden this, const char *, _DWORD);

//----- (00000001800263E0) ----------------------------------------------------
__int64 __fastcall sub_1800263E0(__int64 a1, vtkRenderState *a2, vtkTextureObject **a3, unsigned int *a4)
{
  __int64 *v4; // rdi
  __int64 i; // rcx
  vtkRenderer *Renderer; // rax
  unsigned int v7; // eax
  __int64 v9; // [rsp+0h] [rbp-D8h] BYREF
  int v10[5]; // [rsp+34h] [rbp-A4h] BYREF
  vtkTextureObject *v11; // [rsp+48h] [rbp-90h]
  struct vtkOpenGLRenderWindow *RenderWindow; // [rsp+50h] [rbp-88h]
  vtkTextureObject *v13; // [rsp+58h] [rbp-80h]
  vtkTextureObject *v14; // [rsp+60h] [rbp-78h]
  vtkTextureObject *v15; // [rsp+68h] [rbp-70h]
  vtkTextureObject *v16; // [rsp+70h] [rbp-68h]
  void (__fastcall *v17)(vtkTextureObject *, _QWORD); // [rsp+78h] [rbp-60h]
  vtkTextureObject *v18; // [rsp+80h] [rbp-58h]
  void (__fastcall *v19)(vtkTextureObject *, _QWORD); // [rsp+88h] [rbp-50h]
  vtkTextureObject *v20; // [rsp+90h] [rbp-48h]
  void (__fastcall *v21)(vtkTextureObject *, _QWORD); // [rsp+98h] [rbp-40h]
  vtkTextureObject *v22; // [rsp+A0h] [rbp-38h]
  void (__fastcall *v23)(vtkTextureObject *, __int64); // [rsp+A8h] [rbp-30h]
  vtkTextureObject *v24; // [rsp+B0h] [rbp-28h]
  void (__fastcall *v25)(vtkTextureObject *, __int64); // [rsp+B8h] [rbp-20h]
  __int64 (__fastcall *v26)(_QWORD, _QWORD, __int64, __int64, int *); // [rsp+C0h] [rbp-18h]

  v4 = &v9;
  for ( i = 52; i; --i )
  {
    *(_DWORD *)v4 = -858993460;
    v4 = (__int64 *)((char *)v4 + 4);
  }
  *a3 = (vtkTextureObject *)vtkTextureObject::New(a1);
  v11 = *a3;
  Renderer = vtkRenderState::GetRenderer(a2);
  RenderWindow = vtkRenderer::GetRenderWindow(Renderer);
  vtkTextureObject::SetContext(v11, RenderWindow);
  v13 = *a3;
  vtkTextureObject::SetInternalFormat(v13, 0x822Eu);
  v14 = *a3;
  vtkTextureObject::SetFormat(v14, 0x1903u);
  v15 = *a3;
  vtkTextureObject::Allocate3D(v15, *a4, a4[1], a4[2], 1, 10);
  v16 = *a3;
  v17 = *(void (__fastcall **)(vtkTextureObject *, _QWORD))(*(_QWORD *)*a3 + 312LL);
  v17(v16, 0);
  v18 = *a3;
  v19 = *(void (__fastcall **)(vtkTextureObject *, _QWORD))(*(_QWORD *)*a3 + 328LL);
  v19(v18, 0);
  v20 = *a3;
  v21 = *(void (__fastcall **)(vtkTextureObject *, _QWORD))(*(_QWORD *)*a3 + 344LL);
  v21(v20, 0);
  v22 = *a3;
  v23 = *(void (__fastcall **)(vtkTextureObject *, __int64))(*(_QWORD *)*a3 + 360LL);
  v23(v22, 1);
  v24 = *a3;
  v25 = *(void (__fastcall **)(vtkTextureObject *, __int64))(*(_QWORD *)*a3 + 376LL);
  v25(v24, 1);
  v10[0] = 2123789977;
  v26 = (__int64 (__fastcall *)(_QWORD, _QWORD, __int64, __int64, int *))_glewClearTexImage;
  v7 = (*(__int64 (__fastcall **)(vtkTextureObject *))(*(_QWORD *)*a3 + 208LL))(*a3);
  return v26(v7, 0, 6403, 5126, v10);
}
// 1800742A8: using guessed type struct vtkRenderer *vtkRenderState::GetRenderer(vtkRenderState *__hidden this);
// 1800742E0: using guessed type struct vtkRenderWindow *vtkRenderer::GetRenderWindow(vtkRenderer *__hidden this);
// 1800744A0: using guessed type __int64 __fastcall vtkTextureObject::New(_QWORD);
// 1800744A8: using guessed type void vtkTextureObject::SetContext(vtkTextureObject *__hidden this, struct vtkOpenGLRenderWindow *);
// 1800744B8: using guessed type void vtkTextureObject::SetInternalFormat(vtkTextureObject *__hidden this, unsigned int);
// 1800744C0: using guessed type void vtkTextureObject::SetFormat(vtkTextureObject *__hidden this, unsigned int);
// 1800745B0: using guessed type bool vtkTextureObject::Allocate3D(vtkTextureObject *__hidden this, unsigned int, unsigned int, unsigned int, _DWORD, _DWORD);

//----- (00000001800266D0) ----------------------------------------------------
// Hidden C++ exception states: #wind=5
char __fastcall sub_1800266D0(__int64 a1, vtkOpenGLVertexArrayObject *a2, vtkShaderProgram *a3)
{
  __int64 *v3; // rdi
  __int64 i; // rcx
  const char *v5; // rax
  const char *v7; // rax
  const char *v8; // rax
  __int64 v9; // [rsp+0h] [rbp-298h] BYREF
  int v10; // [rsp+40h] [rbp-258h]
  char v11; // [rsp+50h] [rbp-248h]
  _DWORD v12[21]; // [rsp+70h] [rbp-228h] BYREF
  _BYTE v13[36]; // [rsp+C4h] [rbp-1D4h] BYREF
  _BYTE v14[48]; // [rsp+E8h] [rbp-1B0h] BYREF
  _BYTE v15[44]; // [rsp+118h] [rbp-180h] BYREF
  _BYTE v16[36]; // [rsp+144h] [rbp-154h] BYREF
  _BYTE v17[48]; // [rsp+168h] [rbp-130h] BYREF
  _BYTE v18[44]; // [rsp+198h] [rbp-100h] BYREF
  _BYTE v19[36]; // [rsp+1C4h] [rbp-D4h] BYREF
  _BYTE v20[56]; // [rsp+1E8h] [rbp-B0h] BYREF
  __int64 v21; // [rsp+220h] [rbp-78h]
  __int64 v22; // [rsp+228h] [rbp-70h]
  __int64 v23; // [rsp+230h] [rbp-68h]
  __int64 v24; // [rsp+238h] [rbp-60h]
  vtkOStrStreamWrapper *v25; // [rsp+240h] [rbp-58h]
  __int64 v26; // [rsp+248h] [rbp-50h]
  __int64 v27; // [rsp+250h] [rbp-48h]
  __int64 v28; // [rsp+258h] [rbp-40h]
  vtkOStrStreamWrapper *v29; // [rsp+260h] [rbp-38h]
  __int64 v30; // [rsp+268h] [rbp-30h]
  __int64 v31; // [rsp+270h] [rbp-28h]
  __int64 v32; // [rsp+278h] [rbp-20h]
  vtkOStrStreamWrapper *v33; // [rsp+280h] [rbp-18h]

  v3 = &v9;
  for ( i = 164; i; --i )
  {
    *(_DWORD *)v3 = -858993460;
    v3 = (__int64 *)((char *)v3 + 4);
  }
  v21 = -2;
  v12[0] = 1065353216;
  v12[1] = 1065353216;
  v12[2] = 1065353216;
  v12[3] = 1065353216;
  v12[4] = -1082130432;
  v12[5] = 1065353216;
  v12[6] = 0;
  v12[7] = 1065353216;
  v12[8] = 1065353216;
  v12[9] = -1082130432;
  v12[10] = 1065353216;
  v12[11] = 0;
  v12[12] = -1082130432;
  v12[13] = -1082130432;
  v12[14] = 0;
  v12[15] = 0;
  vtkOpenGLBufferObject::SetType(a1, 0);
  v11 = sub_180001F87(a1, v12, 16, 0);
  if ( v11 )
  {
    vtkOpenGLVertexArrayObject::Bind(a2);
    if ( !vtkShaderProgram::IsAttributeUsed(a3, "ndCoordIn")
      || (sub_1800026FD(v15, "ndCoordIn"),
          LOBYTE(v10) = 0,
          v11 = vtkOpenGLVertexArrayObject::AddAttributeArray(a2, a3, a1, v15, 0, 16, 10, 2, v10),
          sub_180001B8B(v15),
          v11) )
    {
      if ( !vtkShaderProgram::IsAttributeUsed(a3, "texCoordIn")
        || (sub_1800026FD(v18, "texCoordIn"),
            LOBYTE(v10) = 0,
            v11 = vtkOpenGLVertexArrayObject::AddAttributeArray(a2, a3, a1, v18, 8, 16, 10, 2, v10),
            sub_180001B8B(v18),
            v11) )
      {
        vtkOpenGLVertexArrayObject::Release(a2);
        return 1;
      }
      else
      {
        vtkOpenGLVertexArrayObject::Release(a2);
        if ( (unsigned int)vtkObject::GetGlobalWarningDisplay() )
        {
          vtkOStreamWrapper::UseEndl((const struct vtkOStreamWrapper::EndlType *)v19);
          vtkOStrStreamWrapper::vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v20);
          v30 = vtkOStreamWrapper::operator<<(
                  v20,
                  "Generic Warning: In d:\\develop\\yhq\\gcode\\ntcommonlib\\src\\grindsimulater\\src\\vtkgrindsimulatepass.cpp, line ");
          v31 = vtkOStreamWrapper::operator<<(v30, 976);
          v32 = vtkOStreamWrapper::operator<<(v31, "\nError binding texCoords to VAO.");
          vtkOStreamWrapper::operator<<(v32, "\n\n");
          v8 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v20);
          vtkOutputWindowDisplayGenericWarningText(v8);
          v33 = vtkOStrStreamWrapper::rdbuf((vtkOStrStreamWrapper *)v20);
          vtkOStrStreamWrapper::freeze(v33, 0);
          vtkOStrStreamWrapper::~vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v20);
        }
        return 0;
      }
    }
    else
    {
      vtkOpenGLVertexArrayObject::Release(a2);
      if ( (unsigned int)vtkObject::GetGlobalWarningDisplay() )
      {
        vtkOStreamWrapper::UseEndl((const struct vtkOStreamWrapper::EndlType *)v16);
        vtkOStrStreamWrapper::vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v17);
        v26 = vtkOStreamWrapper::operator<<(
                v17,
                "Generic Warning: In d:\\develop\\yhq\\gcode\\ntcommonlib\\src\\grindsimulater\\src\\vtkgrindsimulatepass.cpp, line ");
        v27 = vtkOStreamWrapper::operator<<(v26, 967);
        v28 = vtkOStreamWrapper::operator<<(v27, "\nError binding ndCoords to VAO.");
        vtkOStreamWrapper::operator<<(v28, "\n\n");
        v7 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v17);
        vtkOutputWindowDisplayGenericWarningText(v7);
        v29 = vtkOStrStreamWrapper::rdbuf((vtkOStrStreamWrapper *)v17);
        vtkOStrStreamWrapper::freeze(v29, 0);
        vtkOStrStreamWrapper::~vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v17);
      }
      return 0;
    }
  }
  else
  {
    if ( (unsigned int)vtkObject::GetGlobalWarningDisplay() )
    {
      vtkOStreamWrapper::UseEndl((const struct vtkOStreamWrapper::EndlType *)v13);
      vtkOStrStreamWrapper::vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v14);
      v22 = vtkOStreamWrapper::operator<<(
              v14,
              "Generic Warning: In d:\\develop\\yhq\\gcode\\ntcommonlib\\src\\grindsimulater\\src\\vtkgrindsimulatepass.cpp, line ");
      v23 = vtkOStreamWrapper::operator<<(v22, 957);
      v24 = vtkOStreamWrapper::operator<<(v23, "\nError uploading fullscreen quad vertex data.");
      vtkOStreamWrapper::operator<<(v24, "\n\n");
      v5 = vtkOStrStreamWrapper::str((vtkOStrStreamWrapper *)v14);
      vtkOutputWindowDisplayGenericWarningText(v5);
      v25 = vtkOStrStreamWrapper::rdbuf((vtkOStrStreamWrapper *)v14);
      vtkOStrStreamWrapper::freeze(v25, 0);
      vtkOStrStreamWrapper::~vtkOStrStreamWrapper((vtkOStrStreamWrapper *)v14);
    }
    return 0;
  }
}
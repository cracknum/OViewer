# vtk中ImageReslice的纹理管理

```c++
int vtkActor::RenderOpaqueGeometry(vtkViewport* vp)
{
  ...

  // Should we render during the opaque pass?
  if (this->HasOpaqueGeometry() || (ren->GetSelector() && this->Property->GetOpacity() > 0.0))
  {
    ...

    // render the texture
    if (this->Texture)
    {
      this->Texture->Render(ren);
      ...
    }
    ...
  }

  return renderedSomething;
}

int vtkActor::RenderTranslucentPolygonalGeometry(vtkViewport* vp)
{
  ...

  // Should we render during the translucent pass?
  if (this->HasTranslucentPolygonalGeometry() && !ren->GetSelector())
  {
    ...
    // render the texture
    if (this->Texture)
    {
      this->Texture->Render(ren);
      ...
    }
    ...
  }

  ...
  return renderedSomething;
}
void vtkTexture::Render(vtkRenderer* ren)
{
  ...
  this->Load(ren);
}

void vtkOpenGLTexture::Load(vtkRenderer* ren)
{
  vtkOpenGLRenderWindow* renWin = static_cast<vtkOpenGLRenderWindow*>(ren->GetRenderWindow());
  vtkOpenGLState* ostate = renWin->GetState();

  if (!this->ExternalTextureObject)
  {
    ...

    if (this->CubeMap)
    {
      ...
    }

    if (this->RenderWindow == nullptr && this->LoadTime.GetMTime() > this->GetMTime())
    {
      vtkErrorMacro("A render window was deleted without releasing graphics resources");
    }

    // Need to reload the texture.
    // There used to be a check on the render window's mtime, but
    // this is too broad of a check (e.g. it would cause all textures
    // to load when only the desired update rate changed).
    // If a better check is required, check something more specific,
    // like the graphics context.

    // has something changed so that we need to rebuild the texture?
    if (this->GetMTime() > this->LoadTime.GetMTime() || inputTime > this->LoadTime.GetMTime() ||
      (this->GetLookupTable() && this->GetLookupTable()->GetMTime() > this->LoadTime.GetMTime()) ||
      (this->RenderWindow == nullptr ||
        renWin->GetGenericContext() != this->RenderWindow->GetGenericContext()) ||
      renWin->GetContextCreationTime() > this->LoadTime)
    {
      ...

      // create the texture
      if (this->IsDepthTexture)
      {
        this->TextureObject->CreateDepthFromRaw(
          xsize, ysize, vtkTextureObject::Float32, dataType, nullptr);
      }
      else
      {
        if (numIns == 6)
        {
          this->TextureObject->CreateCubeFromRaw(xsize, ysize, bytesPerPixel, dataType, resultData);
        }
        else
        {
          this->TextureObject->Create2DFromRaw(
            xsize, ysize, bytesPerPixel, dataType, resultData[0]);
        }
      }

      ...
    }
  }
  else
  {
    ...
  }

  ...
}

bool vtkTextureObject::Create2DFromRaw(
  unsigned int width, unsigned int height, int numComps, int dataType, void* data)
{
  ...
  this->CreateTexture();
  this->Bind();

  ...
  

  glTexImage2D(this->Target, 0, this->InternalFormat, static_cast<GLsizei>(this->Width),
    static_cast<GLsizei>(this->Height), 0, this->Format, this->Type,
    static_cast<const GLvoid*>(data));

  ...
}

void vtkTextureObject::CreateTexture()
{
  ...
  if (!this->Handle)
  {
    GLuint tex = 0;
    glGenTextures(1, &tex);
    this->OwnHandle = true;
    vtkOpenGLCheckErrorMacro("failed at glGenTextures");
    this->Handle = tex;

#if defined(GL_TEXTURE_BUFFER)
    if (this->Target && this->Target != GL_TEXTURE_BUFFER)
#else
    if (this->Target)
#endif
    {
      glBindTexture(this->Target, this->Handle);
      vtkOpenGLCheckErrorMacro("failed at glBindTexture");

      // See: http://www.opengl.org/wiki/Common_Mistakes#Creating_a_complete_texture
      // turn off mip map filter or set the base and max level correctly. here
      // both are done.
#ifdef GL_TEXTURE_2D_MULTISAMPLE
      if (this->Target != GL_TEXTURE_2D_MULTISAMPLE)
#endif
      {
        glTexParameteri(this->Target, GL_TEXTURE_MIN_FILTER,
          this->GetMinificationFilterMode(this->MinificationFilter));
        glTexParameteri(this->Target, GL_TEXTURE_MAG_FILTER,
          this->GetMagnificationFilterMode(this->MagnificationFilter));

        glTexParameteri(this->Target, GL_TEXTURE_WRAP_S, this->GetWrapSMode(this->WrapS));
        glTexParameteri(this->Target, GL_TEXTURE_WRAP_T, this->GetWrapTMode(this->WrapT));

#if defined(GL_TEXTURE_3D)
        if (this->Target == GL_TEXTURE_3D)
        {
          glTexParameteri(this->Target, GL_TEXTURE_WRAP_R, this->GetWrapRMode(this->WrapR));
        }
#endif
      }

      if (this->Target == GL_TEXTURE_2D) // maybe expand later on
      {
        glTexParameteri(this->Target, GL_TEXTURE_BASE_LEVEL, this->BaseLevel);
        glTexParameteri(this->Target, GL_TEXTURE_MAX_LEVEL, this->MaxLevel);
      }

      glBindTexture(this->Target, 0);
    }
  }
}
```

从上面的源码可以得到vtkTexture的每次传递数据都是直接create的Texture。
#include "ABufferRenderPass.h"
#include <spdlog/spdlog.h>
#include <vtkAbstractMapper.h>
#include <vtkActorCollection.h>
#include <vtkObjectFactory.h>
#include <vtkOpenGLError.h>
#include <vtkOpenGLFramebufferObject.h>
#include <vtkOpenGLQuadHelper.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLShaderCache.h>
#include <vtkOpenGLState.h>
#include <vtkRenderState.h>
#include <vtkShader.h>
#include <vtkShaderProgram.h>
#include <vtkSmartPointer.h>
#include <vtkTextureObject.h>

namespace Grinding
{
struct ABufferNode
{
  float color[4];
  float pos[4];
  unsigned int next;
};

static_assert(sizeof(ABufferNode) == 36, "ABufferNode size not equal to 36");
}

struct ABufferRenderPass::Private
{
  // head pointer image texture
  vtkSmartPointer<vtkTextureObject> mHeadPointerTex;
  // manage resource self
  GLuint mHeadPointerId;
  int mHeadPointerTexSize[2];

  // link list atomic counter buffer
  unsigned int mAtomicCounterBuffer;
  // shader storage buffer object nodes
  unsigned int mSSBONodes;
  int mMaxNodes;

  vtkSmartPointer<vtkActorCollection> mRenderCollections;

  // render frameBuffer
  vtkSmartPointer<vtkOpenGLFramebufferObject> mFrameBuffer;
  vtkSmartPointer<vtkTextureObject> mColorTexture;
#if defined(GRINDING_ABUFFER_RENDER_PASS_DEBUG)
  std::unique_ptr<vtkOpenGLQuadHelper> mDrawHelper;
#endif

  Private()
    : mAtomicCounterBuffer(0)
    , mSSBONodes(0)
    , mMaxNodes(0)
    , mHeadPointerId(0)
    , mHeadPointerTexSize{}
  {
  }
};

vtkStandardNewMacro(ABufferRenderPass);

void ABufferRenderPass::Render(const vtkRenderState* s)
{
  auto renderer = vtkOpenGLRenderer::SafeDownCast(s->GetRenderer());
  auto renderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderer->GetRenderWindow());
  auto ostate = renderWindow->GetState();

  int* windowSize = renderWindow->GetSize();
  renderWindow->MakeCurrent();
  if (!mPrivate->mRenderCollections)
  {
    SPDLOG_ERROR("using SetRenderActorCollection set abuffer relative collections first");
    return;
  }

  if (!mPrivate->mColorTexture)
  {
    mPrivate->mColorTexture = vtkSmartPointer<vtkTextureObject>::New();
    mPrivate->mColorTexture->SetContext(renderWindow);
    mPrivate->mColorTexture->Allocate2D(windowSize[0], windowSize[1], 4, VTK_UNSIGNED_CHAR, 0);
    SPDLOG_ERROR("{}", glGetError());
  }
  else if (mPrivate->mColorTexture->GetWidth() != windowSize[0] ||
    mPrivate->mColorTexture->GetHeight() != windowSize[1])
  {
    mPrivate->mColorTexture->Resize(windowSize[0], windowSize[1]);
    SPDLOG_ERROR("{}", glGetError());
  }

  if (!mPrivate->mFrameBuffer)
  {
    mPrivate->mFrameBuffer = vtkSmartPointer<vtkOpenGLFramebufferObject>::New();
    mPrivate->mFrameBuffer->SetContext(renderWindow);
    ostate->PushFramebufferBindings();
    mPrivate->mFrameBuffer->Bind();
    mPrivate->mFrameBuffer->AddColorAttachment(0, mPrivate->mColorTexture);
    ostate->PopDrawFramebufferBinding();
    SPDLOG_ERROR("{}", glGetError());
  }

  bool windowResized = false;

  if (mPrivate->mHeadPointerId &&
    !std::equal(mPrivate->mHeadPointerTexSize, mPrivate->mHeadPointerTexSize + 2, windowSize)
    )
  {
    windowResized = true;
    glDeleteTextures(1, &mPrivate->mHeadPointerId);
    mPrivate->mHeadPointerTex = 0;
  }
  if (!mPrivate->mHeadPointerTex)
  {
    mPrivate->mHeadPointerTex = vtkSmartPointer<vtkTextureObject>::New();
    mPrivate->mHeadPointerTex->SetContext(renderWindow);
    // TODO: head pointer分配可能有错误，需要检查，通过glGetTexLevelParameteriv无法获取对应尺寸
    // 原因是在vtkTextureObject::GetDefaultFormat()时使用硬编码无法获取GL_RED_INTEGER，只能返回GL_RED
    // 从而导致出现无效枚举的错误，只能将这里改为原始的opengl代码
    mPrivate->mHeadPointerTexSize[0] = windowSize[0];
    mPrivate->mHeadPointerTexSize[1] = windowSize[1];
    glGenTextures(1, &mPrivate->mHeadPointerId);
    glBindTexture(GL_TEXTURE_2D, mPrivate->mHeadPointerId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, windowSize[0], windowSize[1], 0, GL_RED_INTEGER,
      GL_UNSIGNED_INT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    mPrivate->mHeadPointerTex->AssignToExistingTexture(mPrivate->mHeadPointerId, GL_TEXTURE_2D);
    mPrivate->mHeadPointerTex->SetDataType(GL_UNSIGNED_INT);
    mPrivate->mHeadPointerTex->SetFormat(GL_RED);
    mPrivate->mHeadPointerTex->SetInternalFormat(GL_R32UI);
    mPrivate->mHeadPointerTex->SetMagnificationFilter(GL_NEAREST);
    mPrivate->mHeadPointerTex->SetMinificationFilter(GL_NEAREST);
    mPrivate->mHeadPointerTex->SetWrapS(GL_CLAMP_TO_BORDER);
    mPrivate->mHeadPointerTex->SetWrapT(GL_CLAMP_TO_BORDER);
  }

  {
    glBindImageTexture(2, mPrivate->mHeadPointerId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    unsigned int null = 0xffffffff;
    glClearTexImage(mPrivate->mHeadPointerId, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &null);
  }

  if (!mPrivate->mAtomicCounterBuffer)
  {
    glGenBuffers(1, &mPrivate->mAtomicCounterBuffer);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, mPrivate->mAtomicCounterBuffer);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 1, mPrivate->mAtomicCounterBuffer);
  }
  {
    uint32_t zero = 0;
    glClearBufferData(GL_ATOMIC_COUNTER_BUFFER, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, &zero);
  }

  if (!mPrivate->mSSBONodes || windowResized)
  {
    SPDLOG_INFO("gen ssbo");
    mPrivate->mMaxNodes = windowSize[0] * windowSize[1] * 4 * 2;
    glGenBuffers(1, &mPrivate->mSSBONodes);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mPrivate->mSSBONodes);
    glBufferData(GL_SHADER_STORAGE_BUFFER, mPrivate->mMaxNodes * sizeof(Grinding::ABufferNode),
      nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mPrivate->mSSBONodes);
  }
  {
    uint32_t zero = 0;
    glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, &zero);
  }

  {
    ostate->PushFramebufferBindings();
    mPrivate->mFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetDrawMode());
    vtkOpenGLState::ScopedglColorMask saveColorMask(ostate);
    vtkOpenGLState::ScopedglDepthMask saveDepthMask(ostate);
    ostate->vtkglClear(GL_COLOR_BUFFER_BIT);
    // ostate->vtkglColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    ostate->vtkglDepthMask(GL_FALSE);

    this->PreRender(s);
    this->UpdateCamera(renderer);
    this->UpdateLightGeometry(renderer);
    this->UpdateLights(renderer);
    mPrivate->mRenderCollections->InitTraversal();
    vtkActor* actor = nullptr;
    while ((actor = mPrivate->mRenderCollections->GetNextActor()))
    {
      actor->Render(renderer, actor->GetMapper());
    }
    this->PostRender(s);

    GLuint counterVal;
    glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &counterVal);
    std::cout << "Total nodes written: " << counterVal << std::endl;
    ostate->PopFramebufferBindings();
  }

#if defined(GRINDING_ABUFFER_RENDER_PASS_DEBUG)
  {
    ostate->PushFramebufferBindings();
    mPrivate->mFrameBuffer->Bind(vtkOpenGLFramebufferObject::GetReadMode());
    ostate->vtkglBlitFramebuffer(0, 0, windowSize[0], windowSize[1], 0, 0, windowSize[0],
      windowSize[1], GL_COLOR_BUFFER_BIT, GL_LINEAR);
    ostate->PopFramebufferBindings();
  }
#endif // GRINDING_ABUFFER_RENDER_PASS_DEBUG
}

void ABufferRenderPass::ReleaseGraphicsResources(vtkWindow* w)
{
  // this->ReleaseGraphicsResources(w);
  if (mPrivate->mHeadPointerTex)
  {
    mPrivate->mHeadPointerTex->ReleaseGraphicsResources(w);
  }
  if (mPrivate->mAtomicCounterBuffer)
  {
    glDeleteBuffers(1, &mPrivate->mAtomicCounterBuffer);
    mPrivate->mAtomicCounterBuffer = 0;
  }
  if (mPrivate->mSSBONodes)
  {
    glDeleteBuffers(1, &mPrivate->mSSBONodes);
    mPrivate->mSSBONodes = 0;
  }

  if (mPrivate->mHeadPointerId)
  {
    glDeleteTextures(1, &mPrivate->mHeadPointerId);
    mPrivate->mHeadPointerId = 0;
  }
}

bool ABufferRenderPass::PreReplaceShaderValues(std::string& vertexShader,
  std::string& geometryShader, std::string& fragmentShader, vtkAbstractMapper* mapper,
  vtkProp* prop)
{
  vtkShaderProgram::Substitute(fragmentShader, "//VTK::System::Dec", "#version 440 core", false);
  vtkShaderProgram::Substitute(vertexShader, "//VTK::System::Dec", "#version 440 core", false);
  vtkShaderProgram::Substitute(fragmentShader, "//VTK::CustomUniforms::Dec",
    R"(
		//VTK::CustomUniforms::Dec
		struct ABufferNode
		{
			vec4 color;
			vec4 position;
			unsigned int next;
		};

		layout(std430, binding = 0) buffer ABufferStorage
		{
			ABufferNode nodes[];
		};

		layout(binding = 1, offset = 0) uniform atomic_uint nodeCounter;
		layout(r32ui, binding = 2) uniform uimage2D headPointerImage;

		uniform uint maxNodes;
	)",
    false);
  // TODO: 这里获取的颜色不正确

  vtkShaderProgram::Substitute(fragmentShader, "//VTK::Coincident::Impl",
    R"(
	 	//VTK::Coincident::Impl
		ivec2 pos = ivec2(gl_FragCoord.xy);
  		uint newNodeIndex = atomicCounterIncrement(nodeCounter);
  		if (newNodeIndex >= maxNodes)
  		{
  		  return;
  		}

  		nodes[newNodeIndex].color = fragOutput0;
  		nodes[newNodeIndex].position = vertexVC;
		
  		// insert new node in head
  		uint prevNodeIndex = imageAtomicExchange(headPointerImage, pos, newNodeIndex);
  		nodes[newNodeIndex].next = prevNodeIndex;

		discard;
	 )",
    false);

  return true;
}

bool ABufferRenderPass::SetShaderParameters(vtkShaderProgram* program, vtkAbstractMapper* mapper,
  vtkProp* prop, vtkOpenGLVertexArrayObject* VAO)
{
  glUniform1ui(glGetUniformLocation(program->GetHandle(), "maxNodes"), mPrivate->mMaxNodes);
  glBindImageTexture(2, mPrivate->mHeadPointerId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

  return true;
}

ABufferRenderPass::ABufferRenderPass()
{
  mPrivate = std::make_unique<Private>();
}

ABufferRenderPass::~ABufferRenderPass() = default;
vtkTextureObject* ABufferRenderPass::GetHeadPointerImage() const
{
  return mPrivate->mHeadPointerTex;
}

void ABufferRenderPass::SetRenderActorCollection(vtkActorCollection* collection)
{
  mPrivate->mRenderCollections = collection;
}

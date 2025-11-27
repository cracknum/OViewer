#include "FrameBuffer.h"
#include <QOpenGLFunctions_4_4_Core>

struct FrameBuffer::Impl
{
  Functions* m_Functions;
  GLuint m_FrameBuffer;
  GLuint m_Texture;
  int m_Width;
  int m_Height;

  Impl(Functions* functions, int width, int height)
    : m_Functions(functions)
    , m_FrameBuffer(0)
    , m_Width(width)
    , m_Height(height)
  {
  }
};

FrameBuffer::FrameBuffer(Functions* functions, int width, int height)
{
  m_Impl = std::make_unique<Impl>(functions, width, height);
  createFrameBuffer();
}

FrameBuffer::~FrameBuffer() {}

void FrameBuffer::bind() const {}

void FrameBuffer::unbind() const {}

void FrameBuffer::createFrameBuffer()
{
  auto* functions = m_Impl->m_Functions;
  functions->glGenFramebuffers(1, &m_Impl->m_FrameBuffer);
  functions->glBindFramebuffer(GL_FRAMEBUFFER, m_Impl->m_FrameBuffer);

  functions->glGenTextures(1, &m_Impl->m_Texture);
  functions->glBindTexture(GL_TEXTURE_2D, m_Impl->m_Texture);
  functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Impl->m_Width, m_Impl->m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Impl->m_Texture, 0);
}

void FrameBuffer::updateBufferSize(int width, int height) {
  auto* functions = m_Impl->m_Functions;
  if (width != m_Impl->m_Width || height != m_Impl->m_Height)
  {
    deleteBuffer();
    createFrameBuffer();
  }
  
}

void FrameBuffer::deleteBuffer()
{
  if (m_Impl->m_FrameBuffer)
  {
    auto* functions = m_Impl->m_Functions;
    functions->glDeleteFramebuffers(1, &m_Impl->m_FrameBuffer);
    functions->glDeleteTextures(1, &m_Impl->m_Texture);
    m_Impl->m_FrameBuffer = 0;
    m_Impl->m_Texture = 0;
  }
  
}

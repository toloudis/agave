#include "Util.h"

#include "glsl/v330/V330GLImageShader2DnoLut.h"
#include "Logging.h"

#include "glm.h"

#include <array>
#include <iostream>

static bool GL_ERROR_CHECKS_ENABLED = true;

void
check_glfb(std::string const& message) {
  if (!GL_ERROR_CHECKS_ENABLED) {
    return;
  }
  check_gl(message);
  GLint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    std::string statusstr = "Unknown " + std::to_string(status); 
    switch(status) {
      case GL_FRAMEBUFFER_UNDEFINED: statusstr = "GL_FRAMEBUFFER_UNDEFINED"; break;
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: statusstr = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: statusstr = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: statusstr = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: statusstr = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
      case GL_FRAMEBUFFER_UNSUPPORTED: statusstr = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
      case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: statusstr = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
      case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: statusstr = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
      default:break;
    }
    LOG_DEBUG << "Framebuffer not complete! Error code: " << statusstr;
  }
}

void
check_gl(std::string const& message)
{
  if (!GL_ERROR_CHECKS_ENABLED) {
    return;
  }

  GLenum err = GL_NO_ERROR;
  while ((err = glGetError()) != GL_NO_ERROR) {
    std::string msg = "GL error (" + message + ") :";
    switch (err) {
      case GL_INVALID_ENUM:
        msg += "Invalid enum";
        break;
      case GL_INVALID_VALUE:
        msg += "Invalid value";
        break;
      case GL_INVALID_OPERATION:
        msg += "Invalid operation";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        msg += "Invalid framebuffer operation";
        break;
      case GL_OUT_OF_MEMORY:
        msg += "Out of memory";
        break;
      case GL_STACK_UNDERFLOW:
        msg += "Stack underflow";
        break;
      case GL_STACK_OVERFLOW:
        msg += "Stack overflow";
        break;
      default:
        msg += "Unknown (" + std::to_string(err) + ')';
        break;
    }
    LOG_DEBUG << msg;
  }
}

RectImage2D::RectImage2D()
{
  // setup geometry
  glm::vec2 xlim(-1.0, 1.0);
  glm::vec2 ylim(-1.0, 1.0);
  const std::array<GLfloat, 8> square_vertices{
    xlim[0], ylim[0], xlim[1], ylim[0], xlim[1], ylim[1], xlim[0], ylim[1]
  };

  glGenVertexArrays(1, &_quadVertexArray);
  glBindVertexArray(_quadVertexArray);
  check_gl("create and bind verts");

  glGenBuffers(1, &_quadVertices);
  glBindBuffer(GL_ARRAY_BUFFER, _quadVertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * square_vertices.size(), square_vertices.data(), GL_STATIC_DRAW);
  check_gl("init vtx coord data");

  glm::vec2 texxlim(0.0, 1.0);
  glm::vec2 texylim(0.0, 1.0);
  std::array<GLfloat, 8> square_texcoords{ texxlim[0], texylim[0], texxlim[1], texylim[0],
                                           texxlim[1], texylim[1], texxlim[0], texylim[1] };

  glGenBuffers(1, &_quadTexcoords);
  glBindBuffer(GL_ARRAY_BUFFER, _quadTexcoords);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * square_texcoords.size(), square_texcoords.data(), GL_STATIC_DRAW);
  check_gl("init texcoord data");

  std::array<GLushort, 6> square_elements{ // front
                                           0, 1, 2, 2, 3, 0
  };

  glGenBuffers(1, &_quadIndices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadIndices);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * square_elements.size(), square_elements.data(), GL_STATIC_DRAW);
  _num_image_elements = square_elements.size();
  check_gl("init element data");

  glBindVertexArray(0);
  check_gl("unbind vtx array");

  _image_shader = new GLImageShader2DnoLut();
}

RectImage2D::~RectImage2D()
{
  delete _image_shader;

  glDeleteVertexArrays(1, &_quadVertexArray);
  _quadVertexArray = 0;
  glDeleteBuffers(1, &_quadVertices);
  _quadVertices = 0;
  glDeleteBuffers(1, &_quadTexcoords);
  _quadTexcoords = 0;
  glDeleteBuffers(1, &_quadIndices);
  _quadIndices = 0;
}

void
RectImage2D::draw(GLuint texture2d)
{
  _image_shader->bind();
  check_gl("Bind shader");

  _image_shader->setModelViewProjection(glm::mat4(1.0));

  glActiveTexture(GL_TEXTURE0);
  check_gl("Activate texture");
  glBindTexture(GL_TEXTURE_2D, texture2d);
  check_gl("Bind texture");
  _image_shader->setTexture(0);

  glBindVertexArray(_quadVertexArray);
  check_gl("bind vtx buf");

  _image_shader->enableCoords();
  _image_shader->setCoords(_quadVertices, 0, 2);

  _image_shader->enableTexCoords();
  _image_shader->setTexCoords(_quadTexcoords, 0, 2);

  // Push each element to the vertex shader
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadIndices);
  check_gl("bind element buf");
  glDrawElements(GL_TRIANGLES, (GLsizei)_num_image_elements, GL_UNSIGNED_SHORT, 0);
  check_gl("Image2D draw elements");

  _image_shader->disableCoords();
  _image_shader->disableTexCoords();
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);

  _image_shader->release();
}

GLTimer::GLTimer(void)
{
  StartTimer();
}

GLTimer::~GLTimer(void)
{
  glDeleteQueries(1, &m_EventStart);
  glDeleteQueries(1, &m_EventStop);
}

void
GLTimer::StartTimer(void)
{
  glGenQueries(1, &m_EventStart);
  glGenQueries(1, &m_EventStop);
  glQueryCounter(m_EventStart, GL_TIMESTAMP);

  m_Started = true;
}

float
GLTimer::StopTimer(void)
{
  if (!m_Started)
    return 0.0f;

  glQueryCounter(m_EventStop, GL_TIMESTAMP);
  synchronize(m_EventStop);

  float TimeDelta = 0.0f;

  eventElapsedTime(&TimeDelta, m_EventStart, m_EventStop);
  glDeleteQueries(1, &m_EventStart);
  glDeleteQueries(1, &m_EventStop);

  m_Started = false;

  return TimeDelta;
}

float
GLTimer::ElapsedTime(void)
{
  if (!m_Started)
    return 0.0f;

  glQueryCounter(m_EventStop, GL_TIMESTAMP);
  synchronize(m_EventStop);

  float TimeDelta = 0.0f;

  eventElapsedTime(&TimeDelta, m_EventStart, m_EventStop);

  m_Started = false;

  return TimeDelta;
}

void
GLTimer::synchronize(GLuint eventid)
{
  // wait until the results are available
  GLint stopTimerAvailable = 0;
  while (!stopTimerAvailable) {
    glGetQueryObjectiv(eventid, GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable);
  }
}

void
GLTimer::eventElapsedTime(float* result, GLuint startEvent, GLuint stopEvent)
{
  GLuint64 startTime, stopTime;
  // get query results
  glGetQueryObjectui64v(startEvent, GL_QUERY_RESULT, &startTime);
  glGetQueryObjectui64v(stopEvent, GL_QUERY_RESULT, &stopTime);

  *result = (float)((double)(stopTime - startTime) / 1000000.0);
}

#ifndef RENDERER_H
#define RENDERER_H

#include "glad/glad.h"

#include <QList>
#include <QObject>

#include <QMutex>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLTexture>
#include <QThread>

#include <memory>

class commandBuffer;
class CCamera;
class ImageXYZC;
class RenderGLPT;
class RenderSettings;
class Scene;

#include "renderrequest.h"

class Renderer : public QThread
{
  Q_OBJECT

public:
  Renderer(QString id, QObject* parent, QMutex& mutex);
  virtual ~Renderer();

  void init();
  void run();

  void addRequest(RenderRequest* request);
  bool processRequest();

  inline int getTotalQueueDuration() { return this->m_totalQueueDuration; }

  inline int getRequestCount() { return this->m_requests.count(); }

  // 1 = continuous re-render, 0 = only wait for redraw commands
  void setStreamMode(int32_t mode) { m_streamMode = mode; }

  void resizeGL(int internalWidth, int internalHeight);

protected:
  QString m_id;

  void processCommandBuffer(RenderRequest* rr);
  QImage render();

  void reset(int from = 0);

  int getTime();

  QList<RenderRequest*> m_requests;
  int m_totalQueueDuration;

  void shutDown();

private:
  QMutex* m_openGLMutex;

  QOpenGLContext* m_glContext;
  QOffscreenSurface* m_surface;
  QOpenGLFramebufferObject* m_fbo;

  int32_t m_streamMode;
  int32_t m_width, m_height;

  QTime m_time;

  class SceneDescription
  {
  public:
    inline SceneDescription(QString name, int start, int end)
      : m_name(name)
      , m_start(start)
      , m_end(end)
    {}

    QString m_name;
    int m_start;
    int m_end;
  };

  QList<SceneDescription> m_scenes;

  // TODO move this info.  This class only knows about some abstract renderer and a scene object.
  void myVolumeInit();
  struct myVolumeData
  {
    RenderSettings* m_renderSettings;
    RenderGLPT* m_renderer;
    Scene* m_scene;
    CCamera* m_camera;

    myVolumeData()
      : m_camera(nullptr)
      , m_scene(nullptr)
      , m_renderSettings(nullptr)
      , m_renderer(nullptr)
    {}
  } m_myVolumeData;

signals:
  void kill();
  void requestProcessed(RenderRequest* request, QImage img);
  void sendString(RenderRequest* request, QString s);
};

#endif // RENDERER_H

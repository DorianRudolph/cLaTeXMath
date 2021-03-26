#if defined(BUILD_SKIA) && !defined(MEM_CHECK)

#include "rendercontext.h"
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QApplication>
#include <gpu/gl/GrGLAssembleInterface.h>
#include <core/SkSurfaceProps.h>
#include <QOffscreenSurface>

constexpr GrGLenum BACKEND_FORMAT = GL_RGBA8;
constexpr auto COLOR_TYPE = kRGBA_8888_SkColorType;
constexpr auto ORIGIN = kBottomLeft_GrSurfaceOrigin;

bool RenderContext::AllowEGL;

SkSurfaceProps RenderContext::SurfaceProps(0, SkPixelGeometry::kBGR_H_SkPixelGeometry); //TODO configure

void RenderContext::InitGL() {
  QSurfaceFormat fmt;
  fmt.setDepthBufferSize(0);
  fmt.setRedBufferSize(8);
  fmt.setGreenBufferSize(8);
  fmt.setBlueBufferSize(8);
  fmt.setStencilBufferSize(SKIA_STENCIL_BITS);
  fmt.setSamples(SKIA_SAMPLES);

  fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  fmt.setSwapInterval(1);

  // Request OpenGL 3.3 core or OpenGL ES 3.0.
  if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
    qDebug("Requesting 3.3 core context");
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
  } else {
    qDebug("Requesting 3.0 context");
    fmt.setVersion(3, 0);
  }

  QSurfaceFormat::setDefaultFormat(fmt);

  AllowEGL = QApplication::platformName() != "xcb";
}

RenderContext::RenderContext(QOpenGLContext *ctx, GrGLuint fbo)
    : ctx{ctx}, fbo{fbo}, grCtx{MakeGrContext(ctx)} {}


sk_sp<GrDirectContext> RenderContext::MakeGrContext(QOpenGLContext *ctx) {
  //assume ctx is current
  auto interface = GrGLMakeAssembledInterface(ctx, getProc);
  Q_ASSERT(interface);
  auto grCtx = GrDirectContext::MakeGL(interface);
  Q_ASSERT(grCtx);
  return grCtx;
}

GrGLFuncPtr RenderContext::getProc(void *ctx, const char *name) {
  // don't return an eglQueryString function on XCB
  // otherwise a segfault happens because it returns invalid values (?)
  if (!AllowEGL && !strcmp(name, "eglQueryString")) {
    return nullptr;
  }
  return static_cast<QOpenGLContext *>(ctx)->getProcAddress(name);
}

void RenderContext::createSurface(int w, int h) {
  GrGLFramebufferInfo info;
  info.fFBOID = fbo;
  info.fFormat = BACKEND_FORMAT;
  GrBackendRenderTarget target(w, h, SKIA_SAMPLES, SKIA_STENCIL_BITS, info);

  surface = SkSurface::MakeFromBackendRenderTarget(
      grCtx.get(), target, ORIGIN,
      COLOR_TYPE, nullptr, &SurfaceProps);
}

#endif
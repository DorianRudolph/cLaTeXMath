#if defined(BUILD_SKIA) && !defined(MEM_CHECK)

#ifndef QT_RENDERCONTEXT_H
#define QT_RENDERCONTEXT_H

#include <gpu/GrDirectContext.h>
#include <gpu/gl/GrGLInterface.h>
#include <core/SkSurface.h>
#include <memory>

constexpr int SKIA_STENCIL_BITS = 8;
constexpr int SKIA_SAMPLES = 0;


class QOpenGLContext;

class QOffscreenSurface;

class RenderContext {
public:
  static void InitGL();

  static sk_sp<GrDirectContext> MakeGrContext(QOpenGLContext *ctx);

  RenderContext(QOpenGLContext *ctx, GrGLuint fbo);

  void createSurface(int w, int h);

  sk_sp<SkSurface> surface{};
  static SkSurfaceProps SurfaceProps;
  sk_sp<GrDirectContext> grCtx;

private:
  static bool AllowEGL;

  static GrGLFuncPtr getProc(void *ctx, const char name[]);

  QOpenGLContext *ctx;
  GrGLuint fbo;
};

#endif
#endif
#include "config.h"

#if defined(BUILD_SKIA) && !defined(MEM_CHECK)

#ifndef QT_SKIATEXWIDGET_H
#define QT_SKIATEXWIDGET_H

#include "platform/qt/graphic_qt.h"
#include "latex.h"
#include "rendercontext.h"

#include <QOpenGLWidget>

class QOpenGLFunctions;

class TeXWidget : public QOpenGLWidget
{
 public:
  TeXWidget(QWidget* parent = nullptr, float text_size=20.f);
  virtual ~TeXWidget();
  float getTextSize();

  void setTextSize(float size);
  void setLaTeX(const std::wstring& latex);
  bool isRenderDisplayed();
  int getRenderWidth();
  int getRenderHeight();

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

private:
  tex::TeXRender* _render;
  float _text_size;
  int _padding;
  std::wstring _latex {};

  std::unique_ptr<RenderContext> _render_context;
  QOpenGLFunctions *_functions{};
};

#endif
#endif

#if defined(BUILD_SKIA) && !defined(MEM_CHECK)

#include "qt_skiatexwidget.h"
#include <QOpenGLContext>
#include <core/SkCanvas.h>
#include <QOpenGLFunctions>
#include "platform/skia/graphic_skia.h"

using namespace tex;

TeXWidget::TeXWidget(QWidget* parent, float text_size)
  : QOpenGLWidget(parent),
    _render(nullptr),
    _text_size(text_size),
    _padding(20)
{}

TeXWidget::~TeXWidget()
{
  if (_render != nullptr) delete _render;
}

float TeXWidget::getTextSize()
{
  return _text_size;
}

void TeXWidget::setTextSize(float size)
{
  if(size == _text_size) return;
  _text_size = size;
  if(_render != nullptr) {
    _render->setTextSize(_text_size);
    update();
  }
}

void TeXWidget::setLaTeX(const std::wstring& latex)
{
  if (_render != nullptr) delete _render;

  _render = LaTeX::parse(
        latex,
        width() - _padding * 2,
        _text_size,
        _text_size / 3.f,
        0xff000000);
  puts("setLaTeX");
  update();
}

bool TeXWidget::isRenderDisplayed()
{
  return _render != nullptr;
}

int TeXWidget::getRenderWidth()
{
  return _render == nullptr ? 0 : _render->getWidth() + _padding * 2;
}

int TeXWidget::getRenderHeight()
{
  return _render == nullptr ? 0 : _render->getHeight() + _padding * 2;
}

void TeXWidget::initializeGL() {
  makeCurrent();
  _functions = context()->functions();
  _render_context = std::make_unique<RenderContext>(context(), defaultFramebufferObject());
}

void TeXWidget::paintGL() {
  puts("paintGL");
  auto *canvas = _render_context->surface->getCanvas();
  canvas->clear(SK_ColorWHITE);
  SkPaint paint;
  canvas->drawCircle(100, 100, 50, paint);
  Graphics2D_skia g2 (canvas);
  if (_render) {
    _render->draw(g2, _padding, _padding);
  }
  canvas->flush();
}

void TeXWidget::resizeGL(int w, int h) {
  makeCurrent();
  puts("resizeGL");

  _functions->glViewport(0, 0, w, h);
  _functions->glClearColor(1, 1, 1, 1);
  _functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _functions->glClearStencil(0);

  _render_context->createSurface(w, h);
  update();
}


#endif

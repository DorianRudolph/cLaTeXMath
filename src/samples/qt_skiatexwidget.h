#include "config.h"

#if defined(BUILD_SKIAf) && !defined(MEM_CHECK)

#ifndef QT_TEXWIDGET_H
#define QT_TEXWIDGET_H

#include "latex.h"

#include <QOpenGLWidget>

class TeXWidget : public QOpenGLWidget
{
  Q_OBJECT

public:
  TeXWidget(QWidget* parent = nullptr, float text_size=20.f);
  virtual ~TeXWidget();
  float getTextSize();

  void setTextSize(float size);
  void setLaTeX(const std::wstring& latex);
  bool isRenderDisplayed();
  int getRenderWidth();
  int getRenderHeight();
  void paintEvent(QPaintEvent* event);

 private:
  tex::TeXRender* _render;
  float _text_size;
  int _padding;
};

#endif
#endif

#include "noiseviewer.h"

#include <QPainter>

NoiseViewer::NoiseViewer() {
  setWidth(500);
  setHeight(500);
}

void NoiseViewer::paint(QPainter* painter) {
  painter->fillRect(0, 0, width(), height(), QColor(255, 255, 0));
}

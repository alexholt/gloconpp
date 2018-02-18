#include "rect.h"

Rect::Rect() {
}

Rect::Rect(const Rect& other) : QObject() {
  m_x = other.x();
  m_x2 = other.x2();
  m_y = other.y();
  m_y2 = other.y2();
}

Rect::Rect(float x, float y, float x2, float y2) {
  m_x = x;
  m_y = y;
  m_x2 = x2;
  m_y2 = y2;
}
void Rect::setX(float x) {
  m_x = x;
}

float Rect::x() const {
  return m_x;
}

void Rect::setY(float y) {
  m_y = y;
}

float Rect::y() const {
  return m_y;
}

void Rect::setX2(float x2) {
  m_x2 = x2;
}

float Rect::x2() const {
  return m_x2;
}

void Rect::setY2(float y2) {
  m_y2 = y2;
}

float Rect::y2() const {
  return m_y2;
}

float Rect::width() const {
  return m_x2 - m_x;
}

float Rect::height() const {
  return m_y2 - m_y;
}

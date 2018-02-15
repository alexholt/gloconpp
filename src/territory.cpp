#include <limits>
#include <numeric>
#include <QVector3D>

#include "circle.h"
#include "edge.h"
#include "territory.h"
#include "triangle.h"

Territory::Territory(const QString& path) {
  m_path = path;
}

Territory::Territory(const QString& name, const QString& path) {
  m_name = name;
  m_path = path;
}

Territory::~Territory() {
  if (m_centroid != nullptr)
    delete m_centroid;
}

void Territory::processToken(QString token) {
  token = token.toLower();

  switch (token[0].unicode()) {
  case 'm':
  case 'v':
  case 'h':
  case 'z':
  case ',':
    stateChange(token);
    return;

  default:
    const float val = token.toFloat();
    QVector2D* point = new QVector2D;

    switch (m_pathState) {
    case BEGIN:
    case CLOSE_PATH:
      break;

    case MOVE:
      m_pathState = MOVE_X;
      m_currentVal = val + m_lastPoint->x();
      break;

    case MOVE_X:
      point->setX(m_currentVal);
      point->setY(val + m_lastPoint->y());
      m_pointArray << point;
      m_currentVal = NAN;
      m_lastPoint = m_pointArray.last();
      m_pathState = MOVE;
      break;

    case MOVE_H:
      m_lastPoint = m_pointArray.last();
      point->setX(m_lastPoint->x() + val);
      point->setY(m_lastPoint->y());
      m_pointArray << point;
      break;

    case MOVE_V:
      m_lastPoint = m_pointArray.last();
      point->setX(m_lastPoint->x());
      point->setY(m_lastPoint->y() + val);
      m_pointArray << point;
      break;
    }
  }
}

void Territory::stateChange(QString token) {
  switch (token[0].unicode()) {
  case 'm':
    m_pathState = MOVE;
    if (m_pointArray.length() > 0) {
      m_lastPoint = m_pointArray.last();
    } else {
      m_lastPoint->setX(0);
      m_lastPoint->setY(0);
    }
    break;
  case 'h':
    m_pathState = MOVE_H;
    break;
  case 'v':
    m_pathState = MOVE_V;
    break;
  case 'z':
    m_pathState = CLOSE_PATH;
    break;
  default:
    QString msg("Unknown path token: ");
    msg += token;
    throw std::runtime_error(msg.toStdString());
  }
}

QList<QVector2D*>& Territory::getPointArray() {
  if (m_pointArray.length() > 0) return m_pointArray;

  m_lastPoint->setX(0);
  m_lastPoint->setY(0);
  m_pathState = BEGIN;
  m_currentVal = NAN;
  QString token;

  for (int i = 0; i < m_path.length(); i++) {
    auto chr = m_path[i];
    if (chr == ' ' || chr == ',') {
      processToken(token);
      token = "";
    } else {
      token += chr;
    }
  }

  if (isClockwise()) {
    QList<QVector2D*> reversed;
    for (auto it = --m_pointArray.end(); it >= m_pointArray.begin(); it--) {
      reversed << *it;
    }
    m_pointArray = reversed;
  }

  return m_pointArray;
}

// http://mathworld.wolfram.com/PolygonArea.html
// Compute the signed area
bool Territory::isClockwise() {
  auto vertices = getPointArray();
  float area = 0.0f;
  for (int i = 0; i < vertices.length(); i++) {
    auto first = vertices[i];
    auto second = vertices[(i + 1) % vertices.length()];
    area += first->x() * second->y() - second->x() * first->y();
  }

  // This is intended for mapspace which has a flipped y-axis
  // so it's negative if it is clockwise
  return (area / 2) < 0;
}

Rect Territory::getBoundingBox() {
  if (m_boundingBox.width() > 0) {
    return m_boundingBox;
  }

  getPointArray();

  for (QVector2D* cur : m_pointArray) {
    if (m_boundingBox.x() > cur->x()) {
      m_boundingBox.setX(cur->x());
    }

    if (m_boundingBox.y() > cur->y()) {
      m_boundingBox.setY(cur->y());
    }

    if (m_boundingBox.x2() < cur->x()) {
      m_boundingBox.setX2(cur->x());
    }

    if (m_boundingBox.y2() < cur->y()) {
      m_boundingBox.setY2(cur->y());
    }
  }

  return m_boundingBox;
}

Triangle Territory::getSuperTriangle() {
  auto box = getBoundingBox();
  Triangle superTriangle{
    QVector3D{
      box.x() - 1.0f,
      box.y2() - 1.0f,
      0.0f
    },
    QVector3D{
      box.x() - 1.0f,
      box.y() + box.height() + 2.0f,
      0.0f
    },
    QVector3D{
      box.x2() + box.width() + 2.0f,
      box.y2() - 1.0f,
      0.0f
    }
  };

  return superTriangle;
}

// Formula is from https://en.wikipedia.org/wiki/Centroid, duh
QVector2D* Territory::getCentroid() {
  if (m_centroid != nullptr) return m_centroid;

  std::function<QList<QVector2D>(QList<QVector2D*>&, int)> nextTwo = [](QList<QVector2D*>& vertices, int i) {
    return QList<QVector2D>{
      QVector2D(vertices[i]->x(), vertices[i]->y()),
      QVector2D(vertices[(i + 1) % vertices.length()]->x(), vertices[(i + 1) % vertices.length()]->y())
    };
  };

  auto vertices = getPointArray();

  // a = 1/2 * sum( xi * yi1 - xi1 * yi )
  float a = 0;
  for (int i = 0; i < vertices.length(); i++) {
    auto points = nextTwo(vertices, i);
    a += points[0].x() * points[1].y() - points[1].x() * points[0].y();
  }

  a /= 2;

  // cx = 1/6a * sum( (xi + xi1) * (xi * yi1 - xi1 * yi) )
  float cx = 0;
  for (int i = 0; i < vertices.length(); i++) {
    auto points = nextTwo(vertices, i);
    cx += (points[0].x() + points[1].x()) * (points[0].x() * points[1].y() - points[1].x() * points[0].y());
  }
  cx *= 1 / (6 * a);

  // cy = 1/6a * sum( (yi + yi1) * (xi * yi1 - xi1 * yi) )
  float cy = 0;
  for (int i = 0; i < vertices.length(); i++) {
    auto points = nextTwo(vertices, i);
    cy += (points[0].y() + points[1].y()) * (points[0].x() * points[1].y() - points[1].x() * points[0].y());
  }
  cy *= 1 / (6 * a);

  m_centroid = new QVector2D{cx, cy};
  return m_centroid;
}

bool Territory::fuzzyVertexCompare(const QVector3D& first, const QVector3D& second) const {
  return qFuzzyCompare(first.x(), second.x()) && qFuzzyCompare(first.y(), second.y()) && qFuzzyCompare(first.z(), second.z());
}

QList<Triangle> Territory::getMesh() {
  auto vertices = getPointArray();

  if (m_mesh.length() > 0) {
    return m_mesh;
  }

  auto isConvex = [](QVector3D& left, QVector3D& center, QVector3D& right) -> bool {
    // Using CCW order the point is convex iff the det is 0
    return (left.x() - right.x()) * (center.y() - left.y()) - (center.x() - left.x()) * (left.y() - right.y()) > 0;
  };

  while (vertices.length() >= 3) {
    auto prev = vertices.takeFirst();
    auto center = vertices.takeFirst();
    auto next = vertices[0];

    QVector3D center3D{center->x(), center->y(), 0.0f};
    QVector3D prev3D{prev->x(), prev->y(), 0.0f};
    QVector3D next3D{next->x(), next->y(), 0.0f};

    if (fuzzyVertexCompare(center3D, prev3D) || fuzzyVertexCompare(center3D, next3D)) {
      // If a vertex is too similar to one of its neighbors our isConvex and isEar math
      // will fail so let's nudge the center point slightly off to where it was
      center3D.setX(center3D.x() + 0.001);
      center3D.setY(center3D.y() + 0.001);
    }

    Triangle triangle{prev3D, center3D, next3D};

    if (vertices.length() == 1) {
      // We removed the first two so if we started this iteration with 3 we are at 1
      // now
      m_mesh << triangle;
      break;
    }

    bool isEar = std::accumulate(vertices.begin(), vertices.end(), true, [&](bool acc, const QVector2D* point) {
      auto point3D = QVector3D{point->x(), point->y(), 0.0f};
      auto isEar = acc && ((point == prev || point == next) || !triangle.contains(point3D));
      return isEar;
    });

    if (isConvex(prev3D, center3D, next3D) && isEar) {
      // Clip it
      m_mesh << triangle;
      vertices.prepend(prev);
    } else {
      vertices << prev;
      vertices.prepend(center);
    }
  }

  QList<QVector3D> triangluated;
  for (auto triangle : m_mesh) {
    triangluated << triangle.top() << triangle.left() << triangle.bottom();
  }

  m_numVertices = triangluated.length();
  m_vertices = new float[m_numVertices * 8];


  for (int i = 0; i < triangluated.length(); i++) {
    int p = i * 8;
    m_vertices[p + 0] = triangluated[i].x() - 1000.0f;
    m_vertices[p + 1] = -triangluated[i].y() + 500.0f;
    m_vertices[p + 2] = 10.0f + 10.0f * 0.001;

    m_vertices[p + 3] = 0.0f;
    m_vertices[p + 4] = 0.0f;
    m_vertices[p + 5] = 0.0f;
    m_vertices[p + 6] = 0.0f;
    m_vertices[p + 7] = 0.0f;
  }

  m_numElements = m_mesh.length() * 3;
  //m_numElements = m_mesh.length() * 3 + m_mesh.length() * 2;
  m_elements = new ushort[m_numElements];

  for (uint i = 0; i < m_numElements; i++) {
    m_elements[i] = i;
  }

  return m_mesh;
}

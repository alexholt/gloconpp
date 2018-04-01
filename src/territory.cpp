#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <random>
#include <QImageWriter>
#include <QPainter>
#include <QVector3D>
#include <QVector2D>
#include <noise/noise.h>

#include "circle.h"
#include "edge.h"
#include "glocon.h"
#include "territory.h"
#include "triangle.h"

#define COMPARE_EPSILON 1.0f
#define SCALE 2000.0f

using namespace noise;

QVector2D fract(QVector3D arg) {
  float intPart;
  float fractX = std::modf(arg.x(), &intPart);
  float fractY = std::modf(arg.y(), &intPart);
  return QVector2D{fractX, fractY};
}

float fract(float arg) {
  float intPart;
  float fract = std::modf(arg, &intPart);
  return fract;
}

QVector2D floor(QVector2D arg) {
  return QVector2D{std::floor(arg.x()), std::floor(arg.y())};
}

QVector2D hash(QVector2D x) {
  const QVector2D k = QVector2D( 0.3183099f, 0.3678794f );
  x = x*k + QVector2D{k.y(), k.x()};
  auto retVec = 2.0f * fract( 16.0f * k * fract(x.x() * x.y() * (x.x() + x.y())) );
  retVec.setX(retVec.x() - 1.0f);
  retVec.setY(retVec.y() - 1.0f);
  return retVec;
}

// Noise function based on https://www.shadertoy.com/view/XdXBRH
QVector3D noised(QVector2D p) {
    p.setX(p.x() / SCALE);
    p.setY(p.y() / SCALE);
    QVector2D i = floor( p );
    QVector2D f = fract(p);

#if 1
    // quintic interpolation
    QVector2D vecTimes6 = f * 6;
    QVector2D vecMinus2 = f;
    vecMinus2.setX(vecMinus2.x() - 2.0f);
    vecMinus2.setY(vecMinus2.y() - 2.0f);
    vecTimes6.setX(vecTimes6.x() - 15.0f);
    vecTimes6.setY(vecTimes6.y() - 15.0f);
    vecTimes6 *= f;
    vecTimes6 = QVector2D{vecTimes6.x() + 10.0f, vecTimes6.y() + 10.0f};
    QVector2D u = f*f*f*vecTimes6;
    vecMinus2 *= f;
    vecMinus2 = QVector2D{vecMinus2.x() + 10.0f, vecMinus2.y() + 10.0f};
    QVector2D du = 30.0*f*f*vecMinus2;
#else
    // cubic interpolation
    QVector2D u = f*f*(3.0-2.0*f);
    QVector2D du = 6.0*f*(1.0-f);
#endif

    QVector2D ga = hash( i + QVector2D(0.0,0.0) );
    QVector2D gb = hash( i + QVector2D(1.0,0.0) );
    QVector2D gc = hash( i + QVector2D(0.0,1.0) );
    QVector2D gd = hash( i + QVector2D(1.0,1.0) );

    float va = QVector2D::dotProduct( ga, f - QVector2D(0.0,0.0) );
    float vb = QVector2D::dotProduct( gb, f - QVector2D(1.0,0.0) );
    float vc = QVector2D::dotProduct( gc, f - QVector2D(0.0,1.0) );
    float vd = QVector2D::dotProduct( gd, f - QVector2D(1.0,1.0) );

    auto z = QVector2D{u.y(), u.x()}*(va-vb-vc+vd) + QVector2D{vb,vc};
    z = QVector2D{z.x() - va, z.y() - va};
    z *= du;

    auto yz =
      ga + u.x()*(gb-ga) + u.y()*(gc-ga) + u.x()*u.y()*(ga-gb-gc+gd) +  // derivatives
      z
    ;

    return QVector3D{
      va + u.x()*(vb-va) + u.y()*(vc-va) + u.x()*u.y()*(va-vb-vc+vd),   // value
      yz.x(),
      yz.y()
    };
}

float noisedd(QVector2D p) {
  module::Billow primaryGranite;
  primaryGranite.SetSeed (0);
  primaryGranite.SetFrequency (8.0);
  primaryGranite.SetPersistence (0.625);
  primaryGranite.SetLacunarity (2.18359375);
  primaryGranite.SetOctaveCount (6);
  primaryGranite.SetNoiseQuality (QUALITY_STD);

  // Use Voronoi polygons to produce the small grains for the granite texture.
  module::Voronoi baseGrains;
  baseGrains.SetSeed (1);
  baseGrains.SetFrequency (16.0);
  baseGrains.EnableDistance (true);

  // Scale the small grain values so that they may be added to the base
  // granite texture.  Voronoi polygons normally generate pits, so apply a
  // negative scaling factor to produce bumps instead.
  module::ScaleBias scaledGrains;
  scaledGrains.SetSourceModule (0, baseGrains);
  scaledGrains.SetScale (-0.5);
  scaledGrains.SetBias (0.0);

  // Combine the primary granite texture with the small grain texture.
  module::Add combinedGranite;
  combinedGranite.SetSourceModule (0, primaryGranite);
  combinedGranite.SetSourceModule (1, scaledGrains);

  // Finally, perturb the granite texture to add realism.
  module::Turbulence finalGranite;
  finalGranite.SetSourceModule (0, combinedGranite);
  finalGranite.SetSeed (2);
  finalGranite.SetFrequency (4.0);
  finalGranite.SetPower (1.0 / 8.0);
  finalGranite.SetRoughness (6);

  model::Plane plane;
  plane.SetModule(finalGranite);
  auto val = static_cast<float>(plane.GetValue(p.x(), p.y()));
  val = std::min(std::max(val, -1.0f), 1.0f);
  val += 1.0f;
  val /= 2.0f;
  val *= 255.0f;

  return val;
}

Territory::Territory() {
}

Territory::Territory(const QString& path) {
  m_path = path;
}

Territory::Territory(const QString& path, const QString& name) {
  m_name = name;
  m_path = path;
}

Territory::Territory(const QString& path, const QString& name, bool ensureCCW) {
  m_name = name;
  m_path = path;
  getPointArray();
  if (ensureCCW && isClockwise()) {
    QList<QVector2D*> reversed;
    for (auto it = --m_pointArray.end(); it > m_pointArray.begin(); it--) {
      reversed << *it;
    }
    m_pointArray = reversed;
  }
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
  if (m_mesh.length() > 0)
    return m_mesh;
  buildMesh();
  return m_mesh;
}

void Territory::buildMesh() {
  auto vertices = getPointArray();

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

    if (triangle.isClockwise()) {
      triangle.flip();
    }

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

  QList<QVector3D> triangulated;
  for (int i = 0; i < m_mesh.length(); i++) {
    auto triangle = m_mesh[i];

    QVector3D top = triangle.top();
    QVector3D left = triangle.left();
    QVector3D bottom = triangle.bottom();
    auto x = -1000.0f;
    auto y = 500.0f;
    top.setX(top.x() + x);
    top.setY(-top.y() + y);
    left.setX(left.x() + x);
    left.setY(-left.y() + y);
    bottom.setX(bottom.x() + x);
    bottom.setY(-bottom.y() + y);

    triangle.setPoints(top, left, bottom);
    m_mesh.replace(i, triangle);

    triangulated << top << left << bottom;
  }

  buildVerticesFromPointList(triangulated);
}

void Territory::buildVerticesFromPointList(QList<QVector3D> points) {
  m_canRenderMutex.lock();
  m_canRender = false;
  m_isInitialized = false;
  m_canRenderMutex.unlock();

  if (m_vertices != nullptr) delete[] m_vertices;
  if (m_elements != nullptr) delete[] m_elements;

  m_numVertices = points.length();
  m_vertices = new float[m_numVertices * 8];

  QMap<std::string, float> lookup;
  for (int i = 0; i < points.length(); i++) {
    auto point = points[i];
    std::string vec;
    vec += std::to_string(point.x());
    vec += ',';
    vec += std::to_string(point.y());

    if (!lookup.contains(vec)) {
      lookup.insert(vec, noisedd(QVector2D(point) / SCALE) / 255.0f * 5.0f + 10.0f);
    }
  }

  for (int i = 0; i < points.length(); i++) {
    int p = i * 8;
    m_vertices[p + 0] = points[i].x();
    m_vertices[p + 1] = points[i].y();
    auto point = points[i];
    std::string vec;
    vec += std::to_string(point.x());
    vec += ',';
    vec += std::to_string(point.y());
    m_vertices[p + 2] = lookup[vec];
    points[i].setZ(m_vertices[p + 2]);

    m_vertices[p + 3] = 0.0f;
    m_vertices[p + 4] = 0.0f;
  }

  QVector3D normal;
  for (int i = 0; i < points.length(); i++) {
    int p = i * 8;

    if (i % 3 == 0) {
      auto cur = points[i];
      auto next = points[i + 1];
      auto finale = points[i + 2];
      Triangle tri(cur, next, finale);
      normal = tri.normal();
    }

    m_vertices[p + 5] = normal.x();
    m_vertices[p + 6] = normal.y();
    m_vertices[p + 7] = normal.z();
  }

  m_numElements = m_numVertices * 3;
  m_elements = new ushort[m_numElements];

  for (uint i = 0; i < m_numElements; i++) {
    m_elements[i] = i;
  }

  m_canRenderMutex.lock();
  m_canRender = true;
  m_canRenderMutex.unlock();
}

void Territory::subdivide() {
  QList<QVector3D> points;
  QList<Triangle> mesh;

  for (auto tri : m_mesh) {
    auto pair = tri.split();

    if (pair.first.isClockwise()) pair.first.flip();
    if (pair.second.isClockwise()) pair.second.flip();

    mesh << pair.first << pair.second;

    points
      << pair.first.top()
      << pair.first.left()
      << pair.first.bottom()
      << pair.second.top()
      << pair.second.left()
      << pair.second.bottom()
    ;
  }

  m_canRenderMutex.lock();
  m_canRender = false;
  m_canRenderMutex.unlock();

  m_mesh = mesh;
  m_isInitialized = false;

  m_canRenderMutex.lock();
  m_canRender = true;
  m_canRenderMutex.unlock();

  buildVerticesFromPointList(points);
}

void Territory::intersection(Territory& other) {
  auto otherMesh = other.getMesh();
  QList<Triangle> mesh;

  for (auto tri : m_mesh) {
    bool shouldAdd = false;

    for (auto i = otherMesh.begin(); i != otherMesh.end(); i++) {
      auto otherTri = *i;
      if ((tri.contains(otherTri.bottom()) || tri.contains(otherTri.left()) || tri.contains(otherTri.top())) ||
        (otherTri.contains(tri.bottom()) || otherTri.contains(tri.left()) || otherTri.contains(tri.top()))) {
        shouldAdd = true;
        break;
      }
    }

    if (shouldAdd) {
      mesh << tri;
    }
  }

  QList<QVector3D> newPoints;
  for (auto tri : mesh) {
    newPoints << tri.top() << tri.bottom() << tri.left();
  }

  m_mesh = mesh;
  buildVerticesFromPointList(newPoints);
}

QList<QVector3D> Territory::pointList() {
  auto points = QList<QVector3D>();

  for (auto tri : m_mesh) {
    points << tri.top() << tri.left() << tri.bottom();
  }

  return points;
}

void Territory::writePerlinImage(QString& filename) {
  auto width = 500;
  auto height = 500;
  QImage image(width, height, QImage::Format_ARGB32);
  QPainter painter(&image);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto val = noisedd(QVector2D(x, y) / SCALE);

      QColor color(val, val, val);
      painter.fillRect(x, y, 1, 1, color);
    }
  }

  QImageWriter writer(filename + ".png");
  writer.setFormat("png");
  writer.write(image);
}

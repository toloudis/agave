#include "ViewerState.h"

#include "renderlib/Logging.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>

QJsonArray
jsonVec3(float x, float y, float z)
{
  QJsonArray tgt;
  tgt.append(x);
  tgt.append(y);
  tgt.append(z);
  return tgt;
}
QJsonArray
jsonVec2(float x, float y)
{
  QJsonArray tgt;
  tgt.append(x);
  tgt.append(y);
  return tgt;
}
QJsonArray
jsonVec3(const glm::vec3& v)
{
  QJsonArray tgt;
  tgt.append(v.x);
  tgt.append(v.y);
  tgt.append(v.z);
  return tgt;
}
QJsonArray
jsonVec2(const glm::vec2& v)
{
  QJsonArray tgt;
  tgt.append(v.x);
  tgt.append(v.y);
  return tgt;
}

void
getFloat(QJsonObject obj, QString prop, float& value)
{
  if (obj.contains(prop)) {
    value = (float)obj[prop].toDouble(value);
  }
}
void
getInt(QJsonObject obj, QString prop, int& value)
{
  if (obj.contains(prop)) {
    value = obj[prop].toInt(value);
  }
}
void
getString(QJsonObject obj, QString prop, QString& value)
{
  if (obj.contains(prop)) {
    value = obj[prop].toString(value);
  }
}
void
getBool(QJsonObject obj, QString prop, bool& value)
{
  if (obj.contains(prop)) {
    value = obj[prop].toBool(value);
  }
}
void
getVec3(QJsonObject obj, QString prop, glm::vec3& value)
{
  if (obj.contains(prop)) {
    QJsonArray ja = obj[prop].toArray();
    value.x = ja.at(0).toDouble(value.x);
    value.y = ja.at(1).toDouble(value.y);
    value.z = ja.at(2).toDouble(value.z);
  }
}
void
getVec2(QJsonObject obj, QString prop, glm::vec2& value)
{
  if (obj.contains(prop)) {
    QJsonArray ja = obj[prop].toArray();
    value.x = ja.at(0).toDouble(value.x);
    value.y = ja.at(1).toDouble(value.y);
  }
}
void
getVec2i(QJsonObject obj, QString prop, glm::ivec2& value)
{
  if (obj.contains(prop)) {
    QJsonArray ja = obj[prop].toArray();
    value.x = ja.at(0).toInt(value.x);
    value.y = ja.at(1).toInt(value.y);
  }
}

ViewerState
ViewerState::readStateFromJson(QString filePath)
{
  // defaults from default ctor
  ViewerState p;

  // try to open server.cfg
  QFile loadFile(filePath);
  if (!loadFile.open(QIODevice::ReadOnly)) {
    LOG_DEBUG << "No config file found openable at " << filePath.toStdString();
    return p;
  }

  QByteArray jsonData = loadFile.readAll();
  QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
  if (jsonDoc.isNull()) {
    LOG_DEBUG << "Invalid config file format. Make sure it is json.";
    return p;
  }

  p.stateFromJson(jsonDoc);

  return p;
}

void
ViewerState::stateFromJson(QJsonDocument& jsonDoc)
{
  QJsonObject json(jsonDoc.object());

  glm::vec3 version(0, 0, 1);
  getVec3(json, "version", version);
  // VERSION MUST EXIST.  THROW OR PANIC IF NOT.

  getString(json, "name", m_volumeImageFile);
  getInt(json, "renderIterations", m_renderIterations);
  getFloat(json, "density", m_densityScale);

  glm::ivec2 res(m_resolutionX, m_resolutionY);
  getVec2i(json, "resolution", res);
  m_resolutionX = res.x;
  m_resolutionY = res.y;

  glm::vec3 scale(m_scaleX, m_scaleY, m_scaleZ);
  getVec3(json, "scale", scale);
  m_scaleX = scale.x;
  m_scaleY = scale.y;
  m_scaleZ = scale.z;

  glm::vec3 bgcolor = m_backgroundColor;
  getVec3(json, "backgroundColor", bgcolor);
  m_backgroundColor = bgcolor;

  if (json.contains("clipRegion") && json["clipRegion"].isArray()) {
    QJsonArray ja = json["clipRegion"].toArray();
    QJsonArray crx = ja.at(0).toArray();
    m_roiXmin = crx.at(0).toDouble(m_roiXmin);
    m_roiXmax = crx.at(1).toDouble(m_roiXmax);
    QJsonArray cry = ja.at(1).toArray();
    m_roiYmin = cry.at(0).toDouble(m_roiYmin);
    m_roiYmax = cry.at(1).toDouble(m_roiYmax);
    QJsonArray crz = ja.at(2).toArray();
    m_roiZmin = crz.at(0).toDouble(m_roiZmin);
    m_roiZmax = crz.at(1).toDouble(m_roiZmax);
  }

  if (json.contains("pathTracer") && json["pathTracer"].isObject()) {
    QJsonObject pathTracer = json["pathTracer"].toObject();
    getFloat(pathTracer, "primaryStepSize", m_primaryStepSize);
    getFloat(pathTracer, "secondaryStepSize", m_secondaryStepSize);
  }

  if (json.contains("camera") && json["camera"].isObject()) {
    QJsonObject cam = json["camera"].toObject();
    glm::vec3 tmp;
    getVec3(cam, "eye", tmp);
    m_eyeX = tmp.x;
    m_eyeY = tmp.y;
    m_eyeZ = tmp.z;
    getVec3(cam, "target", tmp);
    m_targetX = tmp.x;
    m_targetY = tmp.y;
    m_targetZ = tmp.z;
    getVec3(cam, "up", tmp);
    m_upX = tmp.x;
    m_upY = tmp.y;
    m_upZ = tmp.z;
    getFloat(cam, "orthoScale", m_orthoScale);
    getFloat(cam, "fovY", m_fov);
    getFloat(cam, "exposure", m_exposure);
    getFloat(cam, "aperture", m_apertureSize);
    getFloat(cam, "focalDistance", m_focalDistance);
    getInt(cam, "projection", m_projection);
  }

  if (json.contains("channels") && json["channels"].isArray()) {
    QJsonArray channelsArray = json["channels"].toArray();
    m_channels.clear();
    m_channels.reserve(channelsArray.size());
    for (int i = 0; i < channelsArray.size(); ++i) {
      ChannelViewerState ch;
      QJsonObject channeli = channelsArray[i].toObject();

      getBool(channeli, "enabled", ch.m_enabled);
      getVec3(channeli, "diffuseColor", ch.m_diffuse);
      getVec3(channeli, "specularColor", ch.m_specular);
      getVec3(channeli, "emissiveColor", ch.m_emissive);
      getFloat(channeli, "glossiness", ch.m_glossiness);
      getFloat(channeli, "opacity", ch.m_opacity);
      getFloat(channeli, "window", ch.m_window);
      getFloat(channeli, "level", ch.m_level);

      QString channelsString = channelsArray[i].toString();
      m_channels.push_back(ch);
    }
  }

  // lights
  if (json.contains("lights") && json["lights"].isArray()) {
    QJsonArray lightsArray = json["lights"].toArray();
    // expect two.
    for (int i = 0; i < std::min(lightsArray.size(), 2); ++i) {
      LightViewerState& ls = (i == 0) ? m_light0 : m_light1;
      QJsonObject lighti = lightsArray[i].toObject();
      getInt(lighti, "type", ls.m_type);
      getVec3(lighti, "topColor", ls.m_topColor);
      getVec3(lighti, "middleColor", ls.m_middleColor);
      getVec3(lighti, "color", ls.m_color);
      getVec3(lighti, "bottomColor", ls.m_bottomColor);
      getFloat(lighti, "topColorIntensity", ls.m_topColorIntensity);
      getFloat(lighti, "middleColorIntensity", ls.m_middleColorIntensity);
      getFloat(lighti, "colorIntensity", ls.m_colorIntensity);
      getFloat(lighti, "bottomColorIntensity", ls.m_bottomColorIntensity);
      getFloat(lighti, "distance", ls.m_distance);
      getFloat(lighti, "theta", ls.m_theta);
      getFloat(lighti, "phi", ls.m_phi);
      getFloat(lighti, "width", ls.m_width);
      getFloat(lighti, "height", ls.m_height);
    }
  }
}

QJsonDocument
ViewerState::stateToJson() const
{
  // fire back some json...
  QJsonObject j;
  j["name"] = m_volumeImageFile;

  // the version of this schema
  j["version"] = jsonVec3(1, 0, 0);

  QJsonArray resolution;
  resolution.append(m_resolutionX);
  resolution.append(m_resolutionY);
  j["resolution"] = resolution;

  j["renderIterations"] = m_renderIterations;

  QJsonObject pathTracer;
  pathTracer["primaryStepSize"] = m_primaryStepSize;
  pathTracer["secondaryStepSize"] = m_secondaryStepSize;
  j["pathTracer"] = pathTracer;

  QJsonArray clipRegion;
  QJsonArray clipRegionX;
  clipRegionX.append(m_roiXmin);
  clipRegionX.append(m_roiXmax);
  QJsonArray clipRegionY;
  clipRegionY.append(m_roiYmin);
  clipRegionY.append(m_roiYmax);
  QJsonArray clipRegionZ;
  clipRegionZ.append(m_roiZmin);
  clipRegionZ.append(m_roiZmax);
  clipRegion.append(clipRegionX);
  clipRegion.append(clipRegionY);
  clipRegion.append(clipRegionZ);

  j["clipRegion"] = clipRegion;

  j["scale"] = jsonVec3(m_scaleX, m_scaleY, m_scaleZ);

  QJsonObject camera;
  camera["eye"] = jsonVec3(m_eyeX, m_eyeY, m_eyeZ);
  camera["target"] = jsonVec3(m_targetX, m_targetY, m_targetZ);
  camera["up"] = jsonVec3(m_upX, m_upY, m_upZ);

  camera["projection"] = m_projection;
  camera["fovY"] = m_fov;
  camera["orthoScale"] = m_orthoScale;

  camera["exposure"] = m_exposure;
  camera["aperture"] = m_apertureSize;
  camera["focalDistance"] = m_focalDistance;
  j["camera"] = camera;

  j["backgroundColor"] = jsonVec3(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z);

  QJsonArray channels;
  for (auto ch : m_channels) {
    QJsonObject channel;
    channel["enabled"] = ch.m_enabled;
    channel["diffuseColor"] = jsonVec3(ch.m_diffuse.x, ch.m_diffuse.y, ch.m_diffuse.z);
    channel["specularColor"] = jsonVec3(ch.m_specular.x, ch.m_specular.y, ch.m_specular.z);
    channel["emissiveColor"] = jsonVec3(ch.m_emissive.x, ch.m_emissive.y, ch.m_emissive.z);
    channel["glossiness"] = ch.m_glossiness;
    channel["opacity"] = ch.m_opacity;
    channel["window"] = ch.m_window;
    channel["level"] = ch.m_level;

    channels.append(channel);
  }
  j["channels"] = channels;

  j["density"] = m_densityScale;

  // lighting
  QJsonArray lights;
  QJsonObject light0;
  light0["type"] = m_light0.m_type;
  light0["distance"] = m_light0.m_distance;
  light0["theta"] = m_light0.m_theta;
  light0["phi"] = m_light0.m_phi;
  light0["color"] = jsonVec3(m_light0.m_color.r, m_light0.m_color.g, m_light0.m_color.b);
  light0["colorIntensity"] = m_light0.m_colorIntensity;
  light0["topColor"] = jsonVec3(m_light0.m_topColor.r, m_light0.m_topColor.g, m_light0.m_topColor.b);
  light0["topColorIntensity"] = m_light0.m_topColorIntensity;
  light0["middleColor"] = jsonVec3(m_light0.m_middleColor.r, m_light0.m_middleColor.g, m_light0.m_middleColor.b);
  light0["middleColorIntensity"] = m_light0.m_middleColorIntensity;
  light0["bottomColor"] = jsonVec3(m_light0.m_bottomColor.r, m_light0.m_bottomColor.g, m_light0.m_bottomColor.b);
  light0["bottomColorIntensity"] = m_light0.m_bottomColorIntensity;
  light0["width"] = m_light0.m_width;
  light0["height"] = m_light0.m_height;
  lights.append(light0);

  QJsonObject light1;
  light1["type"] = m_light1.m_type;
  light1["distance"] = m_light1.m_distance;
  light1["theta"] = m_light1.m_theta;
  light1["phi"] = m_light1.m_phi;
  light1["color"] = jsonVec3(m_light1.m_color.r, m_light1.m_color.g, m_light1.m_color.b);
  light1["colorIntensity"] = m_light1.m_colorIntensity;
  light1["topColor"] = jsonVec3(m_light1.m_topColor.r, m_light1.m_topColor.g, m_light1.m_topColor.b);
  light1["topColorIntensity"] = m_light1.m_topColorIntensity;
  light1["middleColor"] = jsonVec3(m_light1.m_middleColor.r, m_light1.m_middleColor.g, m_light1.m_middleColor.b);
  light1["middleColorIntensity"] = m_light1.m_middleColorIntensity;
  light1["bottomColor"] = jsonVec3(m_light1.m_bottomColor.r, m_light1.m_bottomColor.g, m_light1.m_bottomColor.b);
  light1["bottomColorIntensity"] = m_light1.m_bottomColorIntensity;
  light1["width"] = m_light1.m_width;
  light1["height"] = m_light1.m_height;
  lights.append(light1);
  j["lights"] = lights;

  return QJsonDocument(j);
}

void
ViewerState::writeStateToJson(QString filePath, const ViewerState& state)
{
  QJsonDocument d = state.stateToJson();
}

QString
ViewerState::stateToPythonScript() const
{
  {
    QFileInfo fi(m_volumeImageFile);
    QString outFileName = fi.baseName();

    QString s;
    s += QString("import agaveclient\n\n\n");
    s += QString("def renderfunc(server):\n");
    s += QString("    server.render_frame(\n");
    s += QString("        [\n");

    QString indent("            ");
    s += indent + QString("(\"LOAD_OME_TIF\", \"%1\"),\n").arg(m_volumeImageFile);
    s += indent + QString("(\"SET_RESOLUTION\", %1, %2),\n").arg(m_resolutionX).arg(m_resolutionY);
    s += indent + QString("(\"BACKGROUND_COLOR\", %1, %2, %3),\n")
                    .arg(m_backgroundColor.x)
                    .arg(m_backgroundColor.y)
                    .arg(m_backgroundColor.z);
    s += indent + QString("(\"RENDER_ITERATIONS\", %1),\n").arg(m_renderIterations);
    s += indent + QString("(\"SET_PRIMARY_RAY_STEP_SIZE\", %1),\n").arg(m_primaryStepSize);
    s += indent + QString("(\"SET_SECONDARY_RAY_STEP_SIZE\", %1),\n").arg(m_secondaryStepSize);
    s += indent + QString("(\"SET_VOXEL_SCALE\", %1, %2, %3),\n").arg(m_scaleX).arg(m_scaleY).arg(m_scaleZ);
    s += indent + QString("(\"SET_CLIP_REGION\", %1, %2, %3, %4, %5, %6),\n")
                    .arg(m_roiXmin)
                    .arg(m_roiXmax)
                    .arg(m_roiYmin)
                    .arg(m_roiYmax)
                    .arg(m_roiZmin)
                    .arg(m_roiZmax);

    s += indent + QString("(\"EYE\", %1, %2, %3),\n").arg(m_eyeX).arg(m_eyeY).arg(m_eyeZ);
    s += indent + QString("(\"TARGET\", %1, %2, %3),\n").arg(m_targetX).arg(m_targetY).arg(m_targetZ);
    s += indent + QString("(\"UP\", %1, %2, %3),\n").arg(m_upX).arg(m_upY).arg(m_upZ);
    s += indent + QString("(\"CAMERA_PROJECTION\", %1, %2),\n")
                    .arg(m_projection)
                    .arg(m_projection == Projection::PERSPECTIVE ? m_fov : m_orthoScale);

    s += indent + QString("(\"EXPOSURE\", %1),\n").arg(m_exposure);
    s += indent + QString("(\"DENSITY\", %1),\n").arg(m_densityScale);
    s += indent + QString("(\"APERTURE\", %1),\n").arg(m_apertureSize);
    s += indent + QString("(\"FOCALDIST\", %1),\n").arg(m_focalDistance);

    // per-channel
    for (std::size_t i = 0; i < m_channels.size(); ++i) {
      const ChannelViewerState& ch = m_channels[i];
      s += indent + QString("(\"ENABLE_CHANNEL\", %1, %2),\n").arg(QString::number(i), ch.m_enabled ? "1" : "0");
      s += indent + QString("(\"MAT_DIFFUSE\", %1, %2, %3, %4, 1.0),\n")
                      .arg(QString::number(i))
                      .arg(ch.m_diffuse.x)
                      .arg(ch.m_diffuse.y)
                      .arg(ch.m_diffuse.z);
      s += indent + QString("(\"MAT_SPECULAR\", %1, %2, %3, %4, 0.0),\n")
                      .arg(QString::number(i))
                      .arg(ch.m_specular.x)
                      .arg(ch.m_specular.y)
                      .arg(ch.m_specular.z);
      s += indent + QString("(\"MAT_EMISSIVE\", %1, %2, %3, %4, 0.0),\n")
                      .arg(QString::number(i))
                      .arg(ch.m_emissive.x)
                      .arg(ch.m_emissive.y)
                      .arg(ch.m_emissive.z);
      s += indent + QString("(\"MAT_GLOSSINESS\", %1, %2),\n").arg(QString::number(i)).arg(ch.m_glossiness);
      s += indent + QString("(\"MAT_OPACITY\", %1, %2),\n").arg(QString::number(i)).arg(ch.m_opacity);
      s += indent +
           QString("(\"SET_WINDOW_LEVEL\", %1, %2, %3),\n").arg(QString::number(i)).arg(ch.m_window).arg(ch.m_level);
    }

    // lighting
    s += indent + QString("(\"SKYLIGHT_TOP_COLOR\", %1, %2, %3),\n")
                    .arg(m_light0.m_topColor.r * m_light0.m_topColorIntensity)
                    .arg(m_light0.m_topColor.g * m_light0.m_topColorIntensity)
                    .arg(m_light0.m_topColor.b * m_light0.m_topColorIntensity);
    s += indent + QString("(\"SKYLIGHT_MIDDLE_COLOR\", %1, %2, %3),\n")
                    .arg(m_light0.m_middleColor.r * m_light0.m_middleColorIntensity)
                    .arg(m_light0.m_middleColor.g * m_light0.m_middleColorIntensity)
                    .arg(m_light0.m_middleColor.b * m_light0.m_middleColorIntensity);
    s += indent + QString("(\"SKYLIGHT_BOTTOM_COLOR\", %1, %2, %3),\n")
                    .arg(m_light0.m_bottomColor.r * m_light0.m_bottomColorIntensity)
                    .arg(m_light0.m_bottomColor.g * m_light0.m_bottomColorIntensity)
                    .arg(m_light0.m_bottomColor.b * m_light0.m_bottomColorIntensity);
    s +=
      indent +
      QString("(\"LIGHT_POS\", 0, %1, %2, %3),\n").arg(m_light1.m_distance).arg(m_light1.m_theta).arg(m_light1.m_phi);
    s += indent + QString("(\"LIGHT_COLOR\", 0, %1, %2, %3),\n")
                    .arg(m_light1.m_color.r * m_light1.m_colorIntensity)
                    .arg(m_light1.m_color.g * m_light1.m_colorIntensity)
                    .arg(m_light1.m_color.b * m_light1.m_colorIntensity);
    s += indent + QString("(\"LIGHT_SIZE\", 0, %1, %2),\n").arg(m_light1.m_width).arg(m_light1.m_height);

    s += QString("        ],\n");
    s += QString("        output_name=\"%1\",\n").arg(outFileName);
    s += QString("    )\n");
    s += QString("\n");
    s += QString("agaveclient.agaveclient(renderfunc=renderfunc)\n");

    // LOG_DEBUG << s.toStdString();
    return s;
  }
}

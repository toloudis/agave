#include "FileReader.h"

#include "BoundingBox.h"
#include "ImageXYZC.h"
#include "Logging.h"

#include <QDomDocument>
#include <QElapsedTimer>
#include <QString>
#include <QtDebug>

#include <tiff.h>
#include <tiffio.h>

#include <map>

std::map<std::string, std::shared_ptr<ImageXYZC>> FileReader::sPreloadedImageCache;

FileReader::FileReader() {}

FileReader::~FileReader() {}

uint32_t
requireUint32Attr(QDomElement& el, const QString& attr, uint32_t defaultVal)
{
  QString attrval = el.attribute(attr);
  bool ok;
  uint32_t retval = attrval.toUInt(&ok);
  if (!ok) {
    retval = defaultVal;
  }
  return retval;
}
float
requireFloatAttr(QDomElement& el, const QString& attr, float defaultVal)
{
  QString attrval = el.attribute(attr);
  bool ok;
  float retval = attrval.toFloat(&ok);
  if (!ok) {
    retval = defaultVal;
  }
  return retval;
}

void
FileReader::getZCT(uint32_t i,
                   QString dimensionOrder,
                   uint32_t sizeZ,
                   uint32_t sizeC,
                   uint32_t sizeT,
                   uint32_t& z,
                   uint32_t& c,
                   uint32_t& t)
{
  // assume t = 0 for everything.
  assert(sizeT == 1);
  QString order = dimensionOrder.remove("XY");
  t = 0;
  if (order == "CTZ") {
    c = i % (sizeC);
    z = i / sizeC;
  } else if (order == "CZT") {
    c = i % (sizeC);
    z = i / sizeC;
  } else if (order == "ZCT") {
    c = i / (sizeZ);
    z = i % sizeZ;
  } else if (order == "ZTC") {
    c = i / (sizeZ);
    z = i % sizeZ;
  } else if (order == "TCZ") {
    c = i % (sizeC);
    z = i / sizeC;
  } else if (order == "TZC") {
    c = i / (sizeZ);
    z = i % sizeZ;
  }
}

std::shared_ptr<ImageXYZC>
FileReader::loadOMETiff_4D(const std::string& filepath, bool addToCache)
{
  // check cache first of all.
  auto cached = sPreloadedImageCache.find(filepath);
  if (cached != sPreloadedImageCache.end()) {
    return cached->second;
  }

  std::shared_ptr<ImageXYZC> emptyimage;

  QElapsedTimer twhole;
  twhole.start();

  QElapsedTimer timer;
  timer.start();

  // Loads tiff file
  TIFF* tiff = TIFFOpen(filepath.c_str(), "r");
  if (!tiff) {
    QString msg = "Failed to open TIFF: '" + QString(filepath.c_str()) + "'";
    LOG_ERROR << msg.toStdString();
    // throw new Exception(NULL, msg, this, __FUNCTION__, __LINE__);
    return emptyimage;
  }

  char* omexmlstr = nullptr;
  // ome-xml is in ImageDescription of first IFD in the file.
  if (TIFFGetField(tiff, TIFFTAG_IMAGEDESCRIPTION, &omexmlstr) != 1) {
    QString msg = "Failed to read imagedescription of TIFF: '" + QString(filepath.c_str()) + "'";
    LOG_ERROR << msg.toStdString();

    // throw new Exception(NULL, msg, this, __FUNCTION__, __LINE__);
    return emptyimage;
  }

  // Temporary variables
  uint32 width, height;
  //  tsize_t scanlength;

  // Read dimensions of image
  if (TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width) != 1) {
    QString msg = "Failed to read width of TIFF: '" + QString(filepath.c_str()) + "'";
    LOG_ERROR << msg.toStdString();
    // throw new Exception(NULL, msg, this, __FUNCTION__, __LINE__);
    return emptyimage;
  }
  if (TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height) != 1) {
    QString msg = "Failed to read height of TIFF: '" + QString(filepath.c_str()) + "'";
    LOG_ERROR << msg.toStdString();
    // throw new Exception(NULL, msg, this, __FUNCTION__, __LINE__);
    return emptyimage;
  }

  uint32_t bpp = 0;
  if (TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bpp) != 1) {
    QString msg = "Failed to read bpp of TIFF: '" + QString(filepath.c_str()) + "'";
    LOG_ERROR << msg.toStdString();
    // throw new Exception(NULL, msg, this, __FUNCTION__, __LINE__);
    return emptyimage;
  }

  uint16_t sampleFormat = 0;
  if (TIFFGetField(tiff, TIFFTAG_SAMPLEFORMAT, &sampleFormat) != 1) {
    QString msg = "Failed to read sampleformat of TIFF: '" + QString(filepath.c_str()) + "'";
    LOG_ERROR << msg.toStdString();
    // throw new Exception(NULL, msg, this, __FUNCTION__, __LINE__);
    return emptyimage;
  }

  uint32_t sizeT = 1;
  uint32_t sizeX = width;
  uint32_t sizeY = height;
  uint32_t sizeZ = 1;
  uint32_t sizeC = 1;
  float physicalSizeX = 1.0f;
  float physicalSizeY = 1.0f;
  float physicalSizeZ = 1.0f;
  std::vector<QString> channelNames;
  QString dimensionOrder = "XYCZT";

  // check for plain tiff with ImageJ imagedescription:
  QString qomexmlstr(omexmlstr);
  if (qomexmlstr.startsWith("ImageJ=")) {
    // "ImageJ=\nhyperstack=true\nimages=7900\nchannels=1\nslices=50\nframes=158"
    // "ImageJ=1.52i\nimages=126\nchannels=2\nslices=63\nhyperstack=true\nmode=composite\nunit=
    //      micron\nfinterval=299.2315368652344\nspacing=0.2245383462882669\nloop=false\nmin=9768.0\nmax=
    //        14591.0\n"
    QStringList sl = qomexmlstr.split('\n');
    // split each string into name/value pairs,
    // then look up as a map.
    QMap<QString, QString> imagejmetadata;
    for (int i = 0; i < sl.size(); ++i) {
      QStringList namevalue = sl.at(i).split('=');
      if (namevalue.size() == 2) {
        imagejmetadata.insert(namevalue[0], namevalue[1]);
      } else if (namevalue.size() == 1) {
        imagejmetadata.insert(namevalue[0], "");
      } else {
        QString msg = "Unexpected name/value pair in TIFF ImageJ metadata: " + sl.at(i);
        LOG_ERROR << msg.toStdString();
        return emptyimage;
      }
    }

    if (imagejmetadata.contains("channels")) {
      QString value = imagejmetadata.value("channels");
      sizeC = value.toInt();
    } else {
      QString msg = "Failed to read number of channels of ImageJ TIFF: '" + QString(filepath.c_str()) + "'";
      LOG_WARNING << msg.toStdString();
    }

    if (imagejmetadata.contains("slices")) {
      QString value = imagejmetadata.value("slices");
      sizeZ = value.toInt();
    } else {
      QString msg = "Failed to read number of slices of ImageJ TIFF: '" + QString(filepath.c_str()) + "'";
      LOG_WARNING << msg.toStdString();
    }

    if (imagejmetadata.contains("spacing")) {
      QString value = imagejmetadata.value("spacing");
      bool ok;
      physicalSizeZ = value.toFloat(&ok);
      if (!ok) {
        QString msg = "Failed to read spacing of ImageJ TIFF: '" + QString(filepath.c_str()) + "'";
        LOG_WARNING << msg.toStdString();
        physicalSizeZ = 1.0f;
      } else {
        if (physicalSizeZ < 0.0f) {
          physicalSizeZ = -physicalSizeZ;
        }
        // WHY?  NEED TO FIGURE OUT UNITS HERE.
        physicalSizeX = 0.1f;
        physicalSizeY = 0.1f;
      }
    }

    for (uint32_t i = 0; i < sizeC; ++i) {
      channelNames.push_back(QString::number(i));
    }
  } else if (qomexmlstr.startsWith("{\"shape\":")) {
    // expect a 4d shape array of C,Z,Y,X
    int firstBracket = qomexmlstr.indexOf('[');
    int lastBracket = qomexmlstr.lastIndexOf(']');
    QString shape = qomexmlstr.mid(firstBracket + 1, lastBracket - firstBracket - 1);
    LOG_INFO << shape.toStdString();
    QStringList shapelist = shape.split(',');
    assert(shapelist.size() == 4);
    if (shapelist.size() != 4) {
      QString msg = "Expected shape to be 4D TIFF: '" + QString(filepath.c_str()) + "'";
      LOG_ERROR << msg.toStdString();
      return emptyimage;
    }
    dimensionOrder = "XYZCT";
    sizeX = shapelist[3].toInt();
    sizeY = shapelist[2].toInt();
    sizeZ = shapelist[1].toInt();
    sizeC = shapelist[0].toInt();
    for (uint32_t i = 0; i < sizeC; ++i) {
      channelNames.push_back(QString("%1").arg(i));
    }

  } else if (qomexmlstr.startsWith("<?xml version") && qomexmlstr.endsWith("OME>")) {
    // convert c to xml doc.  if this fails then we don't have an ome tif.
    QDomDocument omexml;
    bool ok = omexml.setContent(qomexmlstr);
    if (!ok) {
      QString msg = "Bad ome xml content";
      LOG_ERROR << msg.toStdString();
      // throw new Exception(NULL, msg, this, __FUNCTION__, __LINE__);
      return emptyimage;
    }

    // extract some necessary info from the xml:
    QDomElement pixelsEl = omexml.elementsByTagName("Pixels").at(0).toElement();
    if (pixelsEl.isNull()) {
      QString msg = "No <Pixels> element in ome xml";
      LOG_ERROR << msg.toStdString();
      return emptyimage;
    }

    // skipping "complex", "double-complex", and "bit".
    std::map<std::string, uint32_t> mapPixelTypeBPP = { { "uint8", 8 },  { "uint16", 16 }, { "uint32", 32 },
                                                        { "int8", 8 },   { "int16", 16 },  { "int32", 32 },
                                                        { "float", 32 }, { "double", 64 } };

    QString pixelType = pixelsEl.attribute("Type", "uint16").toLower();
    LOG_INFO << "pixel type: " << pixelType.toStdString();
    bpp = mapPixelTypeBPP[pixelType.toStdString()];
    if (bpp != 16) {
      LOG_ERROR << "Image must be 16-bit";
      return emptyimage;
    }
    sizeX = requireUint32Attr(pixelsEl, "SizeX", 0);
    sizeY = requireUint32Attr(pixelsEl, "SizeY", 0);
    sizeZ = requireUint32Attr(pixelsEl, "SizeZ", 0);
    sizeC = requireUint32Attr(pixelsEl, "SizeC", 0);
    sizeT = requireUint32Attr(pixelsEl, "SizeT", 0);
    // one of : "XYZCT", "XYZTC","XYCTZ","XYCZT","XYTCZ","XYTZC"
    dimensionOrder = pixelsEl.attribute("DimensionOrder", dimensionOrder);
    physicalSizeX = requireFloatAttr(pixelsEl, "PhysicalSizeX", 1.0f);
    physicalSizeY = requireFloatAttr(pixelsEl, "PhysicalSizeY", 1.0f);
    physicalSizeZ = requireFloatAttr(pixelsEl, "PhysicalSizeZ", 1.0f);
    QString physicalSizeXunit = pixelsEl.attribute("PhysicalSizeXUnit", "");
    QString physicalSizeYunit = pixelsEl.attribute("PhysicalSizeYUnit", "");
    QString physicalSizeZunit = pixelsEl.attribute("PhysicalSizeZUnit", "");

    // find channel names
    QDomNodeList channels = omexml.elementsByTagName("Channel");
    for (int i = 0; i < channels.length(); ++i) {
      QDomNode dn = channels.at(i);
      QDomElement chel = dn.toElement();
      QString chid = chel.attribute("ID");
      QString chname = chel.attribute("Name");
      if (!chname.isEmpty()) {
        channelNames.push_back(chname);
      } else if (!chid.isEmpty()) {
        channelNames.push_back(chid);
      } else {
        channelNames.push_back(QString("%1").arg(i));
      }
    }
  } else {
    // unrecognized string / no metadata.
    // walk the file and count the directories and assume that is Z
    // sizeZ was initialized to 1.
    sizeZ = 0;
    while (TIFFSetDirectory(tiff, sizeZ)) {
      sizeZ++;
    };
    channelNames.push_back("0");
  }

  assert(sizeX == width);
  assert(sizeY == height);

  // allocate the destination buffer!!!!
  assert(sizeC >= 1);
  assert(sizeX >= 1);
  assert(sizeY >= 1);
  assert(sizeZ >= 1);
  size_t planesize = sizeX * sizeY * bpp / 8;
  uint8_t* data = new uint8_t[planesize * sizeZ * sizeC];
  memset(data, 0, planesize * sizeZ * sizeC);
  // stash it here in case of early exit, it will be deleted
  std::unique_ptr<uint8_t[]> smartPtr(data);

  uint8_t* destptr = data;

  // use dimensionOrderZCT to determine whether Z or C comes first!
  // assume XY are not transposed.
  QString dimensionOrderZCT = dimensionOrder.remove("XY");

  if (TIFFIsTiled(tiff)) {
    tsize_t tilesize = TIFFTileSize(tiff);
    uint32 ntiles = TIFFNumberOfTiles(tiff);
    assert(ntiles == 1);
    // assuming ntiles == 1 for all IFDs
    tdata_t buf = _TIFFmalloc(tilesize);
    for (uint32_t i = 0; i < sizeZ * sizeC; ++i) {
      int setdirok = TIFFSetDirectory(tiff, i);
      if (setdirok == 0) {
        LOG_ERROR << "Bad tiff directory specified: " << (i);
        return emptyimage;
      }
      int readtileok = TIFFReadEncodedTile(tiff, 0, buf, tilesize);
      if (readtileok < 0) {
        LOG_ERROR << "Error reading tiff tile";
        return emptyimage;
      }
      // copy buf into data.
      uint32_t t = 0;
      uint32_t z = 0;
      uint32_t c = 0;
      getZCT(i, dimensionOrderZCT, sizeZ, sizeC, sizeT, z, c, t);
      uint32_t planeindexinbuffer = c * sizeZ + z;
      destptr = data + (planesize * (planeindexinbuffer));
      memcpy(destptr, buf, readtileok);
    }
    _TIFFfree(buf);
  } else {
    // stripped.
    // Number of bytes in a decoded scanline
    tsize_t striplength = TIFFStripSize(tiff);
    tdata_t buf = _TIFFmalloc(striplength);

    for (uint32_t i = 0; i < sizeZ * sizeC; ++i) {
      uint32_t planeindexintiff = i;
      int setdirok = TIFFSetDirectory(tiff, planeindexintiff);
      if (setdirok == 0) {
        LOG_ERROR << "Bad tiff directory specified: " << (i);
        return emptyimage;
      }
      // ensure channels are coalesced (transposing from xycz to xyzc)
      uint32_t t = 0;
      uint32_t z = 0;
      uint32_t c = 0;
      getZCT(i, dimensionOrderZCT, sizeZ, sizeC, sizeT, z, c, t);
      uint32_t planeindexinbuffer = c * sizeZ + z;
      destptr = data + (planesize * (planeindexinbuffer));
      uint32 nstrips = TIFFNumberOfStrips(tiff);
      for (tstrip_t strip = 0; strip < nstrips; strip++) {
        int readstripok = TIFFReadEncodedStrip(tiff, strip, buf, striplength);
        if (readstripok < 0) {
          LOG_ERROR << "Error reading tiff strip";
          return emptyimage;
        }

        // copy buf into data.
        memcpy(destptr, buf, readstripok);
        destptr += readstripok;
      }
    }
    _TIFFfree(buf);
  }

  TIFFClose(tiff);

  LOG_DEBUG << "TIFF loaded in " << timer.elapsed() << "ms";

  // TODO: convert data to uint16_t pixels.

  timer.start();
  // we can release the smartPtr because ImageXYZC will now own the raw data memory
  ImageXYZC* im = new ImageXYZC(
    sizeX, sizeY, sizeZ, sizeC, uint32_t(bpp), smartPtr.release(), physicalSizeX, physicalSizeY, physicalSizeZ);
  LOG_DEBUG << "ImageXYZC prepared in " << timer.elapsed() << "ms";

  im->setChannelNames(channelNames);

  LOG_DEBUG << "Loaded " << filepath << " in " << twhole.elapsed() << "ms";

  std::shared_ptr<ImageXYZC> sharedImage(im);
  if (addToCache) {
    sPreloadedImageCache[filepath] = sharedImage;
  }
  return sharedImage;
}

std::shared_ptr<ImageXYZC>
FileReader::loadFromArray_4D(uint8_t* dataArray,
                             std::vector<uint32_t> shape,
                             const std::string& name,
                             std::vector<char> dims,
                             std::vector<std::string> channelNames,
                             std::vector<float> physicalSizes,
                             bool addToCache)
{
  // check cache first of all.
  auto cached = sPreloadedImageCache.find(name);
  if (cached != sPreloadedImageCache.end()) {
    return cached->second;
  }

  // assume data is in CZYX order:
  static const int XDIM = 3, YDIM = 2, ZDIM = 1, CDIM = 0;

  size_t ndim = shape.size();
  assert(ndim == 4);

  uint32_t bpp = 16;
  uint32_t sizeT = 1;
  uint32_t sizeX = shape[XDIM];
  uint32_t sizeY = shape[YDIM];
  uint32_t sizeZ = shape[ZDIM];
  uint32_t sizeC = shape[CDIM];
  assert(physicalSizes.size() == 3);
  float physicalSizeX = physicalSizes[0];
  float physicalSizeY = physicalSizes[1];
  float physicalSizeZ = physicalSizes[2];

  // product of all shape elements must equal number of elements in dataArray
  // dims must either be empty or must be of same length as shape, and end in (Y, X), and start with CZ or ZC or Z ?

  QElapsedTimer timer;
  timer.start();

  // note that im will take ownership of dataArray
  ImageXYZC* im =
    new ImageXYZC(sizeX, sizeY, sizeZ, sizeC, uint32_t(bpp), dataArray, physicalSizeX, physicalSizeY, physicalSizeZ);
  LOG_DEBUG << "ImageXYZC prepared in " << timer.elapsed() << "ms";

  std::vector<QString> qchannelNames;
  for (auto name : channelNames) {
    qchannelNames.push_back(QString::fromStdString(name));
  }
  im->setChannelNames(qchannelNames);

  std::shared_ptr<ImageXYZC> sharedImage(im);
  if (addToCache) {
    sPreloadedImageCache[name] = sharedImage;
  }
  return sharedImage;
}

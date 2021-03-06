#pragma once

#include "Controls.h"
#include "Projection.h"

#include <QGroupBox>

class QCamera;
class RenderSettings;

class QProjectionWidget : public QGroupBox
{
  Q_OBJECT

public:
  QProjectionWidget(QWidget* pParent = NULL, QCamera* cam = nullptr, RenderSettings* rs = nullptr);

private slots:
  void SetFieldOfView(const double& FieldOfView);
  void OnProjectionChanged(const QProjection& Film);

private:
  QFormLayout m_Layout;
  QNumericSlider m_FieldOfViewSlider;

  QCamera* m_qcamera;
};

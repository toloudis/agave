#include "Stable.h"

#include "Scene.h"
#include "ImageXYZC.h"

CScene::CScene(void) :
	//_channel(0),
	m_Camera(),
	//m_Lighting(),
	m_Resolution(),
	m_DirtyFlags(),
	m_Spacing(),
	m_Scale(),
	m_BoundingBox(),
	//m_TransferFunctions(),
	//m_IntensityRange(),
	//m_GradientMagnitudeRange(),
	m_DenoiseParams(),
	m_NoIterations(0)
{
}

CScene::CScene(const CScene& Other)
{
	*this = Other;
}

HO CScene& CScene::operator=(const CScene& Other)
{
	m_Camera					= Other.m_Camera;
	//m_Lighting					= Other.m_Lighting;
	m_Resolution				= Other.m_Resolution;
	m_DirtyFlags				= Other.m_DirtyFlags;
	m_Spacing					= Other.m_Spacing;
	m_Scale						= Other.m_Scale;
	m_BoundingBox				= Other.m_BoundingBox;
	//m_TransferFunctions			= Other.m_TransferFunctions;
	//m_IntensityRange			= Other.m_IntensityRange;
	//m_DensityScale				= Other.m_DensityScale;
	m_DenoiseParams				= Other.m_DenoiseParams;
	m_NoIterations				= Other.m_NoIterations;
	m_RenderSettings = Other.m_RenderSettings;
	//m_ShadingType				= Other.m_ShadingType;
	//m_StepSizeFactor			= Other.m_StepSizeFactor;
	//m_StepSizeFactorShadow		= Other.m_StepSizeFactorShadow;
	//m_GradientDelta				= Other.m_GradientDelta;
	//m_GradientMagnitudeRange	= Other.m_GradientMagnitudeRange;
	//m_GradientFactor			= Other.m_GradientFactor;

	return *this;
}

void CScene::initSceneFromImg(uint32_t vx, uint32_t vy, uint32_t vz, float sx, float sy, float sz)
{
	m_Resolution.SetResX(vx);
	m_Resolution.SetResY(vy);
	m_Resolution.SetResZ(vz);
	m_Spacing.x = sx;
	m_Spacing.y = sy;
	m_Spacing.z = sz;

	//Log("Spacing: " + FormatSize(gScene.m_Spacing, 2), "grid");

	// Compute physical size
	const Vec3f PhysicalSize(Vec3f(
		m_Spacing.x * (float)m_Resolution.GetResX(),
		m_Spacing.y * (float)m_Resolution.GetResY(),
		m_Spacing.z * (float)m_Resolution.GetResZ()
	));

	// Compute the volume's bounding box
	m_BoundingBox.m_MinP = Vec3f(0.0f);
	m_BoundingBox.m_MaxP = PhysicalSize / PhysicalSize.Max();

	m_Camera.m_SceneBoundingBox = m_BoundingBox;

//	for (int i = 0; i < m_Lighting.m_NoLights; ++i) {
//		m_Lighting.m_Lights[i].Update(m_BoundingBox);
//	}
}


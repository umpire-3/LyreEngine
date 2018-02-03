#pragma once

#include "SpherifiedCube.h"
#include "GeometryDX.h"
#include "ConstantBufferDX.h"
#include "PipelineConfigDX.h"

#define MAX_CBUFFERS_AMOUNT D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT
#define MAX_SAMPLERS_AMOUNT D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT
#define MAX_SRVS_AMOUNT D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT
#define MAX_SOBUFFERS_AMOUNT D3D11_SO_BUFFER_SLOT_COUNT

class Planet final {
private:
	static const size_t SLERP_LOOKUP_RESOLUTION = 1024;
	static const size_t SLERP_LOOKUP_STEPS = 10;
	//first step - angle=asin(1/sqrt(3)) (initial cube)
	//each step divides angle by two
	//last step - slerp=lerp
	static std::array<std::array<float, SLERP_LOOKUP_RESOLUTION>, SLERP_LOOKUP_STEPS> s_slerpLookup;

	PipelineConfigDX					m_renderConfig;

	//Pipelines
	struct {
		GeometryDX						geometry;
		PipelineConfigDX				config;
	} m_geometryPipeline;

	struct {
		GeometryDX						geometry;
		PipelineConfigDX				config;
	} m_normalsPipeline;

	//Geometry
	GeometryDX							m_geometry;

	//ConstantBuffers
	struct PlanetCB {
		DirectX::XMFLOAT3 planetPos;
		float radius;
	};
	ConstantBufferDX<PlanetCB>			m_planetCb;

	CComPtr<ID3D11ShaderResourceView>	m_iTerrainSRV = nullptr;

	HRESULT setupStreamOutputBuffers();
	HRESULT initGeometryShader();

	HRESULT initGeometryPipeline();
	HRESULT initNormalsPipeline();

	HRESULT initGeometryAndVS();

	SpherifiedCube						m_sphere;

public:
	Planet(float radius, unsigned seed);
	HRESULT init();
	void render();
	void renderGeometry();
	void renderNormals();
};

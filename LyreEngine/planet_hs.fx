cbuffer Lod : register(b0) {
	float MinDistance;
	float MaxDistance;
	float MinLOD;
	float MaxLOD;
}

cbuffer Camera : register(b1) {
	float3 Position;
	float _dummy_;
}

cbuffer Planet : register(b2) {
	float3 PlanetPos;
	float Radius;
}

struct HS_INPUT {
	float3 pos : POSITION;
};

#define EquallyDivided 0
#define MoreDivided 1
#define LessDivided 2

struct PER_INSTANCE_DATA {
	uint data[4];
};

StructuredBuffer<PER_INSTANCE_DATA> NeighboursDivisionData : register(t0);

struct HS_OUTPUT {
	float3 pos : POSITION;
};

struct HSCF_OUTPUT {
	float edge[4]  :  SV_TessFactor;
	float inside[2]  :  SV_InsideTessFactor;
};

float ComputePatchLOD(float3 midPoint) {
	float dist = length(PlanetPos + midPoint - Position);
	if (dist > MaxDistance) return 0.f;
	float d = 1.f - (clamp(dist, MinDistance, MaxDistance) - MinDistance) / (MaxDistance - MinDistance);
	return lerp(MinLOD, MaxLOD, pow(2.f, 7.f * d) / (float)(1 << 7));
}

float ComputeEdgeLod(float myLod, float neighbourLod, uint neighbourDivision) {
	switch (neighbourDivision) {
	case LessDivided: return (float)(((int)neighbourLod >> 2) << 1);
	case MoreDivided: return (float)(((int)myLod >> 2) << 2);
	case EquallyDivided: default: return min(myLod, neighbourLod);
	}
}

HSCF_OUTPUT HSCF(InputPatch<HS_INPUT, 9> patch, uint id : SV_PrimitiveId) {
	HSCF_OUTPUT output = (HSCF_OUTPUT)0;

	float lods[] =
	{
		ComputePatchLOD(patch[4].pos),
		ComputePatchLOD(patch[5].pos),
		ComputePatchLOD(patch[6].pos),
		ComputePatchLOD(patch[7].pos),
		ComputePatchLOD(patch[8].pos) //me
	};

	output.inside[0] = output.inside[1] = lods[4];
	output.edge[0] = ComputeEdgeLod(lods[4], lods[3], NeighboursDivisionData[id].data[3]);
	output.edge[1] = ComputeEdgeLod(lods[4], lods[0], NeighboursDivisionData[id].data[0]);
	output.edge[2] = ComputeEdgeLod(lods[4], lods[1], NeighboursDivisionData[id].data[1]);
	output.edge[3] = ComputeEdgeLod(lods[4], lods[2], NeighboursDivisionData[id].data[2]);

	return output;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("HSCF")]
HS_OUTPUT HS(InputPatch<HS_INPUT, 9> patch, uint i : SV_OutputControlPointID) {
	HS_OUTPUT output = (HS_OUTPUT)0;
	output.pos = patch[i].pos;
	return output;
}
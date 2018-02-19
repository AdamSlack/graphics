	#version 150 core
in vec3 pos;
in vec3 vshade;
in vec3 normal;
in vec2 tcoord;
in vec3 tangent;
in vec3 bitangent;

out vec3 cshade;
out vec3 v_o;
out vec3 locallight_o;
out vec3 n;

//out vec3 rawpos;
out vec2 tc_out;
uniform vec3 centre;
uniform float pscale;

uniform mat3 rotation_projection;
uniform vec3 objcentre_to_eye_projected;
uniform vec3 light_in_object_coords;
uniform vec3 view_in_object_coords;

out vec3 vecTangent;
out vec3 vecBitangent;

in mat3 tbn;
out mat3 TBN;
out vec3 POS;


void main()
{
	//do it all without homogenous co-ords

	vec3 transpos;
	locallight_o = light_in_object_coords - pos;

	transpos.xyz = rotation_projection * pos.xyz + objcentre_to_eye_projected;

	//rawpos=pos;
    gl_Position.w = transpos.z;

	transpos.z -= pscale;

    gl_Position.xyz = transpos;

	POS = pos;
    v_o = view_in_object_coords + pos;
    n = normal;

    cshade = vshade;
    tc_out = tcoord;

	// check handedness of TBN

	//orthogonalise
	vecTangent = normalize(vecTangent-normal * dot(vecTangent,normal));
	vecBitangent = normalize(vecBitangent-normal * dot(vecBitangent,normal));

	vecTangent = normalize(rotation_projection * normalize(tangent));
	vecBitangent = normalize(rotation_projection * normalize(bitangent));

	//if (dot(cross(normal, vecTangent), vecBitangent) < 0.0f){
	//	vecTangent = vecTangent * -1.0f;
	//}
	//vecTangent = normalize(normalize(tangent));
	//vecBitangent =normalize(normalize(bitangent));

	TBN = transpose(mat3(vecTangent, vecBitangent, normal));
	//TBN = tbn;
}

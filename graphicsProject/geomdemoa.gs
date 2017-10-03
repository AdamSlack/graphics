	#version 150 core
// Simple Geometry shader example
layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;  
in vec3 rawpos[3];
in vec3 Vcshade[3];
in vec3 Vv_o[3];
in vec3 Vlocallight_o[3];
in vec3 Vn[3];
in vec2 Vtcoord[3];
out vec3 cshade;
out vec3 v_o;
out vec3 locallight_o;
out vec3 n;
out vec2 tc_out;
uniform vec3 centre;
uniform float pscale;
uniform mat3 rotation_projection;
uniform vec3 objcentre_to_eye_projected;
uniform vec3 light_in_object_coords;
uniform vec3 view_in_object_coords;
uniform float shade;

struct vertex
{
	vec4 tpos;
	vec3 norm,light,eye,shade;
	vec2 tco;
	
}; 
vertex verts[6];
//globals
float radius;//radius of the constructed shape - calced early in main - used by newvert
//convenient to simply duplicate vertex shader functionality here
//to apply to generated vertices. 
//Vertex shader seems pretty much redundant once you have a geometry shader
//input parameters raw wertex
//outputs transformed and projected vertex
// and adjusted lighting and viewing vectors in object co-ordinates
void vertexshaderfn( in vec3 pos,out vec4 Position, out vec3 v, out vec3 l)
{
	vec3 transpos;
	l=light_in_object_coords-pos;
	transpos.xyz=rotation_projection*pos.xyz+objcentre_to_eye_projected;
    Position.w= transpos.z;
	transpos.z-=pscale;
    Position.xyz=transpos;
    v=view_in_object_coords-pos;
}
vec3 newvert(in vec3 v1,in vec3 v2)
{
	vec3 vtmp=v1+v2-2.0*centre;//vector in  direction between v1 and v2 from centre
	return normalize(vtmp)*radius+centre;
}


void emitvertfromarray(in int i, in int k)
{
	gl_Position=verts[i].tpos;
	tc_out=verts[i].tco;
	if (verts[i].shade.r==verts[i].shade.b &&  verts[i].shade.r==verts[i].shade.g )
	{
		switch(k)
		{
			case 0: cshade=vec3(1.0,1.0,1.0);
			break;
			case 1: cshade=vec3(1.0,1.0,0.0);
			break;
			case 2: cshade=vec3(0.0,0.0,1.0);
			break;
			case 3: cshade=vec3(0.0,1.0,0.0);
			break;
			case 4: cshade=vec3(0.0,1.0,1.0);
			break;
		}
	}
	else
	cshade=verts[i].shade ;
	v_o=verts[i].eye;
	locallight_o=verts[i].light;
	n=verts[i].norm;
	EmitVertex();
	
}
void emittrianglefromarray(in int i, in int j,in int k, in int tag)
{
	emitvertfromarray(i,tag);
	emitvertfromarray(j,tag);
	emitvertfromarray(k,tag);
	EndPrimitive();
}

void main()
{
	int i,j;
	radius=length(rawpos[0]-centre);
	// Set up Structure with extra vertices
	for (i=0;i<3;i++)
	{
		verts[i*2].tpos=gl_in[i].gl_Position;
		verts[i*2].norm=normalize(mix(normalize(centre-rawpos[i]),Vn[i],shade));//allow normals to be adjusted between flat cube and spherical
		verts[i*2].light=Vlocallight_o[i];
		verts[i*2].eye=Vv_o[i];
		verts[i*2].shade=Vcshade[i];
		verts[i*2].tco=Vtcoord[i];
		j=(i+1)%3;
		vec3 tmp=newvert(rawpos[i],rawpos[j]);
		vertexshaderfn(tmp,verts[i*2+1].tpos,verts[i*2+1].eye,verts[i*2+1].light);
		verts[i*2+1].tco=mix(Vtcoord[i],Vtcoord[j],0.5);
		verts[i*2+1].shade=Vcshade[i];
		verts[i*2+1].norm=normalize(mix(normalize(centre-tmp),Vn[i],shade));//allow normals to be adjusted between flat cube and spherical
	}
	//and send them out
	if (shade<1.0)
	{
		emittrianglefromarray(0,1,5,1);
		emittrianglefromarray(1,2,3,2);
		emittrianglefromarray(3,4,5,3);
		emittrianglefromarray(1,3,5,4);
	}
	else
		emittrianglefromarray(0,2,4,0);

	
}

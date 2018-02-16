//--------------------------------------------------------------------------------------------
// File:		sbm.vs
// Version:		V0.1
// Author:		Daniel Rhodes
// Date:		27/11/2006
// Description:	Super Bump Mapping Vertex Shader in GLSL
// Notes:		GLSL documentation can be found	at:
//				http://developer.3dlabs.com/openGL2/index.htm
// Input:		gl_MultiTexCoord0				= texture coordinates for unit 0
// Output:		gl_Position						= vertex object	space position
//				gl_TexCoord						= vertex texture coordinates
//--------------------------------------------------------------------------------------------
varying vec3 vReflectionVec;
varying vec3 vEyeVec;
varying vec3 vNormal;

void main()
{
	// Set the output vertex position in clip space
	gl_Position = ftransform();
   
	// Pass the texture coordinates to the pixel shader
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	vEyeVec = normalize((gl_ModelViewMatrix * gl_Vertex).xyz);
	vNormal = normalize(gl_NormalMatrix * gl_Normal);
	vReflectionVec = reflect(vEyeVec, vNormal);
	
}

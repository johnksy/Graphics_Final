#version 140
#extension GL_ARB_compatibility: enable

in vec4 p;
in vec3 normal, lightDir;

uniform vec4 LightSourcePosition;
uniform vec4 LightSourceDiffuse, LightSourceAmbient, LightSourceSpecular;
uniform vec4 LightModelDiffuse, LightModelAmbient, LightModelSpecular;
uniform vec4 FrontMaterialDiffuse, FrontMaterialAmbient, FrontMaterialSpecular;
uniform float alpha;
uniform vec3 k;
const vec3 ambientColor = vec3(0.3, 0.0, 0.0);
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);
void main()
{	
	vec4 color;
	// ToDo
	vec3 v = normalize(vec3(-p));
	vec3 n = normalize(normal);
	vec3 l = normalize(vec3(LightSourcePosition - p));
	vec3 r = normalize(reflect(-l, n));
	
	vec4 diffuse = FrontMaterialDiffuse * LightSourceDiffuse;
	color = k[0]*max(dot(l, n), 0)*diffuse;
	
	vec4 ambient = FrontMaterialAmbient * LightSourceAmbient;
	ambient +=LightModelAmbient * FrontMaterialAmbient;
	color += k[1]*ambient;
	

	vec4 specular = FrontMaterialSpecular * LightSourceSpecular;
	specular += pow(max(dot(r,v), 0), alpha)*specular;
	color += k[2]*specular;
	

	gl_FragColor = color;
	//gl_FragColor = vec4(0,0,0,1);
	/*
	float lambertian = max(dot(l,n), 0.0);
    float specular = 0.0;

    if(lambertian > 0.0) {
       float specAngle = max(dot(r, v), 0.0);
       specular = pow(specAngle, alpha);
    }
    gl_FragColor = vec4(k[1]*ambientColor +
                      k[0]*lambertian*diffuseColor +
                      k[2]*specular*specColor, 1.0);
	
	*/
}
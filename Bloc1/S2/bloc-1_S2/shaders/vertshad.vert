#version 330 core

in vec3 vertex;
out vec3 fcolor;
in vec3 color;
void main()  {
	vec3 vaux = 0.5*vertex;
	fcolor = color;
    gl_Position = vec4 (vaux, 1.0);
	
}

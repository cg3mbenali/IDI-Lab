#version 330 core

in vec3 vertex;

void main()  {
	vec3 vaux = 0.5*vertex;
    gl_Position = vec4 (vaux, 1.0);
	
}

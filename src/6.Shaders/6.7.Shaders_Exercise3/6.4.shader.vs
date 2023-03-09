#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 ourColor;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	ourColor = aPos;
}

/*
	Q: why is the bottom-left side of our triangle black?
	A: Because the bottom-left side of our triangle has negative x and y
	   coordinates, so the R & G values of RGB color will be replaced by
	   0 (the color values should range between 0.0 and 1.0 and negative
	   values are not accepted.)
*/
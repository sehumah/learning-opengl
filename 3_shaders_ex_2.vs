#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;

uniform float hOffset;  // horizontal offset to move the triangle to the right side of the screen

void main () {
    gl_Position = vec4(aPos.x + hOffset, aPos.y, aPos.z, 1.0);
    ourColor = aColor;
}

// Note: Challenge completed by myself
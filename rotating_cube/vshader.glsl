#version 150 

in vec4 vPosition;
in vec4 vColor;
out vec4 color;

uniform mat4 rotation;

void main() {
  color = vColor;
  gl_Position = rotation * vPosition;
}

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>

#include <opengl_learning/Angel.h>

const int Nsubdivide = 5;
const int NumTriangles = pow(3, Nsubdivide + 1);
const int NumVertices = 3 * NumTriangles;

vec2 points[NumVertices];
int i = 0;

void triangle(const vec2& a, const vec2& b, const vec2& c) {
  points[i++] = a;
  points[i++] = b;
  points[i++] = c;
}

void divide_triangle(const vec2& a, const vec2& b, const vec2& c, int k) {
  if (k > 0) {
    // Compute midpoints
    vec2 ab = (a + b)/2.0;
    vec2 ac = (a + c)/2.0;
    vec2 bc = (b + c)/2.0;

    // Subdivide triangles
    divide_triangle(a, ab, ac, k-1);
    divide_triangle(c, ac, bc, k-1);
    divide_triangle(b, bc, ab, k-1);
  } else {
    // Base case
    triangle(a, b, c);
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, NumTriangles);
  glFlush();
}

void init() {
  vec2 vertices[3] = {vec2(-1.0, -1.0), vec2(0.0, 1.0), vec2(1.0, -1.0)};

  divide_triangle(vertices[0], vertices[1], vertices[2], Nsubdivide);
  
  
  GLuint abuffer;
  glGenVertexArrays(1, &abuffer);
  glBindVertexArray(abuffer);
  
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  GLuint program = InitShader("vshader.glsl", "fshader.glsl");
  glUseProgram(program);

  // Initialize vertex position attribute from vertex shader
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Sierpinski Gasket");

  glewInit();
  glutDisplayFunc(display);

  init();

  glutMainLoop();
  return 0;
}

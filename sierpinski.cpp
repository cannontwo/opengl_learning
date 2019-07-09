#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>

#include <opengl_learning/Angel.h>

const int N = 50000;

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_POINTS, 0, N);
  glFlush();
}

void init() {
  vec2 points[N];

  vec2 vertices[3] = {vec2(-1.0, 1.0), vec2(0.0, 1.0), vec2(1.0, -1.0)};
  points[0] = vec2(0.25, 0.5);
  
  // Generate sierpinski points
  for (int i = 1; i < N; i++) {
    int j = rand() % 3;
    points[i] = (points[i-1] + vertices[j]) / 2.0;
  }
  
  GLuint abuffer;
  glGenVertexArrays(1, &abuffer);
  glBindVertexArray(abuffer);
  
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  // TODO
  
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Simple OpenGL example");

  glewInit();
  glutDisplayFunc(display);

  init();

  glutMainLoop();
}

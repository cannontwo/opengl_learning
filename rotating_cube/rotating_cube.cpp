#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>

#include <opengl_learning/Angel.h>

const int NumVertices = 36;

vec4 quad_colors[NumVertices];
vec4 points[NumVertices];

static int i = 0;
int axis = 0;

GLfloat theta[3] = {0.0, 0.0, 0.0};
GLint matrix_loc;

// Magic numbers/colors
vec4 vertices[8] = {vec4(-0.5, -0.5, 0.5, 1.0), vec4(-0.5, 0.5, 0.5, 1.0),
                    vec4(0.5, 0.5, 0.5, 1.0), vec4(0.5, -0.5, 0.5, 1.0),
                    vec4(-0.5, -0.5, -0.5, 1.0), vec4(-0.5, 0.5, -0.5, 1.0),
                    vec4(0.5, 0.5, -0.5, 1.0), vec4(0.5, -0.5, -0.5, 1.0)};

vec4 colors[8] = {vec4(0.0, 0.0, 0.0, 1.0),
                  vec4(1.0, 0.0, 0.0, 1.0),
                  vec4(1.0, 1.0, 0.0, 1.0),
                  vec4(0.0, 1.0, 0.0, 1.0),
                  vec4(0.0, 0.0, 1.0, 1.0),
                  vec4(1.0, 0.0, 1.0, 1.0),
                  vec4(0.0, 1.0, 1.0, 1.0),
                  vec4(1.0, 1.0, 1.0, 1.0)};

void quad(int a, int b, int c, int d) {
  // Triangle 1
  quad_colors[i] = colors[a];
  points[i] = vertices[a];
  i++;
  quad_colors[i] = colors[b];
  points[i] = vertices[b];
  i++;
  quad_colors[i] = colors[c];
  points[i] = vertices[c];
  i++;

  // Triangle 2
  quad_colors[i] = colors[a];
  points[i] = vertices[a];
  i++;
  quad_colors[i] = colors[c];
  points[i] = vertices[c];
  i++;
  quad_colors[i] = colors[d];
  points[i] = vertices[d];
  i++;
}

// Specify the faces of the cube. Note counter-clockwise vertex order.
void colorcube() {
  quad(1, 0, 3, 2);
  quad(2, 3, 7, 6);
  quad(3, 0, 4, 7);
  quad(6, 5, 1, 2);
  quad(4, 5, 6, 7);
  quad(5, 4, 0, 1);
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 033: // Escape
    case 'q':
    case 'Q':
      exit(EXIT_SUCCESS);
      break;
  }
}

void mouse(int button, int state, int x, int y) {
  if (button  == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
    axis = 0;
  if (button  == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
    axis = 1;
  if (button  == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
    axis = 2;
}

void idle() {
  theta[axis] += 0.1;
  if (theta[axis] > 360.0) {
    theta[axis] -= 360.0;
  }

  glutPostRedisplay();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4 ctm = RotateX(theta[0]) * RotateY(theta[1]) * RotateZ(theta[2]);
  glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  glutSwapBuffers();
}

void init() {
  // Make cube points
  colorcube();

  // Load shaders
  GLuint program = InitShader("vshader.glsl", "fshader.glsl");
  glUseProgram(program);

  // Vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize buffer
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(quad_colors), NULL, GL_STATIC_DRAW);

  // Insert data
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(quad_colors), quad_colors);

  // Initialize vertex position attribute from vertex shader
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  GLuint cloc = glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(cloc);
  glVertexAttribPointer(cloc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

  // Set up space for the transformation matrix
  matrix_loc = glGetUniformLocation(program, "rotation");
  
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Rotating Cube");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);

  glutMainLoop();
  return 0;
}

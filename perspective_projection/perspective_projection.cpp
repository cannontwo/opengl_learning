#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>

#include <opengl_learning/Angel.h>

const int NumVertices = 36;

vec4 quad_colors[NumVertices];
vec4 points[NumVertices];

// Viewing transformation parameters
GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

const GLfloat dr = 5.0 * DegreesToRadians;

GLuint model_view; // Model-view matrix uniform shader variable location

// Projection transformation parameters
GLfloat left = -1.0, right = 1.0;
GLfloat bottom = -1.0, top = 1.0;
GLfloat zNear = 0.5, zFar = 3.0;

GLuint projection; // Projection matrix uniform shader variable location

static int i = 0;

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

    case 'x': left *= 1.1; right *= 1.1; break;
    case 'X': left *= 0.9; right *= 0.9; break;
    case 'y': bottom *= 1.1; right *= 1.1; break;
    case 'Y': bottom *= 0.9; right *= 0.9; break;
    case 'z': zNear *= 1.1; zFar *= 1.1; break;
    case 'Z': zNear *= 0.9; zFar *= 0.9; break;
    case 'r': radius *= 2.0; break;
    case 'R': radius *= 0.5; break;
    case 'o': theta += dr; break;
    case 'O': theta -= dr; break;
    case 'p': phi += dr; break;
    case 'P': phi -= dr; break;

    case ' ': // Reset values
      left = -1.0;
      right = 1.0;
      bottom = -1.0;
      top = 1.0;
      zNear = 0.5;
      zFar = 3.0;
      radius = 1.0;
      theta = 0.0;
      phi = 0.0;
      break;
  }

  glutPostRedisplay();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  vec4 eye(radius * sin(theta) * cos(phi),
           radius * sin(theta) * sin(phi),
           radius * cos(theta),
           1.0);
  vec4 at(0.0, 0.0, 0.0, 1.0);
  vec4 up(0.0, 1.0, 0.0, 0.0);

  mat4 mv = LookAt(eye, at, up);
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);

  mat4 p = Frustum(left, right, bottom, top, zNear, zFar);
  glUniformMatrix4fv(projection, 1, GL_TRUE, p);

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
  model_view = glGetUniformLocation(program, "model_view");
  projection = glGetUniformLocation(program, "projection");
  
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Perspective Cube");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return 0;
}

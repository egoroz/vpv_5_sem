#include <GL/glut.h>
#include <cmath>

// Функция, которую нужно визуализировать
double f(double x, double y) {
  return sin(sqrt(x*x + y*y));
}

int lastX = 0;
int lastY = 0;

float moveX = 0.0f;
float moveY = 0.0f;
float moveZ = -10.0f;


// Диапазон значений x и y
const double X_MIN = -5.0;
const double X_MAX = 5.0;
const double Y_MIN = -5.0;
const double Y_MAX = 5.0;

// Количество точек по x и y
const int GRID_SIZE = 100;

// Угол поворота
float rotateX = 0.0f;
float rotateY = 0.0f;

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

    // Вращение и перемещение
  glTranslatef(moveX, moveY, moveZ); // Сначала перемещение, потом вращение
  glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
  glRotatef(rotateY, 0.0f, 1.0f, 0.0f);

  // Вращение сцены
  glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
  glRotatef(rotateY, 0.0f, 1.0f, 0.0f);

  // Рисование поверхности
  glBegin(GL_QUADS);
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      double x1 = X_MIN + (X_MAX - X_MIN) * i / GRID_SIZE;
      double y1 = Y_MIN + (Y_MAX - Y_MIN) * j / GRID_SIZE;
      double z1 = f(x1, y1);

      double x2 = X_MIN + (X_MAX - X_MIN) * (i + 1) / GRID_SIZE;
      double y2 = Y_MIN + (Y_MAX - Y_MIN) * j / GRID_SIZE;
      double z2 = f(x2, y2);

      double x3 = X_MIN + (X_MAX - X_MIN) * (i + 1) / GRID_SIZE;
      double y3 = Y_MIN + (Y_MAX - Y_MIN) * (j + 1) / GRID_SIZE;
      double z3 = f(x3, y3);

      double x4 = X_MIN + (X_MAX - X_MIN) * i / GRID_SIZE;
      double y4 = Y_MIN + (Y_MAX - Y_MIN) * (j + 1) / GRID_SIZE;
      double z4 = f(x4, y4);

      // Цвет в зависимости от значения z
      glColor3f(z1 + 1, 0, -z1 + 1); // Пример раскраски
      glVertex3f(x1, y1, z1);
      glColor3f(z2 + 1, 0, -z2 + 1);
      glVertex3f(x2, y2, z2);
      glColor3f(z3 + 1, 0, -z3 + 1);
      glVertex3f(x3, y3, z3);
      glColor3f(z4 + 1, 0, -z4 + 1);
      glVertex3f(x4, y4, z4);
    }
  }
  glEnd();

  glFlush();
  glutSwapBuffers();
}

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (double)width / height, 1.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glTranslatef(0.0f, 0.0f, -10.0f);
}

void specialKeys(int key, int x, int y) {
    float moveAmount = 0.5f;

    switch (key) {
        case GLUT_KEY_UP:
            moveY += moveAmount;
            break;
        case GLUT_KEY_DOWN:
            moveY -= moveAmount;
            break;
        case GLUT_KEY_LEFT:
            moveX -= moveAmount;
            break;
        case GLUT_KEY_RIGHT:
            moveX += moveAmount;
            break;
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    float moveAmount = 0.5f;

    switch (key) {
        case 'w':
            moveZ += moveAmount;
            break;
        case 'a':
            moveX += moveAmount;
            break;
        case 's':
            moveZ -= moveAmount;
            break;
        case 'd':
            moveX -= moveAmount;
            break;

    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
  if (state == GLUT_DOWN) {
    lastX = x; // Запоминаем начальные координаты при нажатии кнопки мыши
    lastY = y;
  }
}

void motion(int x, int y) {
    rotateX += (y - lastY);
    rotateY += (x - lastX);

    lastX = x;
    lastY = y;

    glutPostRedisplay();
}



int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("3D Function Visualization");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glEnable(GL_DEPTH_TEST);
  glutSpecialFunc(specialKeys); // Регистрируем обработчик специальных клавиш
  glutKeyboardFunc(keyboard); // Регистрируем обработчик обычных клавиш
  glutMainLoop();
  return 0;
}
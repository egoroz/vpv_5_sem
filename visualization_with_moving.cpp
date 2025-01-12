#include <GL/glut.h>
#include <cmath>
#include <complex>
#include <gsl/gsl_sf_laguerre.h>
#include <gsl/gsl_sf_legendre.h>


double laguerre(int n, int m, double x) {
    return gsl_sf_laguerre_n(n, m, x);
}

double legendre(int l, int m, double x) {
    return gsl_sf_legendre_Plm(l, std::abs(m), x);
}


// Боровский радиус (в атомных единицах)
const double a0 = 1.0;

// Квадрат модуля волновой функции атома водорода
double hydrogenWavefunctionSquared(int n, int l, int m, double r, double theta, double phi) {
    if (n <= 0 || l < 0 || l >= n || std::abs(m) > l) {
        return 0.0; // Недопустимые квантовые числа
    }

    double rho = 2.0 * r / (n * a0);

    // Радиальная часть
    double radial_part = std::pow(rho, l) * std::exp(-rho / 2.0) * laguerre(n - l - 1, 2 * l + 1, rho);

    // Нормировочный коэффициент для радиальной части
    double radial_norm = std::sqrt(2.0 / (n * n * n * a0 * a0 * a0) * tgamma(n - l) / (2.0 * n * tgamma(n + l + 1)));


    // Угловая часть (сферические гармоники)
    std::complex<double> angular_part = std::sqrt((2.0*l + 1.0) * tgamma(l-std::abs(m)+1) / (4.0 * M_PI * tgamma(l + std::abs(m) + 1))) *
       legendre(l, m, std::cos(theta)) * std::exp(std::complex<double>(0.0, m * phi));

    // Здесь angular_norm уже включен в angular_part


    return radial_norm * radial_norm * radial_part * radial_part * std::norm(angular_part);
}










//************

const double PI = 3.141592653589793;

int lastX = 0;
int lastY = 0;

float moveX = 0.0f;
float moveY = 0.0f;
float moveZ = -20.0f;


// Диапазон значений x и y
const double SIZE = 10;
const double X_MIN = -SIZE;
const double X_MAX = SIZE;
const double Y_MIN = -SIZE;
const double Y_MAX = SIZE;
const double Z_MIN = -SIZE;
const double Z_MAX = SIZE;

// Количество точек по x и y и z
const int GRID_SIZE = 50;

// Угол поворота
float rotateX = 0.0f;
float rotateY = 0.0f;

double f_spherical(double r, double theta, double phi) {
  return sin(r); // Пример:  зависимость только от радиуса
  // Или более сложная функция, зависящая от theta и phi
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(moveX, moveY, moveZ);
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);


    double minVal = 1e10; // Большое начальное значение
    double maxVal = 0; // Маленькое начальное значение

    // Первый проход: вычисление minVal и maxVal
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            for (int k = 0; k < GRID_SIZE; k++) {
                double x = X_MIN + (X_MAX - X_MIN) * i / GRID_SIZE;
                double y = Y_MIN + (Y_MAX - Y_MIN) * j / GRID_SIZE;
                double z = Z_MIN + (Z_MAX - Z_MIN) * k / GRID_SIZE;

                // Перевод декартовых в сферические
                double r = std::sqrt(x*x + y*y + z*z);
                double theta = std::acos(z / r);
                double phi = std::atan2(y, x);

                int n = 2;
                int l = 1;
                int m = 0;
                double val = hydrogenWavefunctionSquared(n, l, m, r, theta, phi);



                if (val < minVal) minVal = val;
                if (val > maxVal) maxVal = val;
            }
        }
    }

// Второй проход: отрисовка
     for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            for (int k = 0; k < GRID_SIZE; k++) {
                double x = X_MIN + (X_MAX - X_MIN) * i / GRID_SIZE;
                double y = Y_MIN + (Y_MAX - Y_MIN) * j / GRID_SIZE;
                double z = Z_MIN + (Z_MAX - Z_MIN) * k / GRID_SIZE;

                // Перевод декартовых в сферические
                double r = std::sqrt(x*x + y*y + z*z);
                double theta = std::acos(z / r);
                double phi = std::atan2(y, x);

                int n = 3;
                int l = 2;
                int m = 2;
                double val = hydrogenWavefunctionSquared(n, l, m, r, theta, phi);
                double normalizedVal = (val - minVal) / (maxVal - minVal);
               

                // Минимальный и максимальный радиусы шариков
                const double MIN_RADIUS = 0.0;
                const double MAX_RADIUS = 1.0;
                double radius = (MIN_RADIUS + (MAX_RADIUS - MIN_RADIUS) * normalizedVal);

                // Цвет от синего к красному
                glColor3f(exp(-normalizedVal*normalizedVal/1), 0.0, 1-normalizedVal);

                // Рисуем сферу
                glPushMatrix();
                glTranslatef(x, y, z);
                glutSolidSphere(radius, 10, 10);
                glPopMatrix();
            }
        }
    }

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
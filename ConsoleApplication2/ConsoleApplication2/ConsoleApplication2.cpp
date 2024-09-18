#include "glut.h"
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

int NumberTask = 0;

double x_0, y_0, h, a, b, zoom;
int steps;

// Истинное решение для первого задания
double trueSolutionTestTask(double x)
{
    return pow(2.71828, -1.5*x)*y_0;
}


// Функция
double f(double x, double y)
{
    switch (NumberTask)
    {
    case 0:
        return -3 / 2 * y;
    case 1:
        return 1/pow(1+x*x,1/3)*y*y+y-y*y*y*sin(10*x);
    case 2:
        return -(a*y*y+b*x)/y;
    default:
        return x;
    }
}

// Метод Рунге-Кутта 4 порядка
void RK4(double &x, double &y, double h)
{
    double k1 = h * f(x, y);
    double k2 = h * f(x + 0.5 * h, y + 0.5 * k1);
    double k3 = h * f(x + 0.5 * h, y + 0.5 * k2);
    double k4 = h * f(x + h, y + k3);

    y = y + (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
}


// Построение портрета в изоклинах
void drawPhasePortrait(double h = 0.001)
{
    glPointSize(1.0);
    glColor3f(1.0, 0.0, 0.0);
    double x = 0.0, y = 0.0;
    glBegin(GL_POINTS);

    for (double x_l = -9.9; x_l < 9.9; x_l = x_l + 4.0)
    {
        for (double y_l = -9.9; y_l < 9.9; y_l = y_l + 4.0)
        {
            // Положительный шаг
            x = x_l;
            y = y_l;
            for (int i = 0; i < 100000; i++)
            {
                RK4(x, y, h);
                glVertex2f(x, y);
                x += h;
            }

            // Отрицательный шаг
            x = x_l;
            y = y_l;
            for (int i = 0; i < 20000; i++)
            {
                RK4(x, y, -h);
                glVertex2f(x, y);
                x -= h;
            }
        }
    }

    glEnd();
}

// Построение решения задачи коши (единственное проходящее через конкретную точку)
void drawCauchySolution(double x_l = 0.0, double y_l = 0.0, double h = 0.001, int steps = 20000)
{
    glPointSize(1.0);
    glColor3f(0.0, 0.0, 1.0);
    double x = 0.0, y = 0.0;
    glBegin(GL_POINTS);

    // Положительный шаг
    x = x_l;
    y = y_l;
    for (int i = 0; i < steps; i++)
    {
        RK4(x, y, h);
        glVertex2f(x, y);
        x += h;
    }

    // Отрицательный шаг
    x = x_l;
    y = y_l;
    for (int i = 0; i < steps; i++)
    {
        RK4(x, y, -h);
        glVertex2f(x, y);
        x -= h;
    }

    glEnd();


    //Рисование начальной точки
    glPointSize(5.0);
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    glVertex2f(x_l, y_l);
    glEnd();

    // Истинное решение для первого задания
    if (NumberTask == 0)
    {
        glPointSize(1.0);
        glColor3f(0.0, 1.0, 0.0);
        double x = 0.0, y = 0.0;
        glBegin(GL_POINTS);

        for (int i = 0; i < steps; i++)
        {
            y = trueSolutionTestTask(x);
            glVertex2f(x, y);
            x += h;
        }

        x = x_l;

        for (int i = 0; i < steps; i++)
        {
            y = trueSolutionTestTask(x);
            glVertex2f(x, y);
            x -= h;
        }

        glEnd();
    }
}

void coordinate()
{
    glColor3f(1.0, 1.0, 1.0);
    // Рисование OX оси
    glBegin(GL_LINES);
    glVertex2f(-zoom, 0.0);
    glVertex2f(zoom, 0.0);
    glEnd();

    // Рисование OY оси
    glBegin(GL_LINES);
    glVertex2f(0.0, -zoom);
    glVertex2f(0.0, zoom);
    glEnd();


    // Рисование отметок на оси OX и расставление чисел
    for (int i = -9; i < 10; i +=2)
    {
        double number = double(i) / 10.0 * zoom;
        string numberStr = to_string(number);

        glBegin(GL_LINES);
        glVertex2f(double(i)/10.0*zoom, -0.02 * zoom);
        glVertex2f(double(i) / 10.0 * zoom, 0.02 * zoom);
        glEnd();

        if (i != 0)
        {
            glRasterPos2f(double(i) / 10.0 * zoom + 0.01 * zoom - 0.03 * zoom, 0.03 * zoom);
            int max_Char = 0;
            for (char c : numberStr)
            {
                if (max_Char < 4)
                {
                    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
                }
                max_Char++;
            }
        }
    }

    // Рисование отметок на оси OY и расставление чисел
    for (int i = -9; i < 10; i +=2)
    {
        double number = double(i) / 10.0 * zoom;
        string numberStr = to_string(number);

        glBegin(GL_LINES);
        glVertex2f(-0.02 * zoom, double(i) / 10.0 * zoom);
        glVertex2f(0.02 * zoom, double(i) / 10.0 * zoom);
        glEnd();

        glRasterPos2f(0.01 * zoom, double(i) / 10.0 * zoom + 0.01 * zoom);
        int max_Char = 0;
        for (char c : numberStr)
        {
            if (max_Char < 4)
            {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
            }
            max_Char++;
        }
    }


    glColor3f(1.0, 1.0, 1.0);
    //Название оси X
    glRasterPos2f(0.96 * zoom, 0.01 * zoom);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');

    //Название оси Y
    glRasterPos2f(0.01 * zoom, 0.96 * zoom);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'y');
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawPhasePortrait(h);

    drawCauchySolution(x_0, y_0, h, steps);

    coordinate();

    glFlush();
    NumberTask++;
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(-zoom, zoom, -zoom, zoom);
}


int main(int argc, char** argv)
{

    setlocale(LC_ALL, "Russian");
    
    cout << "Введите количество шагов:";
    cin >> steps;

    cout << "Введите x0:";
    cin >> x_0;

    cout << "Введите y0:";
    cin >> y_0;

    cout << "Введите размер шага:";
    cin >> h;

    cout << "Введите a:";
    cin >> a;

    cout << "Введите b:";
    cin >> b;

    cout << "Введите размер окна:";
    cin >> zoom;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Main task 2");
    init();
    glutDisplayFunc(display);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(650, 0);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Main task 1");
    init();
    glutDisplayFunc(display);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(1300, 0);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Test task");
    init();
    glutDisplayFunc(display);

    glutMainLoop();
    
    cout << "Введите размер шага:";
}
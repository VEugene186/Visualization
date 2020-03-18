#include <stdio.h>
#include <cmath>
#include "Lorenz.h"
#include "RungeKutta.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

const int N = 10001;
const double dt = 0.01;
double t[N], q[N][3];
int xMouse = -1, yMouse = -1;

void solve();
void wireRender();
void tubeRender();
void init();
void displayFunc();
void mouseFunc(int button, int state, int x, int y);
void motionFunc(int x, int y);

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(600, 600);
    glutCreateWindow("LorenzTube");
    init();
    glutDisplayFunc(displayFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);
    glutMainLoop();
    return 0;
}

void solve() {
    RungeKutta method;
    Lorenz eqs;
    method.init(&eqs);
    t[0] = 0.0;
    q[0][0] = 1.0; q[0][1] = 0.0; q[0][2] = 0.0;
    for (int i = 1; i < N; i++) {
        method.makeStep(&eqs, t[i - 1], q[i - 1], q[i], dt);
        t[i] = t[i - 1] + dt;
    }
}

void wireRender() {
    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_STRIP);
    glColor3d(0.0, 0.0, 1.0);
    for (int i = 0; i < N; i++) {
        glVertex3dv(q[i]);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void tubeRender() {
    GLUquadricObj * obj = gluNewQuadric();
    glColor3d(1.0, 1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    for (int i = 0; i < N - 1; i++) {
        double dx = q[i + 1][0] - q[i][0];
        double dy = q[i + 1][1] - q[i][1];
        double dz = q[i + 1][2] - q[i][2];
        double len = sqrt(dx * dx + dy * dy + dz * dz);
        double ang = acos(dz / len) * 180.0 / M_PI;
        glPushMatrix();
        glTranslated(q[i][0], q[i][1], q[i][2]);
        glRotated(ang, -dy / sqrt(dx * dx + dy * dy), dx / sqrt(dx * dx + dy * dy), 0.0);
        gluCylinder(obj, 0.2, 0.2, len, 16, 2);
        glPopMatrix();
    }

    gluDeleteQuadric(obj);
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -50.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_LIGHT0);
    //glCullFace(GL_FRONT);
    solve();
}

void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //wireRender();
    tubeRender();
    glutSwapBuffers();
}

void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            xMouse = x;
            yMouse = y;
            //printf("pressed at %d, %d\n", x, y);
        }
        if (state == GLUT_UP) {
            xMouse = -1;
            yMouse = -1;
            //printf("released at %d, %d\n", x, y);
        }
    }
}

void motionFunc(int x, int y) {
    //printf("moved at %d, %d\n", x, y);
    if ((xMouse >= 0) && (yMouse >= 0)) {
        double xform[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, xform);
        double dx = 0.5 * (x - xMouse), dy = 0.5 * (y - yMouse);
        
        glRotated(dx, xform[1], xform[5], xform[9]);
        glRotated(dy, xform[0], xform[4], xform[8]);

        xMouse = x;
        yMouse = y;
        //float pos[] = { xform[2], xform[6], xform[10] };
        //glLightfv(GL_LIGHT0, GL_POSITION, pos);
        //float dir[] = { -xform[2], -xform[6], -xform[10] };
        //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
        glutPostRedisplay();
    }

}



#include <cstdlib>
#include <GL/glfw.h>
#include <math.h>

float random(float a, float b)
{
    return (rand()/(float)RAND_MAX)*(b-a)+a;
}

float irandom(int a, int b)
{
    return rand()%(b-a+1)+a;
}

bool running()
{
    return( !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam( GLFW_OPENED) );
}

void init()
{
    int width, height;

    glfwInit();
    if( !glfwOpenWindow( 1920, 1080, 0, 0, 0, 0, 8, 0, GLFW_WINDOW ) ) return;

    glfwGetWindowSize( &width, &height );
    height = height > 0 ? height : 1;

    glViewport( 0, 0, width, height );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 65.0f, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(0.0f, -10.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f );
}

void finit()
{
    glfwTerminate();
}


void drawCurvedP(float r, float startAngle, float endAngle, float a, float displacementX, float displacementY)
{
    float R = r+0.5;
    int n = 32;
    float step = M_PI/n;
    float k = 1;

    glBegin(GL_POLYGON);

    glNormal3f(k*cos(startAngle),k*sin(startAngle),0);
    glVertex3f(r*cos(startAngle)+displacementX, r*sin(startAngle)+displacementY, a);
    glVertex3f(R*cos(startAngle)+displacementX, R*sin(startAngle)+displacementY, a);
    glVertex3f(R*cos(startAngle)+displacementX, R*sin(startAngle)+displacementY, -a);
    glVertex3f(r*cos(startAngle)+displacementX, r*sin(startAngle)+displacementY, -a);

    glEnd();

    while(startAngle<=endAngle)
    {
        glBegin(GL_POLYGON);

        glNormal3f(0,0,1);
        glVertex3f(r*cos(startAngle)+displacementX, r*sin(startAngle)+displacementY, a);
        glVertex3f(R*cos(startAngle)+displacementX, R*sin(startAngle)+displacementY, a);
        glVertex3f(R*cos(startAngle+step)+displacementX, R*sin(startAngle+step)+displacementY, a);
        glVertex3f(r*cos(startAngle+step)+displacementX, r*sin(startAngle+step)+displacementY, a);

        glEnd();

        glBegin(GL_POLYGON);

        glNormal3f(0,0,-1);
        glVertex3f(r*cos(startAngle)+displacementX, r*sin(startAngle)+displacementY, -a);
        glVertex3f(R*cos(startAngle)+displacementX, R*sin(startAngle)+displacementY, -a);
        glVertex3f(R*cos(startAngle+step)+displacementX, R*sin(startAngle+step)+displacementY, -a);
        glVertex3f(r*cos(startAngle+step)+displacementX, r*sin(startAngle+step)+displacementY, -a);

        glEnd();

        glBegin(GL_POLYGON);

        glNormal3f(-cos(startAngle),-sin(startAngle),0);
        glVertex3f(r*cos(startAngle)+displacementX, r*sin(startAngle)+displacementY, a);
        glVertex3f(r*cos(startAngle)+displacementX, r*sin(startAngle)+displacementY, -a);

        glNormal3f(-cos(startAngle+step),-sin(startAngle+step),0);
        glVertex3f(r*cos(startAngle+step)+displacementX, r*sin(startAngle+step)+displacementY, -a);
        glVertex3f(r*cos(startAngle+step)+displacementX, r*sin(startAngle+step)+displacementY, a);

        glEnd();

        glBegin(GL_POLYGON);

        glNormal3f(cos(startAngle),sin(startAngle),0);
        glVertex3f(R*cos(startAngle)+displacementX, R*sin(startAngle)+displacementY, a);
        glVertex3f(R*cos(startAngle)+displacementX, R*sin(startAngle)+displacementY, -a);

        glNormal3f(cos(startAngle+step),sin(startAngle+step),0);
        glVertex3f(R*cos(startAngle+step)+displacementX, R*sin(startAngle+step)+displacementY, -a);
        glVertex3f(R*cos(startAngle+step)+displacementX, R*sin(startAngle+step)+displacementY, a);

        glEnd();

        startAngle += step;
    }
    glBegin(GL_POLYGON);

    glNormal3f(-sin(startAngle),cos(startAngle),0);
    glVertex3f(r*cos(startAngle)+displacementX, r*sin(startAngle)+displacementY, a);
    glVertex3f(R*cos(startAngle)+displacementX, R*sin(startAngle)+displacementY, a);
    glVertex3f(R*cos(startAngle)+displacementX, R*sin(startAngle)+displacementY, -a);
    glVertex3f(r*cos(startAngle)+displacementX, r*sin(startAngle)+displacementY, -a);

    glEnd();
}

void drawSolidParalelogram (float x,float y,float z,float ax,float ay,float az)
{
    ax = ax/2;
    ay = ay/2;
    az = az/2;

    // Wall towards -X
    glBegin( GL_POLYGON );
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(x-ax, y-ay, z-az);
        glVertex3f(x-ax, y-ay, z+az);
        glVertex3f(x-ax, y+ay, z+az);
        glVertex3f(x-ax, y+ay, z-az);
    glEnd();

    // Wall towards +X
    glBegin( GL_POLYGON );
        glNormal3f(+1.0, 0.0, 0.0);
        glVertex3f(x+ax, y-ay, z-az);
        glVertex3f(x+ax, y-ay, z+az);
        glVertex3f(x+ax, y+ay, z+az);
        glVertex3f(x+ax, y+ay, z-az);
    glEnd();

    // Wall towards-Y
    glBegin( GL_POLYGON );
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f(x-ax, y-ay, z-az);
        glVertex3f(x-ax, y-ay, z+az);
        glVertex3f(x+ax, y-ay, z+az);
        glVertex3f(x+ax, y-ay, z-az);
    glEnd();

    // Wall towards +Y
    glBegin( GL_POLYGON );
        glNormal3f(0.0, +1.0, 0.0);
        glVertex3f(x-ax, y+ay, z-az);
        glVertex3f(x-ax, y+ay, z+az);
        glVertex3f(x+ax, y+ay, z+az);
        glVertex3f(x+ax, y+ay, z-az);
    glEnd();

    // Wall towards-Z
    glBegin( GL_POLYGON );
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(x-ax, y-ay, z-az);
        glVertex3f(x-ax, y+ay, z-az);
        glVertex3f(x+ax, y+ay, z-az);
        glVertex3f(x+ax, y-ay, z-az);
    glEnd();

    // Wall towards +Z
    glBegin( GL_POLYGON );
        glNormal3f(0.0, 0.0, +1.0);
        glVertex3f(x-ax, y-ay, z+az);
        glVertex3f(x-ax, y+ay, z+az);
        glVertex3f(x+ax, y+ay, z+az);
        glVertex3f(x+ax, y-ay, z+az);
    glEnd();
}

void drawTheKanji()
{
    drawSolidParalelogram(0.75f,5.75f,0.0f,0.5f,4.5f,2.0f);
    drawSolidParalelogram(2.75f,4.75f,0.0f,0.5f,6.5f,2.0f);
    drawCurvedP(0.5,-M_PI/180*90,0,1.0f,2.0f,1.5f);
    drawSolidParalelogram(1.75f,0.75f,0.0f,0.5f,0.5f,2.0f);
    drawCurvedP(5.5f,-M_PI/180*30,0,1.0f,-5.0f,3.5f);

    drawSolidParalelogram(1.75f,7.75f,0.0f,1.5f,0.5f,2.0f);
    drawSolidParalelogram(1.75f,5.75f,0.0f,1.5f,0.5f,2.0f);
    drawSolidParalelogram(1.75f,3.75f,0.0f,1.5f,0.5f,2.0f);

    drawSolidParalelogram(3.75f,6.75f,0.0f,0.5f,2.5f,2.0f);
    drawSolidParalelogram(6.75f,6.75f,0.0f,0.5f,2.5f,2.0f);
    drawSolidParalelogram(5.25f,7.75f,0.0f,2.5f,0.5f,2.0f);
    drawSolidParalelogram(5.25f,6.75f,0.0f,2.5f,0.5f,2.0f);
    drawSolidParalelogram(5.25f,5.75f,0.0f,2.5f,0.5f,2.0f);

    drawSolidParalelogram(5.5f,4.75f,0.0,4.5f,0.5f,2.0f);
    drawCurvedP(5.0,-M_PI/180*60,-M_PI/180*35,1.0f,0.5f,7.5f);

    drawSolidParalelogram(5.75f,3.75f,0.0f,3.0f,0.5f,2.0f);
    drawCurvedP(6.8,-M_PI/180*60,-M_PI/180*35,1.0f,0.5f,7.5f);
    drawCurvedP(6.0,-M_PI/180*60,-M_PI/180*40,1.0f,0.5f,7.5f);

    drawSolidParalelogram(7.0f,2.25f,0.0f,0.5f,2.5f,2.0f);
    drawCurvedP(0.5,-M_PI/180*90,0,1.0f,6.25f,1.0f);
    drawSolidParalelogram(5.75f,0.25f,0.0f,1.0f,0.5f,2.0f);
}

int main()
{
    init();

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glEnable( GL_DEPTH_TEST );

    while( running() )
    {
        glClear( GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT );
        glRotatef( 0.3, 0.4, -0.2, 0.7);

        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        glEnable( GL_COLOR_MATERIAL );
        glEnable( GL_LIGHT0 );

        glColor3ub( 0, 255, 0 );

        drawTheKanji();

        glfwSwapBuffers();
    }
    finit();
    return 0;
}

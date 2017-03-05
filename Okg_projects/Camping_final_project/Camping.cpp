
#include <math.h>
#include <GL/glfw.h>
#define nL 21 /// брой точки в крайниците
#define nH 50 /// четен брой точки в главата
#define nJ 16 /// четен брой точки в ставите
#define PI 3.141592653589793
#define EPS 0.1
#define turnForward(a) glRotatef((a),1,0,0)
#define turnSideway(a) glRotatef((a),0,1,0)
#define turnAround(a) glRotatef((a),0,0,1)
struct OKG_POINT
{
    float x;
    float y;
    float z;
} ;
OKG_POINT vmul(OKG_POINT a, OKG_POINT b, OKG_POINT p)
{
a = {x:a.x-p.x, y:a.y-p.y, z:a.z-p.z};
b = {x:b.x-p.x, y:b.y-p.y, z:b.z-p.z};
return {x:a.y*b.z-a.z*b.y, y:a.z*b.x-a.x*b.z, z:a.x*b.y-a.y*b.x};
}
OKG_POINT spherical( float alpha, float beta, float r )
{
    return {x:r*cosf(alpha)*cosf(beta), y:r*sinf(alpha)*cosf(beta), z:r*sinf(beta)};
}
OKG_POINT limbal( float alpha, float beta, float from, float to, float width, float sx1, float sx2, float sy1, float sy2, float sz )
{
    float k1 = sinf(beta)/2+0.5;
    float k = pow(fabs(sin(beta)),10);
    float r = 0.5+0.5*width*cosf((from+(to-from)*k1)*PI/180);
    r = r*(1-k+k*cosf(beta));
    return {x:(sx1*(1-k1)+sx2*k1)*r*cosf(alpha), y:(sy1*(1-k1)+sy2*k1)*r*sinf(alpha), z:sz*k1};
}
double cosser( double t, double min, double max )
{
    if( min<=t && t<=max )
    t = t-min;
    else
    {
        t -= 2*PI;
        if( min<=t && t<=max )
        t = t-min;
        else
        return 0;
    }

    return 0.5+0.5*cos( t/(max-min)*2*PI-PI );
}
double smartRad( double a, double b, int n, float rads[][5] )
{
    double r = 1;
    for(int i=0; i<n; i++)
    r = r+rads[i][0]*cosser( a-PI/2,rads[i][1],rads[i][2] )*cosser( b,rads[i][3],rads[i][4] );
    return r;
}
void vertex(float alpha,float beta,float from,float span,float width, float sx1, float sx2, float sy1, float sy2, float sz )
{
    OKG_POINT p  = limbal(alpha,beta,from,span,width,sx1,sx2,sy1,sy2,sz);
    OKG_POINT n  = vmul( limbal(alpha+EPS,beta,from,span,width,sx1,sx2,sy1,sy2,sz),
    limbal(alpha,beta+EPS,from,span,width,sx1,sx2,sy1,sy2,sz),p);
    glNormal3f(n.x,n.y,n.z);
    if( fabs(beta)>PI/2-EPS) glNormal3f(0,0,beta);
    glVertex3f(p.x,p.y,p.z);
}
void hertex(float alpha,float beta, int m, float rads[][5], float r=1 )
{
    OKG_POINT p  = spherical(alpha,beta,r*smartRad(alpha,beta,m,rads));
    OKG_POINT n  = vmul( spherical(alpha+EPS,beta,r*smartRad(alpha+EPS,beta,m,rads)),
    spherical(alpha,beta+EPS,r*smartRad(alpha,beta+EPS,m,rads)),p);
    glNormal3f(n.x,n.y,n.z);
    if( fabs(beta)>PI/2-EPS) glNormal3f(0,0,beta);
    glVertex3f(p.x,p.y,p.z);
}
void drawLimb( float from, float span, float width, float sx1, float sy1, float sx2, float sy2, float sz, bool useColor=true )
{
    if( useColor ) glColor3f(0,0.4,0.45);

    float alpha = 0;
        float dalpha = 2.0*PI/nL;
        for( int i=0; i<nL; i++, alpha+=dalpha)
        {
            glBegin( GL_QUAD_STRIP );
    float beta = PI/2;
    for( int j=0; j<nL+1; j++, beta-=dalpha/2)
            {
    vertex(alpha,beta,from,span,width,sx1,sx2,sy1,sy2,sz);
    vertex(alpha+dalpha,beta,from,span,width,sx1,sx2,sy1,sy2,sz);
            }
            glEnd( );
        }
        glTranslatef(0,0,sz); /// преместваме се в края на крайника
}
void drawJoint( float r )
{
    glColor3f(1,0.5,0);

        float alpha = 0;
        float dalpha = 2.0*PI/nJ;
        for( int i=0; i<nJ; i++, alpha+=dalpha)
        {
            glBegin( GL_QUAD_STRIP );
    float beta = PI/2;
    for( int j=0; j<nJ/2+1; j++, beta-=dalpha)
    {
    hertex(alpha,beta,0,NULL,r);
    hertex(alpha+dalpha,beta,0,NULL,r);
            }
            glEnd( );
        }
}
void drawHead( int n, float rads[][5] )
{
    double alpha = PI-PI/2;
    double dalpha = 2.0*PI/nH;
    for( int i=0; i<nH; i++, alpha+=dalpha )
    {
        glBegin( GL_QUAD_STRIP );
        double beta = PI/2;
        for( int j=1; j<nH; j++, beta-=dalpha/2 )
        {
            if( (i%2==0 && (j<nH/3 || i<nH/2-10 || i>nH/2+10) && j<nH-15)
            || (j==nH/2-3 && ((i<nH/2-2 && i>nH/2-8) || (i>nH/2+2 && i<nH/2+8)) )
            || (j==nH/2-2 && ((i<nH/2-2 && i>nH/2-6) || (i>nH/2+2 && i<nH/2+6)) )
            || (j==nH/2-1 && ((i<nH/2-2 && i>nH/2-4) || (i>nH/2+2 && i<nH/2+4)) )
            || (i==nH/2 && j>nH/2-5 && j<nH/2+10))
            glColor3f(0.3,0.5,0.55);
            else
            glColor3f(0,0.4,0.45);
            hertex( alpha,beta,n,rads );
            hertex( alpha+dalpha,beta,n,rads );
        }
    glEnd( );
    }
}
void drawLeg(float angle[])
{
    glPushMatrix();
    drawJoint(0.5);
    turnForward(angle[0]); // завъртане на крака
    turnSideway(angle[1]);
    drawLimb (-90,230,0.3, 0.9,1, 0.7,1, 3);
    drawJoint(0.3);
    turnForward(angle[2]); // завъртане на коляното
    drawLimb (-90,300,0.2, 0.5,0.7, 0.4,0.5, 3);
    drawJoint(0.2);
    glTranslatef(0,0,0.3);
    turnForward(90); // завъртане на ходилото
    turnForward(angle[3]);
    glTranslatef(0,0,-0.4);
    drawLimb (-90,420,0.1, 0.6,0.5, 0.9,0.2, 1.5);
    glPopMatrix();
}
void drawHand(float angle[])
{
    glPushMatrix();
    drawJoint(0.3);
    turnSideway(angle[0]); // завъртане в рамото
    turnForward(angle[1]);
    turnAround(angle[2]);
    drawLimb (-90,200,0.4, 0.5,0.6, 0.9,0.7, 2.5);
    drawJoint(0.25);
    turnForward(angle[3]); // завъртане в лакътя
    drawLimb (-90,180,0.2, 0.5,0.5, 0.4,0.3, 2);
    drawJoint(0.15);
    turnForward(angle[4]); // завъртане в китката
    turnSideway(angle[5]);
    turnAround(angle[6]);
    drawLimb (-90,180,0.3, 0.4,0.3, 0.6,0.1, 0.9);
    glPopMatrix();
}
void drawBody(float pos[], float ori[], float scale[], float waist[], float head[], float leftHand[], float rightHand[], float leftLeg[], float rightLeg[])
{
    glPushMatrix();
    glTranslatef(pos[0],pos[1],pos[2]); // позиция на тялото
    turnAround(ori[0]); // ориентация на тялото
    turnForward(ori[1]);

    glScalef(scale[0],scale[1],scale[2]); // размер на тялото

    glPushMatrix();
    glTranslatef(0,-0.1,-0.3); // таз
    drawLimb(-120,30,0.9, 2,1.5, 0.7,0.4, 1.5);
    drawJoint(0.2);
    turnForward(waist[0]); // завъртане в кръста
    turnAround(waist[1]);
    drawLimb(-130,150,0.4, 1.2,1, 1.9,0.9, 3.0, true);

    glPushMatrix();
    glTranslatef(0,0,-0.5);
    turnForward(180);
    glTranslatef(-1.1,0,0);
    drawHand(leftHand); // лява ръка
    glTranslatef(2.2,0,0);
    drawHand(rightHand); // дясна ръка
    glPopMatrix();

    turnAround(head[0]+180); // завъртане във врата
    turnForward(head[1]);
    turnSideway(head[2]);
    glTranslatef(0,-0.3,0.85);
    glScalef(scale[3]*0.8,scale[3],scale[3]);
    float rads[2][5] = {{-0.25,-3,+3,-1.8,0.0}, {-0.20,PI-3,PI+3,-1.5,1.0}};
    drawHead(2,rads);
    glPopMatrix();

    turnForward(180);
    glTranslatef(0.7,0,0);
    drawLeg(rightLeg); // десен крак
    glTranslatef(-1.4,0,0);
    drawLeg(leftLeg); // ляв крак
    glPopMatrix();
}


void drawSurface(float x,float y,float z, float a, float b, float c)
{
    a = a/2;
    b = b/2;
    c = c/2;

    glBegin( GL_POLYGON );
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(x-a, y-b, z-c);
        glVertex3f(x-a, y-b, z+c);
        glVertex3f(x-a, y+b, z+c);
        glVertex3f(x-a, y+b, z-c);
    glEnd();

    glBegin( GL_POLYGON );
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(x+a, y-b, z-c);
        glVertex3f(x+a, y-b, z+c);
        glVertex3f(x+a, y+b, z+c);
        glVertex3f(x+a, y+b, z-c);
    glEnd();

    glBegin( GL_POLYGON );
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f(x-a, y-b, z-c);
        glVertex3f(x-a, y-b, z+c);
        glVertex3f(x+a, y-b, z+c);
        glVertex3f(x+a, y-b, z-c);
    glEnd();

    glBegin( GL_POLYGON );
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(x-a, y+b, z-c);
        glVertex3f(x-a, y+b, z+c);
        glVertex3f(x+a, y+b, z+c);
        glVertex3f(x+a, y+b, z-c);
    glEnd();

    glBegin( GL_POLYGON );
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(x-a, y-b, z-c);
        glVertex3f(x-a, y+b, z-c);
        glVertex3f(x+a, y+b, z-c);
        glVertex3f(x+a, y-b, z-c);
    glEnd();

    glBegin( GL_POLYGON );
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(x-a, y-b, z+c);
        glVertex3f(x-a, y+b, z+c);
        glVertex3f(x+a, y+b, z+c);
        glVertex3f(x+a, y-b, z+c);
    glEnd();
}

void drawSmoothCylinder ( float x, float y, float z, float r, float h )
{
    int n = 32;

    float alpha = 0.0;
    float dalpha = 2*M_PI/n;

    for( int i=0; i<n; i++)
    {
        float dx1 = r*cos(alpha);
        float dy1 = r*sin(alpha);

        float dx2 = r*cos(alpha+dalpha);
        float dy2 = r*sin(alpha+dalpha);

        /// Рисуване на околна стена
        glBegin( GL_POLYGON );
            glNormal3f( cos(alpha), sin(alpha), 0 );
            glVertex3f( x+dx1, y+dy1, z+h );
            glVertex3f( x+dx1, y+dy1, z );
            glNormal3f( cos(alpha+dalpha), sin(alpha+dalpha), 0 );
            glVertex3f( x+dx2, y+dy2, z );
            glVertex3f( x+dx2, y+dy2, z+h );
        glEnd();

        /// Рисуване на парче от долната основа
        glBegin( GL_POLYGON );
            glNormal3f( 0, 0, -1 );
            glVertex3f( x,     y,     z );
            glVertex3f( x+dx1, y+dy1, z );
            glVertex3f( x+dx2, y+dy2, z );
        glEnd();

        /// Рисуване на парче от горната основа
        glBegin( GL_POLYGON );
            glNormal3f( 0, 0, 1 );
            glVertex3f( x,     y,     z+h );
            glVertex3f( x+dx1, y+dy1, z+h );
            glVertex3f( x+dx2, y+dy2, z+h );
        glEnd();

        alpha += dalpha;
    }
}

void drawSmoothEllipsoid( float x, float y, float z, float rx, float ry, float rz )
{
    int na = 48;
    int nb = 24;

    float alpha;
    float dalpha = 2.0*M_PI/na;
    float beta;
    float dbeta = 1.0*M_PI/nb;

    beta = M_PI/2;
    for( int j=0; j<nb; j++, beta-=dbeta)
    {
        alpha = 0;
        for( int i=0; i<na; i++, alpha+=dalpha)
        {
            OKG_POINT p;
            glBegin( GL_POLYGON );

            p = spherical(alpha,beta,1);
            glNormal3f(p.x,p.y,p.z);
            glVertex3f(x+rx*p.x,y+ry*p.y,z+rz*p.z);

            p = spherical(alpha+dalpha,beta,1);
            glNormal3f(p.x,p.y,p.z);
            glVertex3f(x+rx*p.x,y+ry*p.y,z+rz*p.z);

            p = spherical(alpha+dalpha,beta-dbeta,1);
            glNormal3f(p.x,p.y,p.z);
            glVertex3f(x+rx*p.x,y+ry*p.y,z+rz*p.z);

            p = spherical(alpha,beta-dbeta,1);
            glNormal3f(p.x,p.y,p.z);
            glVertex3f(x+rx*p.x,y+ry*p.y,z+rz*p.z);

            glEnd( );
        }
    }
}

void drawSmoothSphere( float x, float y, float z, float r )
{
    int na = 48;
    int nb = 24;

    float alpha;
    float dalpha = 2.0*M_PI/na;
    float beta;
    float dbeta = 1.0*M_PI/nb;

    beta = M_PI/2;
    for( int j=0; j<nb; j++, beta-=dbeta)
    {
        alpha = 0;
        for( int i=0; i<na; i++, alpha+=dalpha)
        {
            OKG_POINT p;
            glBegin( GL_POLYGON );

            p = spherical(alpha,beta,1);
            glNormal3f(p.x,p.y,p.z);
            glVertex3f(x+r*p.x,y+r*p.y,z+r*p.z);

            p = spherical(alpha+dalpha,beta,1);
            glNormal3f(p.x,p.y,p.z);
            glVertex3f(x+r*p.x,y+r*p.y,z+r*p.z);

            p = spherical(alpha+dalpha,beta-dbeta,1);
            glNormal3f(p.x,p.y,p.z);
            glVertex3f(x+r*p.x,y+r*p.y,z+r*p.z);

            p = spherical(alpha,beta-dbeta,1);
            glNormal3f(p.x,p.y,p.z);
            glVertex3f(x+r*p.x,y+r*p.y,z+r*p.z);

            glEnd( );
        }
    }
}

void drawFire ( float x, float y, float z, float r, float h )
{
    int n = 128;

    float alpha = 0.0;
    float dalpha = 2*M_PI/n;

    for( int i=0; i<n; i++)
    {
        float dx1 = r*cos(alpha);
        float dy1 = r*sin(alpha);

        float dx2 = r*cos(alpha+dalpha);
        float dy2 = r*sin(alpha+dalpha);

        float cosbeta = h/sqrt(h*h+r*r);
        float sinbeta = r/sqrt(h*h+r*r);

        /// Рисуване на околна стена
        glBegin( GL_POLYGON );
            glNormal3f( cos(alpha)*cosbeta, sin(alpha)*cosbeta, sinbeta );
            glColor3ub(255,0,0);
            glVertex3f( x+dx1, y+dy1, z );

            glNormal3f( cos(alpha+dalpha)*cosbeta, sin(alpha+dalpha)*cosbeta, sinbeta );
            glColor3ub(255,80,0);
            glVertex3f( x+dx2, y+dy2, z );

            glNormal3f( 2*cos(alpha+dalpha/2)*cosbeta, 2*sin(alpha+dalpha/2)*cosbeta, 2*sinbeta );
            glColor3ub(255,210,0);
            glVertex3f( x,     y,     z+h );
        glEnd();

        /// Рисуване на парче от основата
        glBegin( GL_POLYGON );
            glNormal3f( 0, 0, -1 );
            glVertex3f( x,     y,     z );
            glVertex3f( x+dx1, y+dy1, z );
            glVertex3f( x+dx2, y+dy2, z );
        glEnd();

        alpha += dalpha;
    }
}

void drawPine ( float x, float y, float z, float r, float h )
{
    int n = 128;

    float alpha = 0.0;
    float dalpha = 2*M_PI/n;

    for( int i=0; i<n; i++)
    {
        float dx1 = r*cos(alpha);
        float dy1 = r*sin(alpha);

        float dx2 = r*cos(alpha+dalpha);
        float dy2 = r*sin(alpha+dalpha);

        float cosbeta = h/sqrt(h*h+r*r);
        float sinbeta = r/sqrt(h*h+r*r);

        /// Рисуване на околна стена
        glBegin( GL_POLYGON );
            glNormal3f( cos(alpha)*cosbeta, sin(alpha)*cosbeta, sinbeta );
            glColor3ub(0,50,0);
            glVertex3f( x+dx1, y+dy1, z );

            glNormal3f( cos(alpha+dalpha)*cosbeta, sin(alpha+dalpha)*cosbeta, sinbeta );
            glColor3ub(0,87,0);
            glVertex3f( x+dx2, y+dy2, z );

            glNormal3f( 2*cos(alpha+dalpha/2)*cosbeta, 2*sin(alpha+dalpha/2)*cosbeta, 2*sinbeta );
            glColor3ub(0,102,0);
            glVertex3f( x,     y,     z+h );
        glEnd();


        glBegin( GL_POLYGON );
            glNormal3f( 0, 0, -1 );
            glColor3ub(0,40,0);
            glVertex3f( x,     y,     z );
            glVertex3f( x+dx1, y+dy1, z );
            glVertex3f( x+dx2, y+dy2, z );
        glEnd();

        alpha += dalpha;
    }
}

void drawCircle( float x, float y, float z, float r )
{
    int n = 64;
    float alpha = 0.0;
    float dalpha = 2*M_PI/n;

    glBegin( GL_POLYGON );
    for( int i=0; i<n; i++)
    {
        glVertex3f( x+r*cos(alpha), y+r*sin(alpha), z );
        alpha += dalpha;
    }
    glEnd();
}


void drawBench(float x, float y, float z, float height, float length, float width)
{
    glColor3ub(102,51,0);
    drawSmoothCylinder(x, y, z, height/10, height);
    drawSmoothCylinder(x, -y, z, height/10, height);
    drawSmoothCylinder(-x, y, z, height/10, height);
    drawSmoothCylinder(-x, -y, z, height/10, height);
    drawSurface(0, 0, z+2.6, length, width, 1);
}

bool running()
{
    int width, height;
    glfwGetWindowSize( &width, &height );
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 40.0f, (GLfloat)width/(GLfloat)height, 4, 100 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glfwSwapBuffers();
    glClear( GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT );
    return( !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam( GLFW_OPENED) );
}

int main()
{
    glfwInit();
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES,GL_TRUE);

    if( !glfwOpenWindow( 1000, 600, 8, 8, 8, 0, 16, 0, GLFW_WINDOW ) ) return 1;
    glfwSetWindowTitle("CAMPFIRE");

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glEnable( GL_NORMALIZE );
    glShadeModel(GL_SMOOTH);

    glMaterialf(GL_FRONT,GL_SHININESS,10);
    float F[4] = {0.4,0.3,0,1};
    glMaterialfv(GL_FRONT,GL_SPECULAR,F);


    while( running() )
    {
        float t=glfwGetTime()/20*3;
        float dist=18+3*cos(2.1*t);
        gluLookAt(dist*1.95*cos(1.9*t),dist*1.95*sin(1.9*t),dist*2.7, 0,0,1, 0,0,1 );

        ///meadow
        glColor3ub(0,153,0);
        drawSurface(0, -8, -8.5 , 36 ,60, 1);

        ///campers
        float angle=47*sinf(5*t);
        {
            float position[]={-2,-19.8,-4};
            float orientation[]={0,0};
            float scale[]={1.2,1.2,1.2,1.2};
            float waist[]={-18,0};
            float head[]={0,0,0};
            float leftHand[]={-10,55,100,50,0,-29,-100};
            float rightHand[]={10,50,-90,100,0,29,100};
            float leftLeg[]={100,-20,-100,0};
            float rightLeg[]={100,20,-100,0};
            drawBody (position,orientation,scale,waist,head,leftHand,rightHand,leftLeg,rightLeg);
        }
        {
            float position[]={10,-9,-4};
            float orientation[]={100,0};
            float scale[]={1.2,1.2,1.2,1.2};
            float waist[]={-18,0};
            float head[]={0,0,0};
            float leftHand[]={-10,80,90,10,0,-29,-100};
            float rightHand[]={11,80,-90,10,0,29,100};
            float leftLeg[]={100,-20,-100,0};
            float rightLeg[]={100,20,-100,0};
            drawBody (position,orientation,scale,waist,head,leftHand,rightHand,leftLeg,rightLeg);
         }
         {
            float position[]={-10,-8,-4};
            float orientation[]={263,0};
            float scale[]={1.2,1.2,1.2,1.2};
            float waist[]={-18,0};
            float head[]={0,0,0};
            float leftHand[]={-10,80,90,10,0,-29,-100};
            float rightHand[]={11,80,-90,10,0,29,100};
            float leftLeg[]={100,-20,-100,0};
            float rightLeg[]={100,20,-100,0};
            drawBody (position,orientation,scale,waist,head,leftHand,rightHand,leftLeg,rightLeg);
         }
         {
            float position[]={-1,0,-4};
            float orientation[]={180,0};
            float scale[]={1.2,1.2,1.2,1.2};
            float waist[]={5,0};
            float head[]={0,15,0};
            float leftHand[]={-10,30,25,60,0,50,-100};
            float rightHand[]={30,40,-50,100+fabs(angle),0,-10,100};
            float leftLeg[]={100,-2,-100,0};
            float rightLeg[]={100,2,-100,0};
            drawBody (position,orientation,scale,waist,head,leftHand,rightHand,leftLeg,rightLeg);
         }
         {
            float position[]={10,-13.7,-4};
            float orientation[]={165,0};
            float scale[]={1.2,1.2,1.2,1.2};
            float waist[]={-30,0};
            float head[]={0,0,0};
            float leftHand[]={-10,60,40,20,0,-29+fabs(angle),-60+fabs(angle)};
            float rightHand[]={20,55,-90,55,0,29,100};
            float leftLeg[]={100,-20,-100,0};
            float rightLeg[]={100,20,-100,0};
            drawBody (position,orientation,scale,waist,head,leftHand,rightHand,leftLeg,rightLeg);
         }

        ///benches
        drawBench(5.3,1,-7.5,3,12,3);

        glPushMatrix();
        glTranslatef(0,-20,0);
        drawBench(5.3,1,-7.5,3,12,3);
        glPopMatrix();

        glPushMatrix();
        glRotatef(90,0,0,1);
        glTranslatef(-10,-10,0);
        drawBench(5.3,1,-7.5,3,12,3);
        glPopMatrix();

        glPushMatrix();
        glRotatef(90,0,0,1);
        glTranslatef(-10,10,0);
        drawBench(5.3,1,-7.5,3,12,3);
        glPopMatrix();

        ///fence
        glColor3ub(224,224,224);
        for(double i=12; i>=-17.5; i=i-1.589)
            drawSurface(i,21.9,-6.2,1,0.3,4);
        drawSurface(-3,21.9,-6,30,0.3,1);
        for(double i=17.5; i>=-17.5; i=i-1.589)
            drawSurface(i,-37.9,-6.2,1,0.3,4);
        drawSurface(0,-37.9,-6,35.7,0.3,1);
        for(double i=20.65; i>=-37; i=i-1.589)
            drawSurface(17.9,i,-6.2,0.3,1,4);
        drawSurface(17.9,-8,-6,0.3,60.1,1);
        for(double i=20.65; i>=-37; i=i-1.589)
            drawSurface(-17.9,i,-6.2,0.3,1,4);
        drawSurface(-17.9,-8,-6,0.3,60.1,1);

        ///door
        for(double i=20.65; i>=16; i=i-1.589)
            drawSurface(12.5,i,-6.2,0.3,1,4);
        drawSurface(12.5,19.2,-6,0.3,5.5,1);

        ///rocks
        glColor3ub(25,25,0);
        drawCircle(0, -10, -7.9, 4);
        glColor3ub(128,128,128);
        drawSmoothEllipsoid(3.5,-10,-7.8,0.8,1.4,0.5);
        drawSmoothEllipsoid(-3.5,-10,-7.8,0.8,1.4,0.5);
        drawSmoothEllipsoid(0,-13.5,-7.8,1.4,0.8,0.5);
        drawSmoothEllipsoid(0,-6.5,-7.8,1.4,0.8,0.5);

        glPushMatrix();
        glRotatef(45,0,0,1);
        glTranslatef(-3.5,-7.11,-7.8);
        drawSmoothEllipsoid(0,0,0,0.8,1.4,0.5);
        glPopMatrix();

        glPushMatrix();
        glRotatef(45,0,0,1);
        glTranslatef(-10.5,-7.11,-7.8);
        drawSmoothEllipsoid(0,0,0,0.8,1.4,0.5);
        glPopMatrix();

        glPushMatrix();
        glRotatef(-45,0,0,1);
        glTranslatef(10.5,-7.11,-7.8);
        drawSmoothEllipsoid(0,0,0,0.8,1.4,0.5);
        glPopMatrix();

        glPushMatrix();
        glRotatef(-45,0,0,1);
        glTranslatef(3.5,-7.11,-7.8);
        drawSmoothEllipsoid(0,0,0,0.8,1.4,0.5);
        glPopMatrix();

        ///firewood that is currently "burning"
        glColor3ub(51,25,0);
        glPushMatrix();
        glRotatef(30,1,0,0);
        glTranslatef(0,-10.5,-3);
        drawSmoothCylinder(0,0,0,0.5,4);
        glPopMatrix();

        glPushMatrix();
        glRotatef(-30,1,0,0);
        glTranslatef(0,-6.5,-13);
        drawSmoothCylinder(0,0,0,0.5,4);
        glPopMatrix();

        glPushMatrix();
        glRotatef(30,0,1,0);
        glTranslatef(2,-10,-8);
        drawSmoothCylinder(0,0,0,0.5,4);
        glPopMatrix();

        glPushMatrix();
        glRotatef(-30,0,1,0);
        glTranslatef(-2,-10,-8);
        drawSmoothCylinder(0,0,0,0.5,4);
        glPopMatrix();

        ///fire
        drawFire(0, -10, -7.9, 2, 5);

        ///popcorn box
        glColor3ub(102,0,51);
        drawSurface(-1,-3,-3,1.4,1.4,2.2);
        glColor3ub(255,255,255);
        drawCircle(-1,-3,-1.89,0.56);

        ///trees
        glColor3ub(51,25,0);
        drawSmoothCylinder(-10,18,-4,1,10);
        glColor3ub(0,204,0);
        drawSmoothSphere(-10,18,6,3.5);

        glColor3ub(51,25,0);
        drawSmoothCylinder(-12,8,-4,0.8,10);
        drawPine(-12,8,0,3,11);

        glColor3ub(51,25,0);
        drawSmoothCylinder(3,17,-8,0.8,10);
        drawPine(3,17,-4,4,14);

        ///dog house
        glColor3ub(153,76,0);
        drawSurface(11,-33,-4,4,0.4,4.5);
        drawSurface(13,-30.9,-4,0.4,4.5,4.5);
        drawSurface(9,-30.9,-4,0.4,4.5,4.5);
        drawSurface(11,-30.9,-1.9,4,4.5,0.4);
        drawSurface(12.55,-28.85,-4,0.9,0.4,4.5);
        drawSurface(9.55,-28.85,-4,0.9,0.4,4.5);
        drawSurface(11,-28.85,-2.22,4,0.4,0.9);

        glColor3ub(161,0,0);
        glPushMatrix();
        glRotatef(45,0,1,0);
        glTranslatef(9.5,-30.9,8);
        drawSurface(0,0,0,4.5,5.3,0.4);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(9.5,-30.9,-1);
        glRotatef(-45,0,1,0);
        drawSurface(0,0,0,4.5,5.3,0.4);
        glPopMatrix();

        ///firewood
        glColor3ub(81,51,20);
        for(double j=0;j<=5;j=j+0.5)
        {
            for(double i=-15+j; i<=-5-j; i++)
            {
                glPushMatrix();
                glRotatef(90,1,0,0);
                glTranslatef(i,-2+j,30);
                drawSmoothCylinder(0,-5,0,0.5,3);
                glPopMatrix();
            }
        }

        ///doge
        glColor3ub(255,255,255);
        drawSmoothEllipsoid(10,-21,-6.2,1,1.7,0.5);
        drawSmoothCylinder(9.5,-21.7,-7.4,0.25,1.2);
        drawSmoothCylinder(10.5,-21.7,-7.4,0.25,1.2);
        drawSmoothCylinder(9.5,-20,-7.4,0.25,1.2);
        drawSmoothCylinder(10.5,-20,-7.4,0.25,1.2);
        drawSmoothSphere(10,-19,-5.9,0.8);
        drawSmoothEllipsoid(10,-22.5,-6.2,0.2,0.8,0.1);

    }
    glfwTerminate();
    return 0;
}

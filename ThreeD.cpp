#include <string.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
using namespace std;

#include <stdlib.h>
#include <math.h>

#include <sys/types.h>
#include "glut.h"
#include "glm/vec3.hpp"     //glm::vec3
#include "glm/vec4.hpp"     //glm::vec4
#include "glm/mat4x4.hpp"   //glm::mat4
#include "glm/gtc/matrix_transform.hpp"  //glm::translate, glm::rotate, glm::scale, glm::perspective


int z_offset=-24;
int x_offset=0;
int y_offset=0;
int oldx=0,oldy=0;
int windx=1024;
int windy=1024;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define DRAW_FRAME

class Vertex {
public:
    double x, y, z;

    Vertex( double x, double y, double z ) {
        setCoordinates( x, y, z );
    }
    void setCoordinates( double xval, double yval, double zval ) {
        x = xval; y = yval; z = zval;
    }
};

class Normal {
public:
    double x, y, z;

    Normal ( double x, double y, double z ) {
        setCoordinates( x, y, z );
    }
    void setCoordinates(double xval, double yval, double zval) {
        double l = sqrt(xval*xval + yval*yval + zval*zval);
        if (l != 0.0)
            l = 1.0 / l;
        x = l*xval;
        y = l*yval;
        z = l*zval;
    }
};

class Texel {
public:
    double u, v;

    Texel(double u, double v) {
        setCoordinates(u, v);
    }
    void setCoordinates(double uval, double vval) {
        u = uval;
        v = vval;
    }
};

class Face {
public:
    int *vList;
    int *nList;
    int *tList;
    int vIndex;
    const int DEFAULT_SIZE;
    int current_max_size;

    Face(): DEFAULT_SIZE(4) {
        vIndex = -1;
    }

    void addVertex(int v) {
        // make indices zero referenced
        add(v-1, -1, -1);
    }

    void addVertexTexel(int v, int t) {
        add(v-1, -1, t-1);
    }

    void addVertexNormal(int v, int n) {
        add(v-1, n-1, -1);
    }

    void addVertexNormalTexel(int v, int n, int t) {
        add(v-1, n-1, t-1);
    }

private:
    void add(int v, int n, int t) {
        if (vIndex < 0) {
            vList = new int[DEFAULT_SIZE];
            nList = new int[DEFAULT_SIZE];
            tList = new int[DEFAULT_SIZE];
            current_max_size = DEFAULT_SIZE;
            vIndex = 0;
        }
        vList[vIndex] = v;
        nList[vIndex] = n;
        tList[vIndex] = t;
        vIndex += 1;
        if (vIndex == current_max_size) {
            current_max_size = 2*vIndex;
            int *newV = new int[current_max_size];
            int *newN = new int[current_max_size];
            int *newT = new int[current_max_size];
            for ( int i = 0; i < vIndex; i++ ) {
                newV[i] = vList[i];
                newN[i] = nList[i];
                newT[i] = tList[i];
            }
            delete [] vList;
            delete [] nList;
            delete [] tList;
            vList = newV;
            nList = newN;
            tList = newT;
        }
    }
};


class WavefrontObj  {
public:
    Vertex **v;
    int vIndex;
    int current_max_verticies;
    
    Normal **n;
    int nIndex;
    int current_max_normals;
    
    Texel **t;
    int tIndex;
    int current_max_texels;
    
    Face **f;
    int fIndex;
    int current_max_faces;
    
    bool isFlat;
    GLuint mode;


    WavefrontObj(char *filename) : DEFAULT_SIZE(16) {
        vIndex = -1;
        nIndex = -1;
        tIndex = -1;
        fIndex = -1;

        isFlat = false;
        mode = GL_POLYGON;

        char *line = new char[200];
        char *line_back = new char[200];
        char wspace[] = {' ', '\t'};
        char separator[] = {'/'};
        char *token;
        int indices[3];
        double x, y, z;
        double tex_u, tex_v;

        ifstream file(filename);
        if ( !file ) {
            cerr <<"Cannot open file: " <<filename <<" exiting." <<endl;
            exit ( -1 );
        }
        while ( !file.eof() ) {
            file.getline( line, 199 );			
            // first, strip off comments
            if ( line[0] == '#' )
                continue;
            else if ( !strcmp( line, "" ) )
                continue;

            else {
                strcpy( line_back, line ); // strtok destroys line.
                token = strtok( line, wspace);
                if ( !strcmp( token, "v" ) ) {
                    x = atof( strtok( NULL, wspace ) );
                    y = atof( strtok( NULL, wspace ) );
                    z = atof( strtok( NULL, wspace ) );
                    addVertex( x, y, z );
                }
                else if ( !strcmp( token, "vn" ) ) {
                    x = atof( strtok( NULL, wspace ) );
                    y = atof( strtok( NULL, wspace ) );
                    z = atof( strtok( NULL, wspace ) );
                    addNormal( x, y, z );
                }
                else if ( !strcmp( token, "vt" ) ) {
                    tex_u = atof( strtok( NULL, wspace ) );
                    tex_v = atof( strtok( NULL, wspace ) );
                    addTexel(tex_u, tex_v );
                }
                else if ( !strcmp( token, "f" ) ) {
                    Face *f = addFace();
                    for (char *p = strtok( NULL, wspace ); p; p = strtok( NULL, wspace ) ) {
                        indices[0] = -1;
                        indices[1] = -1;
                        indices[2] = -1;
                        int i = 0;
                        for ( int j = 0 ; j < strlen( p ); j++ ) {
                            if ( p[j] != '/' ) {
                                if ( indices[i] == -1 )
                                    indices[i] = 0;
                                indices[i] *= 10;
                                char c[2];
                                c[0] = p[j];
                                c[1] = '\0';
                                indices[i] += atoi( c );
                            }
                            else {
                                i++;
                            }
                        }

                        if ( (indices[1] == -1) & (indices[2] == -1) ) {            
                            f->addVertex(indices[0]);                                    // num//
                        }
                        else  if ( indices[2] == -1 ) {
                            f->addVertexTexel(indices[0], indices[1]);                   // num/num/
                        }
                        else if ( indices[1] == -1 ) {
                            f->addVertexNormal( indices[0], indices[2] );                // num//num
                        }
                        else {
                            f->addVertexNormalTexel(indices[0], indices[1], indices[2]); // num/num/num
                        }
                    }
                }
                else if ( !strcmp( token, "g" ) ) {      // group
                }
                else if ( !strcmp( token, "s" ) ) {      // smoothing group
                }
                else if ( !strcmp( token, "" ) ) {       // blank line
                }
                else {
                    cout <<line_back <<endl;
                }
            }
        }
    }

    void addVertex(double x, double y, double z) {
        Vertex *vert = new Vertex(x, y, z);
        addVertex(vert);
    }

    void addVertex(Vertex *vert) {
        if (vIndex < 0) {
            v = new Vertex*[DEFAULT_SIZE];
            vIndex = 0;
            current_max_verticies = DEFAULT_SIZE;
        }
        v[vIndex] = vert;
        vIndex += 1;
        if (vIndex == current_max_verticies) {
            current_max_verticies = 2*vIndex;
            Vertex **newV = new Vertex*[current_max_verticies];
            for ( int i = 0; i < vIndex; i++ )
                newV[i] = v[i];
            delete [] v;
            v = newV;
        }
    }

    void addNormal(double x, double y, double z) {
        Normal *norm = new Normal(x, y, z);
        addNormal(norm);
    }

    void addNormal(Normal *norm) {
        if (nIndex < 0) {
            n = new Normal*[DEFAULT_SIZE];
            nIndex = 0;
            current_max_normals = DEFAULT_SIZE;
        }
        n[nIndex] = norm;
        nIndex += 1;
        if (nIndex == current_max_normals) {
            current_max_normals = 2*nIndex;
            Normal **newN = new Normal*[current_max_normals];
            for ( int i = 0; i < nIndex; i++ )
                newN[i] = n[i];
            delete [] n;
            n = newN;
        }
    }

    void addTexel(double u, double v) {
        Texel *texl = new Texel(u, v);
        addTexel(texl);
    }

    void addTexel(Texel *texl) {
        if (tIndex < 0) {
            t = new Texel*[DEFAULT_SIZE];
            tIndex = 0;
            current_max_texels = DEFAULT_SIZE;
        }
        t[tIndex] = texl;
        tIndex += 1;
        if (tIndex == current_max_texels) {
            current_max_texels = 2*tIndex;
            Texel **newT = new Texel*[current_max_texels];
            for ( int i = 0; i < tIndex; i++ )
                newT[i] = t[i];
            delete [] t;
            t = newT;
        }
    }
    
    Face *addFace( ) {
        if (fIndex < 0) {
            f = new Face*[DEFAULT_SIZE];
            fIndex = 0;
            current_max_faces = DEFAULT_SIZE;
        }
        f[fIndex] = new Face();
        fIndex += 1;
        if (fIndex == current_max_faces) {
            current_max_faces = 2*fIndex;
            Face **newF = new Face*[current_max_faces];
            for ( int i = 0; i < fIndex; i++ )
                newF[i] = f[i];
            delete [] f;
            f = newF;
        }
        return f[fIndex - 1];
    }

    Normal *faceNormal(Vertex *v0, Vertex *v1, Vertex *v2) {
        double ax, ay, az;
        double bx, by, bz;
        double nx, ny, nz;
        
        ax = v1->x - v0->x;
        ay = v1->y - v0->y;
        az = v1->z - v0->z;
        
        bx = v2->x - v0->x;
        by = v2->y - v0->y;
        bz = v2->z - v0->z;
        
        nx = ay*bz - az*by;
        ny = az*bx - ax*bz;
        nz = ax*by - ay*bx;
        
        return new Normal(nx, ny, nz);
    }

void Draw() {
    int face, vertex, i;
    for (face = 0; face < fIndex; face++) {
        Face *currentFace = f[face];
        glBegin(mode);
        for (vertex = 0; vertex < currentFace->vIndex; vertex++) {
            if (isFlat) {
                if (vertex == 0) {
                    Normal *norm = faceNormal(v[currentFace->vList[0]], v[currentFace->vList[1]], v[currentFace->vList[2]]);
                    glNormal3d(norm->x, norm->y, norm->z);
                }
            }
            else if ((i = currentFace->nList[vertex]) >= 0) {
                if ( i < nIndex )
                    glNormal3d(n[i]->x, n[i]->y, n[i]->z);
                else  {
    //                      cerr <<"Error i = " <<i <<" nIndex = " <<nIndex <<endl;
                }
            }
            else if (vertex == 0) {
                Normal *norm = faceNormal(v[currentFace->vList[0]], v[currentFace->vList[1]], v[currentFace->vList[2]]);
                currentFace->nList[0] = nIndex;
                addNormal(norm);
                glNormal3d(norm->x, norm->y, norm->z);
            }
            if ((i = currentFace->tList[vertex]) >= 0) {
                glTexCoord2d(t[i]->u, t[i]->v);
            }
            i = currentFace->vList[vertex];
            if ( i < vIndex )
                glVertex3d(v[i]->x, v[i]->y, v[i]->z);
            else {
//                  cerr <<"Error i = " <<i <<" vIndex = " <<vIndex <<endl;
            }
        }
        glEnd();
    }
}
private:
    const int DEFAULT_SIZE;
};


double rotation = 0.0;
double yangle=0.0;
double xangle=0.0;

WavefrontObj *data;
bool firstTime = true;

void Init( int w, int h ) {
    glViewport(0, 0, w, h);
    glShadeModel(GL_SMOOTH);								// Set Smooth Shading 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			    	// Background Color 
    glClearDepth(1.0f);										// Depth buffer setup 
    glEnable(GL_DEPTH_TEST);								// Enables Depth Testing 
    glDepthFunc(GL_LEQUAL);									// The Type Of Depth Test To Do 
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);		// Use perspective correct interpolation if available

    /*
      glFrontFace(GL_CCW);
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
    */

    glMatrixMode(GL_PROJECTION);							// Select The Projection Matrix
    glLoadIdentity();										// Reset The Projection Matrix
    double aspect = (double) h/w;
    glFrustum(-5, 5, -5*aspect, 5*aspect, 10, 74);          // Define perspective projection frustum
	//gluPerspective(30, w/h, 10, 74);

    glMatrixMode(GL_MODELVIEW);								// Select The Modelview Matrix
    glLoadIdentity();										// Reset The Modelview Matrix
    
    if (firstTime) {
        glEnable(GL_LIGHTING);
        float frontColor[] = {0.2f, 0.7f, 0.7f, 1.0f};        
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);
		glMaterialfv(GL_FRONT, GL_SPECULAR, frontColor);
		glMaterialf(GL_FRONT, GL_SHININESS, 100);
        
        float lightDirection[] = {2.0f, 0.0f, 1.0f, 0};
        float ambientIntensity[] = {0.1f, 0.1f, 0.1f, 1.0f};
        float lightIntensity[]   = {0.9f, 0.9f, 0.9f, 1.0f};
        float lightSpec[] = {1.0f, 1.0f, 1.0f, 1};
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientIntensity);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
        glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
        glEnable(GL_LIGHT0);
        firstTime = false;
    }
}

void DrawFrame()
{
	glBegin(GL_LINES);
	glVertex3d(0,0,0);
	glVertex3d(6,0,0);

	glVertex3d(0,0,0);
	glVertex3d(0,6,0);
	glVertex3d(0,0,0);
	glVertex3d(0.2,6,0);

	glVertex3d(0,0,0);
	glVertex3d(0,0,6);
	glVertex3d(0,0,0);
	glVertex3d(0,0.2,6);
	glVertex3d(0,0,0);
	glVertex3d(0,0.4,6);
	glEnd(); 

	glutWireCube(10);
}

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glLoadIdentity();
    glTranslated(x_offset, y_offset, z_offset);                          // Viewing transformation
    glRotated(xangle, 0.0, 1.0, 0.0);               
    glRotated(yangle, 1.0, 0.0, 0.0);   //use yangle
    if (data != NULL)
        data->Draw();
    else
        glutSolidTeapot(1.0);	//draw a teapot when no argument is provided

#ifdef DRAW_FRAME
	DrawFrame();
#endif
    glFlush();
    glutSwapBuffers();


}


//
// GLUT keypress function
// 
void onKeyPress(unsigned char key, int x, int y) {
    cout<<"key is "<<key<<endl;
    if ( key == 'r' ) {      // request transformation from user
        rotation += 2;
    }
    else if ( key == 'w' ) { // quit the program.
        data->mode = GL_LINE_LOOP;
    }
    else if ( key == 'p' ) { // quit the program.
        data->mode = GL_POINTS;
    }
    else if ( key == 's' ) { // quit the program.
        glShadeModel(GL_SMOOTH);								// Set Smooth Shading 
        data->mode = GL_POLYGON;
    }
    else if ( key == 'f' ) { // quit the program.
        glShadeModel(GL_FLAT);								// Set Smooth Shading 
        data->mode = GL_POLYGON;
    }
    else if ( key == 'q' ) { // quit the program.
        delete data;
        exit( 0 );
    }
    else if ( key == 'j' ) { // quit the program.
        x_offset--;
    }
    else if ( key == 'l' ) { // quit the program.
        x_offset++;
    }
    else if(key=='i'){
        y_offset++;
    }
    else if(key=='m'){
        y_offset--;
    }
    else if(key=='u'){
        z_offset++;
    }
    else if(key=='n'){
        z_offset--;
    }

    glutPostRedisplay();
}

void cbkMouse(int button, int state, int x, int y){
	printf("(%d, %d)\n", x, y);
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        oldx=x;
        oldy=y;
        cout<<"the left mouse is clicking"<<endl;
    }
    else if(button==GLUT_LEFT_BUTTON && state==GLUT_UP){
        cout<<"the right mouse is clicking"<<endl;
    }

}
void motion(int x, int y)  
{  
    cout<<"x is "<<x<<" y is  "<<y<<endl;
    int deltax = oldx - x;  
    int deltay = oldy - y;  
    xangle -= 2*360*float(deltax) / windx;   //if up to down, xangle should be negative
    yangle -= 2*360*float(deltay) / windy;  //if up to down, yangle should be negative
    oldx = x;
    oldy = y;
    glutPostRedisplay();  
}

// void motion(int x, int y)  {}
void cbkTimer(int e){
	// rotation += 0.5;
	glutPostRedisplay();
	glutTimerFunc(10, cbkTimer, 1);
}


int main( int argc, char *argv[] ) {
    // window size parameters
    int w = 1024; int h = 1024;

    // glut initialization functions:
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize(w,h);
    glutInitWindowPosition(100,100);
    glutCreateWindow("ImageViewer");

    Init(w, h);

    // display, onMouseButton, mouse_motion, onKeyPress, and resize are functions defined above
    glutDisplayFunc(Draw);       
    glutKeyboardFunc(onKeyPress);
	glutMouseFunc(cbkMouse);
    glutMotionFunc(motion);
    glutReshapeFunc(Init);
	glutTimerFunc(10, cbkTimer, 1);

    if ( argc >= 2 ) 
        data = new WavefrontObj( argv[1] );
    else
        data = NULL;

    // start glutMainLoop -- infinite loop

    glutMainLoop();

    return 0;
}

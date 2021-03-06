#include "camera.hpp"
#include <cmath>

Camera * Camera::_activeCamera = NULL;

Camera::Camera( GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ):
    _eye(eyeX, eyeY, eyeZ),
    _phi(0), _theta(0), _haut(0,0,1), 
    _avant_presse(false), _arriere_presse(false), _gauche_presse(false), _droite_presse(false), _haut_presse(false), _bas_presse(false),
    _vitesse(0.30f)
{
    if (!_activeCamera)
    {
        _activeCamera = this;
    }
    vectorFromAngle();
}

void Camera::move()
{

    if (_avant_presse)
    {
        _eye = _eye + _avant * _vitesse;
    }
    if (_gauche_presse)
    {
        _eye = _eye + _gauche * _vitesse;
    }
    if (_arriere_presse)
    {
        _eye = _eye - _avant * _vitesse;
    }
    if (_droite_presse)
    {
        _eye = _eye - _gauche * _vitesse;
    }
    if (_haut_presse)
    {
        _eye = _eye + _haut * _vitesse;
    }
    if (_bas_presse)
    {
        _eye = _eye - _haut * _vitesse;
    }


    _center = _eye + _avant;
}

void Camera::go(GLdouble x, GLdouble y, GLdouble z)
{
    _eye.set(x, y, z);
    _center = _eye + _avant;
}

void Camera::mouse_event(GLFWwindow * w, double x, double y)
{
    int width, height;
    glfwGetWindowSize(w, &width, &height);
    width /= 2;
    height /= 2;


    if ((x - width != 0) ||  (y - height != 0))
    {

        _activeCamera->_theta    -= (x - width)*0.2f;
        _activeCamera->_phi      -= (y - height)*0.2f;

        _activeCamera->vectorFromAngle();

        #ifdef __APPLE__    
            int xpos, ypos;
            glfwGetWindowPos(w, &xpos, &ypos);

            CGPoint warpPoint = CGPointMake(width + xpos, height + ypos);
            CGWarpMouseCursorPosition(warpPoint);
            CGAssociateMouseAndMouseCursorPosition(true);
        #else
            glfwSetCursorPos (w, width, height);
        #endif
    }
}

void Camera::vectorFromAngle()
{
    static Vect3D up(0.0f,0.0f,1.0f);

    if (_phi > 89.0f)
    {
        _phi = 89.0f;
    }
    else if (_phi < -89.0f)
    {
        _phi = -89.0f;
    }


    double tmp = cos(_phi * M_PI/180);


    _avant.set(tmp * cos(_theta * M_PI/180), tmp * sin(_theta * M_PI/180), sin(_phi * M_PI/180));
    _avant.normalize();

    _gauche = up * _avant;
    _gauche.normalize();

    _center = _eye + _avant;
}

void Camera::display()
{
    move();
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear The Screen And The Depth Buffer
    glLoadIdentity();


    //calcul de la matrice de gluLookAt (deprecated)
    Vect3D up = -_gauche * _avant;

    GLfloat M[] =
    {
        -_gauche[0], up[0], -_avant[0], 0.0f,
        -_gauche[1], up[1], -_avant[1], 0.0f,
        -_gauche[2], up[2], -_avant[2], 0.0f,
        0.0f,       0.0f,   0.0f,      1.0f
    };

    glMultMatrixf(M);
    glTranslated(-_eye[0], -_eye[1], -_eye[2]);

}

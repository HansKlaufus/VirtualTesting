#include "glcanvas.h"
#include "maingui.h"
#include "node.h"
#include "vector.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <typeinfo>
#include <vector>
#include <wx/wx.h>
#include <wx/glcanvas.h>

BEGIN_EVENT_TABLE(cGLCanvas, wxGLCanvas)
    EVT_MOTION(cGLCanvas::mouseMoved)
    EVT_LEFT_DOWN(cGLCanvas::onLeftDown)
    EVT_LEFT_UP(cGLCanvas::onLeftReleased)
    EVT_RIGHT_DOWN(cGLCanvas::rightClick)
    EVT_LEAVE_WINDOW(cGLCanvas::mouseLeftWindow)
    EVT_SIZE(cGLCanvas::onSizeEvent)
    EVT_KEY_DOWN(cGLCanvas::keyPressed)
    EVT_KEY_UP(cGLCanvas::keyReleased)
    EVT_MOUSEWHEEL(cGLCanvas::onMouseWheelEvent)
    EVT_PAINT(cGLCanvas::onPaintEvent)
END_EVENT_TABLE()


cGLCanvas::cGLCanvas(wxFrame* parent, int* args) : wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
	m_parent       = parent;
	m_context      = new wxGLContext(this);

    m_cameraX      = 0;
    m_cameraY      = 0;
    m_cameraZ      = 0;

    m_cameraAngleX = 0;
    m_cameraAngleY = 0;
    m_cameraAngleZ = 0;

    m_upX          = 0;
    m_upY          = 0;
    m_upZ          = 0;

    m_radius       = 1.0;
    m_initialised  = false;

    m_showBackgroundRaster = true;
    m_showAxisFrame        = true;
    m_showSpheres          = true;

    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
//    initGL();
}

cGLCanvas::~cGLCanvas()
{
	delete m_context;
}

void cGLCanvas::mouseMoved(wxMouseEvent& event)
{
    // Rotate
    static bool  dragging = false;
    static float xOld;
    static float yOld;

    if(event.LeftIsDown()) {
        if(!dragging) {
          dragging = true;
        } else {
          m_cameraAngleY = m_cameraAngleY + (yOld - event.GetY())/getHeight()*360;
          m_cameraAngleZ = m_cameraAngleZ + (xOld - event.GetX())/getWidth()*360;
          Refresh(false);
        }

        xOld = event.GetX();
        yOld = event.GetY();

    } else if(event.RightIsDown()) {
        if(!dragging) {
          dragging = true;
        } else {
          m_cameraAngleX = m_cameraAngleX + (xOld - event.GetX())/getWidth()*360;
          Refresh(false);
        }

        xOld = event.GetX();
        yOld = event.GetY();

    } else {
        dragging = false;
    }
}

void cGLCanvas::onMouseWheelEvent(wxMouseEvent& event)
{
    if (event.GetWheelRotation()>0) {
        m_cameraX *= 1.1;
        m_cameraY *= 1.1;
        m_cameraZ *= 1.1;
    } else {
        m_cameraX /= 1.1;
        m_cameraY /= 1.1;
        m_cameraZ /= 1.1;
    }

//    m_cameraX = m_cameraX + event.GetWheelRotation();
//    m_cameraY = m_cameraY + event.GetWheelRotation();
//    m_cameraZ = m_cameraZ + event.GetWheelRotation();

    wxString message = wxString::Format(wxT("Camera Position = (%8.3f, %8.3f, %8.3f)"), m_cameraX, m_cameraY, m_cameraZ);
    ((cMainGUI*) m_parent)->SetStatus(message, 0);

    redraw();
}

void cGLCanvas::onLeftDown(wxMouseEvent& event){}
void cGLCanvas::onLeftReleased(wxMouseEvent& event) {}
void cGLCanvas::rightClick(wxMouseEvent& event) {}
void cGLCanvas::mouseLeftWindow(wxMouseEvent& event) {}
void cGLCanvas::keyPressed(wxKeyEvent& event) {}
void cGLCanvas::keyReleased(wxKeyEvent& event) {}

void cGLCanvas::onSizeEvent(wxSizeEvent& evt)
{
//	wxGLCanvas::OnSize(evt);
    Refresh();
}

void cGLCanvas::onPaintEvent( wxPaintEvent& evt )
{
    if(!IsShown()) return;

    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
    redraw();
}

void cGLCanvas::initGL(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // Black Background
    glClearDepth(1.0f);                                 // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Enable the shade model
    glShadeModel(GL_SMOOTH);

    // Enable lighting models
    glEnable(GL_LIGHTING);

    // Define and enable global ambient light
//    GLfloat globalAmbientLight[] = { 0.5f, 0.5f, 0.5f};
//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);

    // Create light components
    GLfloat ambientLight[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[]      = { 0.0f, 0.0f, 0.0f, 1.0f };

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);     // Define ambient light component
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);     // Define diffuse light component
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);   // Define specular light component
    glLightfv(GL_LIGHT0, GL_POSITION, position);        // Define position of light0
    glEnable(GL_LIGHT0);                                // Enable light0


    // Enable Color Material; to be used in conjunction with glColor only
//    glEnable(GL_COLOR_MATERIAL);                        // Enable colour tracking
//    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  // Use predefined material properties; set by glColor


    m_initialised = true;
}

void cGLCanvas::setProjectionMatrix(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    GLdouble left   = -1.5*m_radius;
    GLdouble right  = +1.5*m_radius;
    GLdouble bottom = -1.5*m_radius;
    GLdouble top    = +1.5*m_radius;

    GLdouble aspect = (GLdouble)(bottomrigth_x - topleft_x) / (GLdouble)(bottomrigth_y - topleft_y);
    if ( aspect < 1.0 ) { // window taller than wide
        bottom /= aspect;
        top /= aspect;
    } else {
        left *= aspect;
        right *= aspect;
    }

    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(2*m_radius<20000) {
        gluPerspective(45, aspect, 0.1, 20000);
    } else {
        gluPerspective(45, aspect, 0.1, 2*m_radius);
    }
}

void cGLCanvas::setModelMatrix(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(m_cameraX, m_cameraY, m_cameraZ, 0, 0, 0, m_upX, m_upY, m_upZ);
    glRotatef(m_cameraAngleX, 1, 0, 0);
    glRotatef(m_cameraAngleY, 0, 1, 0);
    glRotatef(m_cameraAngleZ, 0, 0, 1);
}

int cGLCanvas::getWidth()
{
    return GetSize().x;
}

int cGLCanvas::getHeight()
{
    return GetSize().y;
}

void cGLCanvas::drawBackgroundRaster(void)
{
    /*
    ** Draw a wire sphere as background raster
    */
    glPushMatrix();
        GLfloat redEmissiveMaterial[] = {1.0f, 0.0f, 0.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, redEmissiveMaterial);

        double r = sqrt(m_cameraX*m_cameraX + m_cameraY*m_cameraY + m_cameraZ*m_cameraZ);
        glBegin(GL_LINE_LOOP);
            GLUquadricObj *quadric = gluNewQuadric();
            gluQuadricDrawStyle(quadric, GLU_LINE);
            gluSphere(quadric, r, 36, 36);
            gluDeleteQuadric(quadric);
        glEnd();
    glPopMatrix();
}

void cGLCanvas::drawAxisFrame(void)
{
    /*
    ** Draw an axes frame
    */
    GLfloat v_RedEmissiveMaterial[] = {1.0, 0.0, 0.0};
    GLfloat v_GreenEmissiveMaterial[] = {0.0, 1.0, 0.0};
    GLfloat v_BlueEmissiveMaterial[] = {0.0, 0.0, 1.0};

    cVector v_Origin(0.0f, 0.0f, 0.0f);
    cVector v_XAxis(m_radius, 0.0f, 0.0f);
    cVector v_YAxis(0.0f, m_radius, 0.0f);
    cVector v_ZAxis(0.0f, 0.0f, m_radius);

    this->drawVector(v_Origin, v_XAxis, v_RedEmissiveMaterial);
    this->drawVector(v_Origin, v_YAxis, v_GreenEmissiveMaterial);
    this->drawVector(v_Origin, v_ZAxis, v_BlueEmissiveMaterial);
}

void cGLCanvas::drawNode(cNode* p_Node)
{
    cVector v_Position = p_Node->GetPosition();
    GLdouble v_Radius = 0.1; //p_Node->GetMass()/10;
    if(v_Radius<0.1) v_Radius = 0.1;
    if(v_Radius>1.0) v_Radius = 1.0;

    GLfloat v_YellowMaterial[] = {0.5f, 0.5f, 0.0f};
    GLfloat v_RedMaterial[] = {0.5f, 0.0f, 0.0f};
    GLfloat v_BlueMaterial[] = {0.0f, 0.0f, 0.5f};

    glPushMatrix();
        std::string v_ClassType = typeid(*p_Node).name();
        if(v_ClassType.find("cStaticNode")<v_ClassType.size())
        {
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, v_RedMaterial);
        } else if(v_ClassType.find("cPositionNode")<v_ClassType.size()) {
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, v_BlueMaterial);
        } else {
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, v_YellowMaterial);
        }

        glTranslatef(v_Position.at(0), v_Position.at(1), v_Position.at(2));

        // Draw a solid sphere
        glBegin(GL_LINE_LOOP);
            GLUquadricObj *v_Quadric = gluNewQuadric();
            gluQuadricDrawStyle(v_Quadric, GLU_FILL);
            gluSphere(v_Quadric, v_Radius, 20, 20);
            gluDeleteQuadric(v_Quadric);
        glEnd();

        glTranslatef(-v_Position.at(0), -v_Position.at(1), -v_Position.at(2));
    glPopMatrix();
}

void cGLCanvas::drawConnection(cConnection* p_Connection)
{
    cObject* v_Object = ((cMainGUI*)m_parent)->GetObjectPointer();

    cNode* v_Node = v_Object->GetNodePointerByID(p_Connection->GetNodeId1());

    cVector v_TranslationVector  = v_Node->GetPosition();
//    cVector v_DisplacementVector = p_Connection->GetDisplacementVector();
    cVector v_DisplacementVector = p_Connection->GetPosition();
    cVector v_ForceVector        = p_Connection->GetForce();

    cVector v_VerticalVector(0.0f, 0.0f, 1.0f);
    cVector v_RotationVector = v_VerticalVector.OuterProduct(v_DisplacementVector);
    if(v_RotationVector.GetLength()<0.001)
    {
        // Displacement vector and Vertical vector are parallel; take alternative Rotation vector
        v_RotationVector.clear();
        v_RotationVector.push_back(0.0f);
        v_RotationVector.push_back(1.0f);
        v_RotationVector.push_back(0.0f);
    }

    double v_Angle = v_VerticalVector.GetAngle(v_DisplacementVector);
    double v_Length = v_DisplacementVector.GetLength();
    double v_Radius = 0.05;//*(p_Connection->GetSpringConstant() + p_Connection->GetDampingConstant());
    double v_Ratio = fabs(v_ForceVector.GetLength())/5; // TODO: Replace 5000 with logical range
    if (v_Ratio > 0.5) v_Ratio = 0.5;

    glPushMatrix();
        GLfloat v_StrainMaterial[] = {v_Ratio, 0.5-v_Ratio, 0.0};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, v_StrainMaterial);

        glTranslated(v_TranslationVector.at(0), v_TranslationVector.at(1), v_TranslationVector.at(2));
        glRotated(v_Angle*180/M_PI, v_RotationVector.at(0), v_RotationVector.at(1), v_RotationVector.at(2));

        GLUquadricObj *v_Quadric = gluNewQuadric();
        gluQuadricDrawStyle(v_Quadric, GLU_FILL);
        gluCylinder(v_Quadric, v_Radius, v_Radius, v_Length, 20, 20);

        glRotated(-v_Angle*180/M_PI, v_RotationVector.at(0), v_RotationVector.at(1), v_RotationVector.at(2));
        glTranslated(-v_TranslationVector.at(0), -v_TranslationVector.at(1), -v_TranslationVector.at(2));

        gluDeleteQuadric(v_Quadric);
    glPopMatrix();
}

void cGLCanvas::drawActuator(cActuator* p_Actuator)
{
    cObject* v_Object = ((cMainGUI*)m_parent)->GetObjectPointer();

    cNode* v_Node = v_Object->GetNodePointerByID(p_Actuator->GetNodeId1());

    cVector v_TranslationVector = v_Node->GetPosition();
    cVector v_DisplacementVector = p_Actuator->GetPosition();

    cVector v_VerticalVector(0.0f, 0.0f, 1.0f);
    cVector v_RotationVector = v_VerticalVector.OuterProduct(v_DisplacementVector);
    if(v_RotationVector.GetLength()<0.001)
    {
        // Displacement vector and Vertical vector are parallel; take alternative Rotation vector
        v_RotationVector.clear();
        v_RotationVector.push_back(0.0f);
        v_RotationVector.push_back(1.0f);
        v_RotationVector.push_back(0.0f);
    }

    double v_Angle            = v_VerticalVector.GetAngle(v_DisplacementVector);
    double v_LengthBase       = p_Actuator->GetBaseLength();
    double v_LengthConnection = 0.2;//(v_DisplacementVector.GetLength() - v_LengthBase) - v_LengthRod;
    double v_LengthRod        = (v_DisplacementVector.GetLength() - v_LengthBase)-v_LengthConnection;
    double v_RadiusRod        = 0.05;
    double v_RadiusConnection = v_RadiusRod;
    double v_RadiusBase       = 2*v_RadiusRod;

    glPushMatrix();
        GLUquadricObj *v_Quadric = gluNewQuadric();
        gluQuadricDrawStyle(v_Quadric, GLU_FILL);

        // Draw Actuator Base
        GLfloat v_BaseMaterial[] = {0.0f, 0.0f, 0.5f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, v_BaseMaterial);

        glTranslatef(v_TranslationVector.at(0), v_TranslationVector.at(1), v_TranslationVector.at(2));
        glRotated(v_Angle*180/M_PI, v_RotationVector.at(0), v_RotationVector.at(1), v_RotationVector.at(2));

        gluCylinder(v_Quadric, v_RadiusBase, v_RadiusBase, v_LengthBase, 20, 20);

        // Draw the first cap on base
        gluDisk(v_Quadric, 0.0, v_RadiusBase, 1, 1);

        // Draw the second cap on base
        glTranslatef(0, 0, v_LengthBase);
        gluDisk(v_Quadric, 0.0, v_RadiusBase, 1, 1);

        // Draw Actuator Rod
        GLfloat v_RodDiffuse[] = {0.4f, 0.4f, 0.4f};
        GLfloat v_RodAmbient[] = {0.25f, 0.25f, 0.25f};
        GLfloat v_RodSpecular[] = {0.77f, 0.77f, 0.77f};
        GLfloat v_Shininess = 0.6f;

        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, v_RodDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, v_RodAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, v_RodSpecular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, v_Shininess*128);

        gluCylinder(v_Quadric, v_RadiusRod, v_RadiusRod, v_LengthRod, 20, 20);

        // Draw the first cap on rod
        gluDisk(v_Quadric, 0.0, v_RadiusRod, 1, 1);

        // Draw the connection
        glTranslatef(0, 0, v_LengthRod);
        gluCylinder(v_Quadric, v_RadiusConnection, 0, v_LengthConnection, 20, 20);

        glTranslatef(0, 0, -v_LengthRod);
        glTranslatef(0, 0, -v_LengthBase);

        glRotated(-v_Angle*180/M_PI, v_RotationVector.at(0), v_RotationVector.at(1), v_RotationVector.at(2));
        glTranslatef(-v_TranslationVector.at(0), -v_TranslationVector.at(1), -v_TranslationVector.at(2));

        gluDeleteQuadric(v_Quadric);
    glPopMatrix();
}

void cGLCanvas::drawConnection2(cConnection* p_Connection)
{
    cObject* v_Object = ((cMainGUI*)m_parent)->GetObjectPointer();

    cNode* v_Node1 = v_Object->GetNodePointerByID(p_Connection->GetNodeId1());
    cNode* v_Node2 = v_Object->GetNodePointerByID(p_Connection->GetNodeId2());

    cVector v_Position1 = v_Node1->GetPosition();
    cVector v_Position2 = v_Node2->GetPosition();
    cVector v_Force     = p_Connection->GetForce();

    double v_Ratio = v_Force.GetLength()/ 5; // TODO: Replace 5000 with logical range
    GLfloat v_EmissiveMaterial[] = {v_Ratio, 1.0-v_Ratio, 0.0};

    glBegin(GL_LINES);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, v_EmissiveMaterial);

        glVertex3d(v_Position1.at(0), v_Position1.at(1), v_Position1.at(2));
        glVertex3d(v_Position2.at(0), v_Position2.at(1), v_Position2.at(2));
    glEnd();
}

void cGLCanvas::drawVector(cVector p_StartPoint, cVector p_EndPoint, GLfloat p_Material[])
{
    cVector v_VerticalVector(0.0f, 0.0f, 1.0f);
    cVector v_DisplacementVector = p_EndPoint - p_StartPoint;
    cVector v_RotationVector = v_VerticalVector.OuterProduct(v_DisplacementVector);

    double v_Angle = v_VerticalVector.GetAngle(v_DisplacementVector);

    double v_ArrowLength1 = 0.9*v_DisplacementVector.GetLength();
    double v_ArrowLength2 = 0.1*v_DisplacementVector.GetLength();
    double v_ArrowRadius1 = 0.01*v_ArrowLength1;
    double v_ArrowRadius2 = 0.02*v_ArrowLength1;

    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, p_Material);

        GLUquadricObj *v_Quadric = gluNewQuadric();
        gluQuadricDrawStyle(v_Quadric, GLU_FILL);

        glTranslatef(p_StartPoint.at(0), p_StartPoint.at(1), p_StartPoint.at(2));
        glRotated(v_Angle*180/M_PI, v_RotationVector.at(0), v_RotationVector.at(1), v_RotationVector.at(2));

        gluCylinder(v_Quadric, v_ArrowRadius1, v_ArrowRadius1, v_ArrowLength1, 20, 20);

        glTranslatef(0, 0, v_ArrowLength1);
        gluCylinder(v_Quadric, v_ArrowRadius2, 0, v_ArrowLength2, 20, 20);

        glTranslatef(0, 0, -v_ArrowLength1);

        glRotated(-v_Angle*180/M_PI, v_RotationVector.at(0), v_RotationVector.at(1), v_RotationVector.at(2));
        glTranslatef(-p_StartPoint.at(0), -p_StartPoint.at(1), -p_StartPoint.at(2));

        gluDeleteQuadric(v_Quadric);
    glPopMatrix();
}

void cGLCanvas::drawForceInputVector(cNode* p_Node)
{
//    cVector v_Position = p_Node->GetPosition();
//
//    // Draw a force input vector
//    cVector v_ForceVector = p_Node->GetForceInput();
//    if(v_ForceVector.GetLength()>0.01)
//    {
//        GLfloat v_BlueEmissiveMaterial[] = {0.0, 0.0, 1.0};
//        cVector v_EndPoint = v_Position + v_ForceVector;
//        this->drawVector(v_Position, v_EndPoint, v_BlueEmissiveMaterial);
//    }
}

void cGLCanvas::drawForceVector(cNode* p_Node)
{
    cVector v_Position = p_Node->GetPosition();

    double v_Mass = p_Node->GetMass();
    if(v_Mass>0)
    {
        cVector v_ForceVector = p_Node->GetForce();
        if(v_ForceVector.GetLength()>0.01)
        {
            GLfloat v_RedEmissiveMaterial[] = {1.0f, 0.0f, 0.0f};
            cVector v_EndPoint = v_Position + v_ForceVector;
            this->drawVector(v_Position, v_EndPoint, v_RedEmissiveMaterial);
        }
    }
}

void cGLCanvas::redraw(void)
{
    initGL();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        SetCurrent(*m_context);
        setProjectionMatrix(0, 0, getWidth(), getHeight());
        setModelMatrix();

        if(m_showBackgroundRaster) drawBackgroundRaster();
        if(m_showAxisFrame) drawAxisFrame();

        cObject* v_Object = ((cMainGUI*)m_parent)->GetObjectPointer();
        if(v_Object)
        {
            // Draw Actuators
            for(unsigned int v_Index=0; v_Index<v_Object->GetNumberActuators(); v_Index++)
            {
                cActuator* v_Actuator = v_Object->GetActuatorPointerByIndex(v_Index);
                this->drawActuator(v_Actuator);
            }

            unsigned int v_NumberConnections = v_Object->GetNumberConnections();
            if(m_showSpheres)
            {
                // Draw connections as cylinders
                for(unsigned int v_Index=0; v_Index<v_NumberConnections; v_Index++)
                {
                    cConnection* v_Connection = v_Object->GetConnectionPointerByIndex(v_Index);
                    this->drawConnection(v_Connection);
                }
            }
            else
            {
                // Draw connections as lines
                glBegin(GL_LINES);
                    GLfloat redEmissiveMaterial[] = {1.0f, 0.0f, 0.0f};
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, redEmissiveMaterial);
                    for(unsigned int v_Index=0; v_Index<v_NumberConnections; v_Index++)
                    {
                        cConnection* v_Connection = v_Object->GetConnectionPointerByIndex(v_Index);

                        cNode* v_Node1 = v_Object->GetNodePointerByID(v_Connection->GetNodeId1());
                        cNode* v_Node2 = v_Object->GetNodePointerByID(v_Connection->GetNodeId2());

                        cVector v_Position1 = v_Node1->GetPosition();
                        cVector v_Position2 = v_Node2->GetPosition();

                        glVertex3d(v_Position1.at(0), v_Position1.at(1), v_Position1.at(2));
                        glVertex3d(v_Position2.at(0), v_Position2.at(1), v_Position2.at(2));
                    }
                glEnd();
            }

            unsigned int v_NumberNodes = v_Object->GetNumberNodes();
            if(m_showSpheres)
            {
                // Draw Nodes as spheres
                for(unsigned int v_Index=0; v_Index<v_NumberNodes; v_Index++)
                {
                    cNode* v_Node = v_Object->GetNodePointerByIndex(v_Index);
                    this->drawNode(v_Node);

                    if(m_showForceVectors) this->drawForceVector(v_Node);
                    if(m_showForceInputVectors) this->drawForceInputVector(v_Node);
                }
            }
            else
            {
                // Draw Nodes as dots
                glBegin(GL_POINTS);
                    GLfloat yellowEmissiveMaterial[] = {1.0f, 0.0f, 0.0f};
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellowEmissiveMaterial);
                    for(unsigned int v_Index=0; v_Index<v_NumberNodes; v_Index++)
                    {
                        cNode* v_Node = v_Object->GetNodePointerByIndex(v_Index);
                        cVector v_Position = v_Node->GetPosition();

                        glVertex3d(v_Position.at(0), v_Position.at(1), v_Position.at(2));
                    }
                glEnd();
            }
        }
    glPopMatrix();

    glFlush();
    SwapBuffers();
}

void cGLCanvas::setCameraPosition(double x, double y, double z, double upX, double upY, double upZ)
{
    m_cameraX = x;
    m_cameraY = y;
    m_cameraZ = z;

    m_upX = upX;
    m_upY = upY;
    m_upZ = upZ;
}

void cGLCanvas::setCameraAngle(double angleX, double angleY, double angleZ)
{
    m_cameraAngleX = angleX;
    m_cameraAngleY = angleY;
    m_cameraAngleZ = angleZ;
}

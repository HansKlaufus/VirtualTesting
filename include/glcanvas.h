#ifndef CGLCANVAS_H
#define CGLCANVAS_H

#include <wx/glcanvas.h>

#include "actuator.h"
#include "connection.h"
#include "node.h"

class cGLCanvas : public wxGLCanvas
{
private:
    wxGLContext*	m_context;
    wxFrame*        m_parent;

    GLdouble        m_cameraX;
    GLdouble        m_cameraY;
    GLdouble        m_cameraZ;
    GLdouble        m_cameraAngleX;
    GLdouble        m_cameraAngleY;
    GLdouble        m_cameraAngleZ;

    GLdouble        m_upX;
    GLdouble        m_upY;
    GLdouble        m_upZ;

    double          m_radius;
    double          m_centreX;
    double          m_centreY;
    double          m_centreZ;

    bool            m_initialised;

    bool            m_showBackgroundRaster;
    bool            m_showAxisFrame;
    bool            m_showForceVectors;
    bool            m_showForceInputVectors;
    bool            m_showSpheres;

    void            drawBackgroundRaster(void);
    void            drawAxisFrame(void);
    void            drawNode(cNode* p_Node);
    void            drawConnection(cConnection* p_Connection);
    void            drawConnection2(cConnection* p_Connection);
    void            drawActuator(cActuator* p_Actuator);
    void            drawVector(cVector p_StartPoint, cVector p_EndPoint, GLfloat p_Material[]);
    void            drawForceInputVector(cNode* p_Node);
    void            drawForceVector(cNode* p_Node);

public:
    cGLCanvas(wxFrame* parent, int* args);
    virtual ~cGLCanvas();

	int getWidth();
	int getHeight();

	void setRadius(double r) {m_radius = r;};
	void setCentre(double x, double y, double z) {m_centreX = x; m_centreY = y; m_centreZ = z;};

	void redraw(void);

	void showBackgroundRaster(bool show) {m_showBackgroundRaster = show;};
	void showAxisFrame(bool show) {m_showAxisFrame = show;};
	void showForceVectors(bool show) {m_showForceVectors = show;};
	void showForceInputVectors(bool show) {m_showForceInputVectors = show;};
	void showSpheres(bool show) {m_showSpheres = show;};

	bool hasBackgroundRaster(void) {return m_showBackgroundRaster;};
	bool hasAxisFrame(void) {return m_showAxisFrame;};
	bool hasForceVectors(void) {return m_showForceVectors;};
	bool hasForceInputVectors(void) {return m_showForceInputVectors;};
	bool hasSpheres(void) {return m_showSpheres;};

    void initGL(void);
	void setProjectionMatrix(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void setModelMatrix(void);
	void setCameraPosition(double, double, double, double, double, double);
	void setCameraAngle(double, double, double);

	// events
	void onSizeEvent(wxSizeEvent& evt);
	void onPaintEvent(wxPaintEvent& evt);

	void onMouseWheelEvent(wxMouseEvent& event);
	void onLeftDown(wxMouseEvent& event);
	void onLeftReleased(wxMouseEvent& event);
	void mouseMoved(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif // CGLCANVAS_H

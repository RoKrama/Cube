#ifndef CUBE_H
#define CUBE_H

#include <QMainWindow>
#include <QPoint>
#include <cmath>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include <QPointF>
#include <QColor>
#include <QPushButton>
#include <QMouseEvent>
#include <cstdlib>
#include <QDebug>


using std::cos;
using std::sin;

class taskbar;
class Point
{
    float coor[3];

public:

    Point () {};
    Point(float x, float y, float z);


    Point& operator = (Point other); // assign operator yay?
    //Point(Point &other) {other = Point(coor[0], coor[1], coor[2]);}; // copy constructor yay

    void rotate(const float &a, const float &b, const float &g); // rotates coor array of points by a,b,g in rad
    const float* get_point() {return coor;};
    float* set_point() {return coor;};
    Point& operator += (Point add_on);
    Point& operator -= (Point add_off);
};
static Point camera(500,500,-100);
class Polygon
{
    Point corners[4];
    Point position;
    Point positioned_corners[4];

public:
    Polygon() {};
    Polygon(Point starting_position, Point init_corners[4]);

    void rotate_polygon(const float &a, const float &b, const float &g);
//    void project_polygon(){for(int i=0; i<4; i++) positioned_corners[i]-= camera;};

    Point* get_polygon() {return positioned_corners;};
    Point midpoint;
    QColor color;
    const float get_midpoint_z(int xyz);
    const float get_midpoint();
    Polygon operator = (Polygon other);

    const bool operator > (Polygon &other);
    const bool operator == (Polygon &other);
    const bool operator >= (Polygon &other);
};
class cube;
class Move_button : public QPushButton
{
    Q_OBJECT

    cube* mvb_obj;
    QPoint press_point;
    QPoint diff;
public:
    Move_button(cube* movable_obj);
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
};
class cube : public QMainWindow
{
    Q_OBJECT

    float a,b,g; //rotation angles in deg

    QColor color_array[6];
    Point cube_position;
    Point cube_points[8];
    Point cube_faces_points[6][4];
    Polygon faces[6];
    Polygon projected_faces[6];

    QTimer timer; // for animation
    QPixmap* pixmap; // frame graphic data buffer
    QPainter painter; // for frame painting

    Move_button move_button;
    QPushButton close_button;
public:
    QPointF qpoint[4];
    void add_to_stack();
    cube();
    ~cube();
public slots:
    void testfn();
    void paintEvent(QPaintEvent*) override; // display-drawing definition
};


#endif // CUBE_H

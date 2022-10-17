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

#include <QDebug>


using std::cos;
using std::sin;

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
};

class Polygon
{
    friend class cube;

    Point corners[4];
    Point position;
    Point positioned_corners[4];
    void set_pcoor(float x, float y, float z);

public:
    Polygon() {};
    Polygon(Point starting_position, Point init_corners[4]);

    Polygon operator = (Polygon other);
    void rotate_polygon(const float &a, const float &b, const float &g);
    const Point* get_polygon() {return positioned_corners;};
    const float get_midpoint_z() {return /*Point((corners[0].get_point()[0]+corners[2].get_point()[0])/2,
                                                (corners[0].get_point()[1]+corners[2].get_point()[1])/2,*/
                                                ((corners[0].get_point()[2]+corners[2].get_point()[2])/2);};
};
class polygon;
class cube : public QMainWindow
{
    Q_OBJECT

    QColor color_array[6];
    float a,b,g; //rotation angles in deg
    Point cube_position;
    Point cube_points[8];
    Point cube_faces_points[6][4];
    Polygon faces[6];
    Polygon projected_faces[6];

   // void apply_projection(Polygon to_project);

    QTimer timer; // for animation
    QPixmap* pixmap; // frame graphic data buffer
    QPainter painter; // for frame painting


public:
    QPointF qpoint[4];

    cube();
    ~cube();
public slots:
    void testfn();
    void paintEvent(QPaintEvent*) override;; // display-drawing definition

};

#endif // CUBE_H

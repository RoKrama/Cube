#include "cube.h"
#include "insertion_sort.h"

cube::cube():
    cube_position(300,300,100),

    cube_points{Point(-150, -150, -150),
                Point( 150, -150, -150),
                Point( 150, -150,  150),
                Point(-150, -150,  150),
                Point(-150,  150, -150),
                Point(-150,  150,  150),
                Point( 150,  150,  150),
                Point( 150,  150, -150)},

    color_array{QColor::fromRgb(23,25,12),
                QColor::fromRgb(32,23,51),
                QColor::fromRgb(43,43,19),
                QColor::fromRgb(67,31,14),
                QColor::fromRgb(12,34,43),
                QColor::fromRgb(53,90,94)},

    a(0.005),b(0.004),g(0.003),

    timer(this),
    pixmap(new QPixmap(800,800)),
    painter(),
    move_button(this),
    close_button(this)
{
    close_button.setGeometry(575,0,25,25);
    close_button.setStyleSheet("background-color: rgba(0,0,0,125)");
    connect(&close_button, &QPushButton::clicked, this, &cube::close);

    setPalette(QPalette(Qt::darkCyan));
    setGeometry(100,100, 600, 600);
    setWindowFlags(Qt::FramelessWindowHint);


        for (int j = 0; j < 4; j++)
        {
            cube_faces_points[0][j] = cube_points[j];
            cube_faces_points[1][j] = cube_points[j+4];
        }
        for (int i = 0; i < 2; i++)
        {
            cube_faces_points[5][i*3] = cube_points[i];
            cube_faces_points[5][i+1] = cube_points[4+i*3];
            cube_faces_points[2][i] = cube_points[3-i];
            cube_faces_points[2][i+2] = cube_points[6-i];
            cube_faces_points[3][i] = cube_points[i*3];
            cube_faces_points[3][i+2] = cube_points[5-i];
            cube_faces_points[4][i] = cube_points[i*6+1];
            cube_faces_points[4][i+2] = cube_points[6-i*4];
        }
        for (int i = 0; i < 6; i++) // setting faces as polygons -> array of points
            faces[i] = Polygon(cube_position, cube_faces_points[i]);

        connect(&timer, &QTimer::timeout, this, &cube::testfn);
        timer.start(20);
}
void cube::testfn()
{
    pixmap->fill(Qt::darkCyan);
    painter.begin(pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 3));
    float cmr_d= -1000;

    for (int i = 3; i < 6; i++)
    {
        for(int j =0; j<4; j++)
        {
            qpoint[j].setX((-1000*(faces[i].get_polygon()[j].get_point()[0]-300)/(-1000+faces[i].get_polygon()[j].get_point()[2]))+300);
            qpoint[j].setY((-1000*(faces[i].get_polygon()[j].get_point()[1]-300)/(-1000+faces[i].get_polygon()[j].get_point()[2]))+300);
            // have to transform self class Point into Qt::QPointF for Qt::drawPolygon()
        }
        painter.setBrush(faces[i].color);
        painter.drawPolygon(qpoint, 4);
    }
    painter.end();
    update();

    isrt::List<Polygon> sort(faces, 6);
    for (int i = 0; i < 6; i++)
        faces[i].rotate_polygon(a, b, g);
}
void cube::paintEvent(QPaintEvent *)
{
    painter.begin(this);
    painter.drawPixmap(0,0, *pixmap);
    painter.end();
}
cube::~cube()
{
    delete pixmap;
}
Point::Point(float x, float y, float z) :
    coor{x,y,z}
{}
//float Point::rotation_matrix[3][3] = {cos()};
void Point::rotate(const float &a, const float &b, const float &g)
{
    float temp_coor[3]; for(int i = 0; i<3; i++) temp_coor[i]=coor[i];
    //init of temporary so values aren't altered during calc

    coor[0] = (  cos(b)*cos(g)                         * temp_coor[0] ) +
              (( sin(a)*sin(b)*cos(g) - cos(a)*sin(g)) * temp_coor[1] ) +
              (( cos(a)*sin(b)*cos(g) + sin(a)*sin(g)) * temp_coor[2] ) ;

    coor[1] = (  cos(b)*sin(g)                         * temp_coor[0] ) +
              (( sin(a)*sin(b)*sin(g) + cos(a)*cos(g)) * temp_coor[1] ) +
              (( cos(a)*sin(b)*sin(g) - sin(a)*cos(g)) * temp_coor[2] ) ;

    coor[2] = ( -sin(b)       * temp_coor[0] ) +
              ( sin(a)*cos(b) * temp_coor[1] ) +
              ( cos(a)*cos(b) * temp_coor[2] ) ;

    // multiplication by multiplied rotational matrices into one general rotation matrix (oreder: yaw -> pitch -> roll)
    // using std::cos, std::sin
}
Point& Point::operator =(Point other)
{
    for(int i=0; i<3; i++) coor[i] = other.coor[i];
    return *this;
}
Point& Point::operator += (Point add_on)
{
    for(int i = 0; i<3; i++) coor[i] += add_on.coor[i];
    return *this;
}
Point &Point::operator -=(Point add_off)
{
    for(int i = 0; i<3; i++) coor[i] -= add_off.coor[i];
    return *this;
}
Polygon::Polygon(Point starting_position, Point init_corners[4]) :
    position(starting_position)
{
    for (int i = 0; i < 4; i++) corners[i] = init_corners[i];
    color = QColor::fromRgb(std::rand()%256,std::rand()%256,std::rand()%256);

}
void Polygon::rotate_polygon(const float &a, const float &b, const float &g)
{
    for(int i = 0; i < 4; i++) corners[i].rotate(a, b, g);
    for(int i = 0; i < 4; i++) ((positioned_corners[i] = corners[i]) += position);
    // rotating points storing them in positioned_corners[] and incrementing them by position
}
const float Polygon::get_midpoint_z(int xyz)
{
    return ((corners[0].get_point()[xyz]+corners[xyz].get_point()[xyz])/2);
    // returns midpoint of a rectangle
}

const float Polygon::get_midpoint()
{
    for(int i = 0; i < 4; i++)
        midpoint += corners[i];

    for(int j = 0; j < 3; j++)
        midpoint.set_point()[j] /= 4;
    return midpoint.get_point()[2];
}
Polygon Polygon::operator =(Polygon other)
{

    for(int i = 0; i<4; i++) corners[i] = other.corners[i];
    position = other.position;
    color = other.color;
    return *this;
}

const bool Polygon::operator > (Polygon &other)
{
    if (get_midpoint() > other.get_midpoint()) return true;
    return false;
}
const bool Polygon::operator == (Polygon &other)
{
    if (get_midpoint() == other.get_midpoint()) return true;
    return false;
}
const bool Polygon::operator >= (Polygon &other)
{
    if (get_midpoint() >= other.get_midpoint()) return true;
    return false;
}
Move_button::Move_button(cube* movable_obj) :
    mvb_obj(movable_obj),
    press_point(),
    diff()
{
    setParent(movable_obj);
    setGeometry(0,0,575,25);
    setStyleSheet("background-color: rgba(0,0,0,100)");
}
void Move_button::mousePressEvent(QMouseEvent* event)
{
    press_point = event->pos();
}
void Move_button::mouseMoveEvent(QMouseEvent* event)
{
    diff = event->pos() - press_point;
    mvb_obj->move(mvb_obj->pos() +diff);
}

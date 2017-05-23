#ifndef PIXEL_H
#define PIXEL_H
#include <QColor>

class pixel
{
public:
    pixel();
    ~pixel() {}

    QColor color;   //the color of a pixel
    bool done;      //a boolean to know if we already computed the pixel

    bool getBool() const { return done; }   //mutator and accessor for the boolean
    void setBool(const bool &value) { done = value; }

    QColor getColor() const { return color; } //mutator and accessor for the color
    void setColor(const QColor &value) { color = value; }

};

#endif // PIXEL_H

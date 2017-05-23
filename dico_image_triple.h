#ifndef DICO_IMAGE_TRIPLE_H
#define DICO_IMAGE_TRIPLE_H
#include "candy_image.h"
#include <QImage>
#include <QColor>
#include <QRgb>

using namespace std;


class Dico_image_triple
{
public:

    Dico_image_triple();

    ~Dico_image_triple() {  }

    std::vector<Candy_Image> Image_Array;   //vector of Candy_image

    QImage* find_nearest_image(QColor); //this function return the nearest image for a color

};

#endif // DICO_IMAGE_TRIPLE_H

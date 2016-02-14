#include "image.h"

image::image(QPixmap pixmap, int w, int h, QWidget *parent) :QLabel(parent),  pixmap(pixmap) {
    setScaledContents(true);
    setnPixmap(pixmap);
    setFixedSize(w,h);
    show();
}

void image::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    resizeImage();
}

void image::setnPixmap (const QPixmap &pixmap){
    setPixmap(pixmap);
    resizeImage();
}

void image::resizeImage() {
    QSize pixSize = pixmap.size();
    pixSize.scale(size(), Qt::KeepAspectRatio);
    setFixedSize(pixSize);
}

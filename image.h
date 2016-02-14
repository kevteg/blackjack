#ifndef IMAGE_H
#define IMAGE_H

/**
  Autores:
  Estefany Salas
  Kevin Hernández
  */


#include <QLabel>
/**
 * @brief Clase para manejar mejor las imágenes
 */
class image : public QLabel
{
    Q_OBJECT
public:
    explicit image(QPixmap pixmap, int w = 0, int h = 0, QWidget *parent = 0);

public slots:
    /**
     * @brief Cambiar imagen
     */
    void setnPixmap(const QPixmap&);

protected:
    /**
     * @brief Con este metodo la imagen debe cambiar de tamaño al cambiar la ventana de tamaño
     */
    void resizeEvent(QResizeEvent *);

private slots:
    void resizeImage();

private:
    QLabel *label;
    QPixmap pixmap;
};

#endif // IMAGE_H

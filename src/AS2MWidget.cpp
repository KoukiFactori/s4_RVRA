#include "AS2MWidget.h"
#include <QApplication>
#include <QMessageBox>
#include <QKeyEvent>
#include <QtDebug>
#include <QDir>

#include <iostream>
#include <sstream>

const int     AS2MWidget::nbImages  = 8;
const QSize   AS2MWidget::sizeMulti = QSize(1920,1080);


AS2MWidget::AS2MWidget(const QString & basename, int tv, int numView, QWidget *parent) :
    QGLWidget(parent),
    typeView(static_cast<TypeView>( tv>=MONO && tv<=MULTI ? tv : MONO )),
    numView( numView>=0 && numView<AS2MWidget::nbImages ? numView : 0 ),
    basename(basename),
    swapEyes(false)
{
    // protection sur vue à afficher
    if (this->typeView == ANAG_RB || this->typeView == ANAG_RC || this->typeView == STEREO)
        if (this->numView == AS2MWidget::nbImages-1) this->numView = AS2MWidget::nbImages-2;
    if (this->fillMask() && this->fillMono())
    {
        // on modifie le titre de la fenêtre
        this->setWindowTitle("AS2MWidget - Visionneuse");

        // on ajuste la taille de la fenêtre : à activer après codage du chargement des images
        this->resize(this->imgMono[0].size());

        // on calcule les images anaglyphes
        this->fillAnag();

        // on calcule l'image multistéréo (si demandé car modification de la taille nécessaire)
        if (this->typeView==MULTI) this->fillMult();
    }
    else
    {
        QMessageBox::critical(nullptr,"AS2MViewer","Erreur lors du chargement des images.");
    }
}

// permet au main de savoir si les images ont été chargées correctement
bool AS2MWidget::imagesLoaded() const
{
    return this->imgMask.size() == AS2MWidget::nbImages &&
            this->imgMono.size() == AS2MWidget::nbImages;

// renvoie vrai initialement sinon l'application se termine sans afficher la fenêtre
//   return true;
}

// méthode pour charger une série d'images dans un QVector
bool AS2MWidget::fillVector(const QString & base, QVector<QImage> & vector)
{
    bool res(true);
    QString filename;
    QImage tmp;
    QString dataPath = "R:/TP_AS2M/data/";
    if ( !QDir(dataPath).exists() ) dataPath = "./data/";

    for (int i=0; i<AS2MWidget::nbImages && res; i++)
    {
        filename = ( dataPath + base + "%1.png").arg(i);
        qDebug() << "Chargement de" << filename << "... ";
        tmp = QImage(filename);
        res = !tmp.isNull();
        if (res)
        {
            tmp = tmp.convertToFormat(QImage::Format_RGB32);
            if (this->typeView==MULTI) tmp = tmp.scaled( AS2MWidget::sizeMulti );
            vector.push_back( tmp );
            qDebug() << "OK";
        }
    }
    return res;
}

bool AS2MWidget::fillMask()
{
    return this->fillVector("alioscopy_40_full_HD/mask_", this->imgMask);
}

bool AS2MWidget::fillMono()
{
/// --- TODO : Chargement des images mono

    return this->fillVector(this->basename, this->imgMono);
}

// calcul des anaglyphes
void AS2MWidget::fillAnag()
{
/// --- TODO : Calculs des images anaglyphes

    // --- TODO : Try to rewrite and replace QRgb by QColor
    for (size_t i = 0; i < this->nbImages - 1; i++)
    {
        auto& leftImg = this->imgMono[i];
        std::cout << "Left image: " << i << std::endl;

        auto& rightImg = this->imgMono[i + 1];
        std::cout << "Right image: " << i + 1 << std::endl;

        //New image
        QImage resRB { leftImg };
        QImage resRC { leftImg };

        //naive iteration, assuming all images are the same size
        for (int y = 0; y < leftImg.height(); ++y) {
            for (int x = 0; x < leftImg.width(); ++x) {
                QRgb leftPixel = leftImg.pixel(x, y);
                QRgb rightPixel = rightImg.pixel(x, y);

                resRB.setPixel(x, y, qRgb(qRed(leftPixel), (qGreen(leftPixel) + qGreen(rightPixel)) / 2, qBlue(rightPixel)));
                resRC.setPixel(x, y, qRgb(qRed(leftPixel), qGreen(rightPixel), qBlue(rightPixel)));
            }
        }

        this->imgAnagRB.push_back(resRB);
        this->imgAnagRC.push_back(resRC);
    }
}

void AS2MWidget::saveAnag() const
{
    /// --- TODO : Sauvegarde des images anaglyphes
    for (int i = 0; i < this->nbImages - 1; ++i) {
        std::string filenameRB = "./result/AnaRB_" + std::to_string(i) + ".png";
        std::string filenameRC = "./result/AnaRC_" + std::to_string(i) + ".png";

        std::cout << "Sauvegarde couple " << i << std::endl;

        auto resRB = this->imgAnagRB[i].save(filenameRB.c_str(), "PNG", 50);
        if (!resRB) std::cout << "Echec de l'enregistrement" << std::endl;

        auto resRC = this->imgAnagRC[i].save(filenameRC.c_str(), "PNG", 50);
        if (!resRC) std::cout << "Echec de l'enregistrement" << std::endl;
    }
}

// calcul de l'image multiscopique
void AS2MWidget::fillMult()
{
/// --- TODO : Calcule l'image composite pour l'écran multiscopique

}

void AS2MWidget::saveMult() const
{
/// --- TODO : Sauvegarde de l'image composite

}

// méthode d'affichage d'une QImage sous OpenGL
void AS2MWidget::paintImage(const QImage & img) const
{
    QImage i(QGLWidget::convertToGLFormat(img));
    glDrawPixels(i.width(),i.height(),GL_RGBA,GL_UNSIGNED_BYTE,i.bits());
}

void AS2MWidget::paintMono() const
{
/// --- TODO : Dessin de l'image mono
    auto& img = this->imgMono[this->numView];
    this->paintImage(img);
}

void AS2MWidget::paintStereo() const
{
/// --- TODO : Dessin du couple de vues stéréoscopiques
    auto& leftImg = this->imgMono[this->numView + this->swapEyes];
    auto& rightImg = this->imgMono[this->numView - this->swapEyes + 1];

    glDrawBuffer(GL_BACK_RIGHT);
    this->paintImage(rightImg);

    glDrawBuffer(GL_BACK_LEFT);
    this->paintImage(leftImg);
}

void AS2MWidget::paintAnagRB() const
{
/// --- TODO : Dessin du couple de vues en anaglyphe rouge-bleu
    auto& img = this->imgAnagRB[this->numView];
    this->paintImage(img);
}

void AS2MWidget::paintAnagRC() const
{
/// --- TODO : Dessin du couple de vues en anaglyphe rouge-cyan
    auto& img = this->imgAnagRC[this->numView];
    this->paintImage(img);
}

void AS2MWidget::paintMulti() const
{
/// --- TODO : Dessin de l'image composite

}

void AS2MWidget::initializeGL()
{
}

void AS2MWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,w,0,h,-1,1);
    glMatrixMode(GL_MODELVIEW);
}

void AS2MWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
/// --- TODO - Dessin avec le type de rendu désiré

    switch (this->typeView) {
        case TypeView::MONO:
            this->paintMono();
            break;

        case TypeView::ANAG_RB:
            this->paintAnagRB();
            break;

        case TypeView::ANAG_RC:
            this->paintAnagRC();
            break;

        case TypeView::STEREO:
            this->paintStereo();
        break;

        case MULTI:
            break;
    }
}

void AS2MWidget::keyPressEvent(QKeyEvent *event)
{
    bool needRender = true;

    switch ( event->key() )
    {
        case Qt::Key_Escape:
            needRender = false;
            this->close();
            break;

        /// --- TODO : changement du mode de rendu
        case Qt::Key_1:
            this->typeView = TypeView::MONO;
            this->setWindowTitle("AS2MWidget - Render mode: mono");
            break;

        case Qt::Key_2:
            this->typeView = TypeView::ANAG_RB;
            this->setWindowTitle("AS2MWidget - Render mode: anaglyphe rouge/bleu");
            break;

        case Qt::Key_3:
            this->typeView = TypeView::ANAG_RC;
            this->setWindowTitle("AS2MWidget - Render mode: anaglyphe rouge/cyan");
            break;

        case Qt::Key_4:
            this->typeView = TypeView::STEREO;
            this->setWindowTitle("AS2MWidget - Render mode: stéréoscopie active");
            break;

        case Qt::Key_5:
            this->typeView = TypeView::MULTI;
            this->setWindowTitle("AS2MWidget - Render mode: multimode | Non implémenté");
            needRender = false;
            break;

        /// --- TODO : échange de l'affichage des images gauche-droite
        case Qt::Key_S: {
            this->swapEyes = !this->swapEyes;
            break;
        }

        /// --- TODO : sauvegarde des images anaglyphes et de l'image composite multiscopique
        case Qt::Key_L: {
            this->saveAnag();
            needRender = false;
            break;
        }

        /// --- TODO : Changement du couple de vues visualisé,
        ///             décalage vers la droite et décalage vers la gauche
        case Qt::Key_Right:
            this->numView = (this->numView == (this->nbImages - 2) ? 0 : (this->numView + 1));
            std::cout << "Couple: " << this->numView << std::endl;
            break;

        case Qt::Key_Left:
            this->numView = (this->numView == 0 ? (this->nbImages - 2) : (this->numView - 1));
            std::cout << "Couple: " << this->numView << std::endl;
            break;

        default:
            needRender = false;
            break;
    }

    if (needRender) {
        paintGL();
        updateGL();
    }
}

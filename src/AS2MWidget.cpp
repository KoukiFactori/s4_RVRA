#include "as2mwidget.h"
#include <QApplication>
#include <QMessageBox>
#include <QKeyEvent>
#include <QtDebug>
#include <QDir>

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
        this->setWindowTitle("AS2MWidget : séquence d'images " + this->basename);
        // on ajuste la taille de la fenêtre : à activer après codage du chargement des images
        // this->resize(this->imgMono[0].size());
        // on calcule les images anaglyphes
        this->fillAnag();
        // on calcule l'image multistéréo (si demandé car modification de la taille nécessaire)
        if (this->typeView==MULTI) this->fillMult();
    }
    else
    {
        QMessageBox::critical(NULL,"AS2MViewer","Erreur lors du chargement des images.");
    }
}

// permet au main de savoir si les images ont été chargées correctement
bool AS2MWidget::imagesLoaded() const
{
//    return this->imgMask.size() == AS2MWidget::nbImages &&
//            this->imgMono.size() == AS2MWidget::nbImages;

// renvoie vrai initialement sinon l'application se termine sans afficher la fenêtre
    return true;
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

    return true;
}

// calcul des anaglyphes
void AS2MWidget::fillAnag()
{
/// --- TODO : Calculs des images anaglyphes

}

void AS2MWidget::saveAnag() const
{
/// --- TODO : Sauvegarde des images anaglyphes

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

}

void AS2MWidget::paintStereo() const
{
/// --- TODO : Dessin du couple de vues stéréoscopiques

}

void AS2MWidget::paintAnagRB() const
{
/// --- TODO : Dessin du couple de vues en anaglyphe rouge-bleu

}

void AS2MWidget::paintAnagRC() const
{
/// --- TODO : Dessin du couple de vues en anaglyphe rouge-cyan

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

}

void AS2MWidget::keyPressEvent(QKeyEvent *event)
{
    switch ( event->key() )
    {
    case Qt::Key_Escape:
        this->close();
        break;

        /// --- TODO : changement du mode de rendu


        /// --- TODO : échange de l'affichage des images gauche-droite


        /// --- TODO : sauvegarde des images anaglyphes et de l'image composite multiscopique


        /// --- TODO : Changement du couple de vues visualisé,
        ///             décalage vers la droite et décalage vers la gauche
    }
}

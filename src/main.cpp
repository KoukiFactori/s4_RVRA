#include <QApplication>
#include <QMessageBox>
#include "as2mwidget.h"

int main(int argc, char *argv[])
{
    int ret(0);
    QApplication app(argc,argv);

    /// on peut utiliser app.arguments() au lieu de argc et argv
    if (app.arguments().size() < 3)
    {
        QString msg = "Nombre d'arguments insuffisant. \n";
        msg += "Usage : baseName tpRendu\n";
        msg += "   où\n";
        msg += " - baseName est le nom de base de la série à visualiser,\n";
        msg += " - tpRendu  est le type du rendu avec comme rendu possible \n";
        msg += "            1 MONO, 2 ANAG_RB, 3 ANAG_RC, 4 STEREO, 5 MULTI \n";
        QMessageBox::critical(NULL,"AS2MViewer", msg);
        // Exemple d'appel : ruinart_ 1
    }
    else
    {
        // Initialisation du contexte par défaut d'openGL
        QGLFormat format;
        format.setStereo(true);
        QGLFormat::setDefaultFormat(format);

        // création fenêtre d'affichage
        AS2MWidget widget(app.arguments()[1], app.arguments()[2].toInt(),
                          app.arguments().size()>3?app.arguments()[3].toInt():0);
        // vérification du chargement des images
        if (widget.imagesLoaded())
        {
            // affichage
            widget.show();
            // boucle de traitement
            ret = app.exec();
        }
    }
    
    return ret;
}

#ifndef AS2MWIDGET_H
#define AS2MWIDGET_H

#include <QGLWidget>

class AS2MWidget : public QGLWidget
{
private:
    enum TypeView { MONO=1,                        // Liste des types de rendu possible
                    ANAG_RB, ANAG_RC,
                    STEREO, MULTI };

    const static int   nbImages;                   // Nombre de vues prises en charge
    const static QSize sizeMulti;                  // Taille des filtres pour le rendu multiscopique
    /// --- TODO : Ajout d'un attribut constant statique définissant le nom des étudiants


    TypeView typeView;                             // Type de rendu sélectionné
    int      numView;                              // Numéro du couple de vues sélectionné 0 => 1er couple (0,1)
    QString  basename;                             // nom de base des fichiers
    bool     swapEyes;                             // Indicateur de l'échange de l'image de gauche avec l'image de droite

/// Stockage des images
    QVector<QImage> imgMask;                       // Filtres pour la multiscopie
    QVector<QImage> imgMono;                       // Images monoscopiques
    QVector<QImage> imgAnagRB;                     // Images anaglyphes pour les lunettes Rouge - Bleu
    QVector<QImage> imgAnagRC;                     // Images anaglyphes pour les lunettes Rouge - Cyan
    QImage          imgMult;                       // Image composite correspondant aux vues entrelacées suivant les filtres

/// Méthodes de remplissage des tableaux d'images
    bool fillVector(const QString &, QVector<QImage> &vector);
    bool fillMask();
    bool fillMono();                                /// TODO
    void fillAnag();                                /// TODO
    void fillMult();                                /// TODO

/// Méthodes de sauvegarde de certains types d'images
    void saveMult() const;                          /// TODO
    void saveAnag() const;                          /// TODO

/// Méthodes de dessins des images
    void paintImage (const QImage &) const;         // Méthode générique
    void paintMono  () const;                       /// TODO
    void paintAnagRB() const;                       /// TODO
    void paintAnagRC() const;                       /// TODO
    void paintMulti () const;                       /// TODO
    void paintStereo() const;                       /// TODO

protected:
    void keyPressEvent(QKeyEvent *event);           // Récupération et gestion des événements claviers

public:
    AS2MWidget(const QString &, int, int,           // Constructeur
               QWidget *parent=NULL);
    bool imagesLoaded() const;                      // Vérification du bon chargement des images
    void initializeGL();                            // Initialisation du contexte GL
    void resizeGL(int w, int h);                    // Gestion du redimensionnement de la fenêtre
    void paintGL();                                 // Fonction de dessin openGL
        
};

#endif // AS2MWIDGET_H

#include "imagegraphtreatment.h"


ImageGraphTreatment::ImageGraphTreatment()
{

}

typedef Graph<float, float, float> GraphType;
int largeur, hauteur, nombre_noeuds, nombre_aretes;

//remplit l'histograme de 0
void ImageGraphTreatment::histo::initialisation()
{
    for(int i=0 ; i<256 ; i++)
    {
        rouge[i]=0;
        vert[i]=0;
        bleu[i]=0;
    }
}

//normalise l'histogramme entre 0 et 1
void ImageGraphTreatment::histo::normalisation()
{
    int maxb=1;
    int maxr=1;
    int maxv=1;
    for(int i=0 ; i< 256 ; i++)
    {
        if(bleu[i]>maxb){
            maxb = bleu[i];
        }
        if(vert[i]>maxv){
            maxv = vert[i];
        }
        if(rouge[i]>maxr){
            maxr = rouge[i];
        }
    }
    for(int i=0 ; i<256 ; i++)
    {
        bleu[i] = bleu[i]/maxb;
        vert[i] = vert[i]/maxv;
        rouge[i] = rouge[i]/maxr;
    }
}

//affiche l'histogramme
void ImageGraphTreatment::histo::affichage()
{
    cout << "------histogramme rouge------" << endl;
    for(int i = 0 ; i < 256 ; i++)
    {
        cout << "intensiter " << i << " = " << rouge[i] << endl;
    }
    cout << "------histogramme vert------" << endl;
    for(int i = 0 ; i < 256 ; i++)
    {
        cout << "intensiter " << i << " = " << vert[i] << endl;
    }
    cout << "------histogramme bleu------" << endl;
    for(int i = 0 ; i < 256 ; i++)
    {
        cout << "intensiter " << i << " = " << bleu[i] << endl;
    }
}

void ImageGraphTreatment::creationHisto(QImage img, QImage calc, histo* background, histo* foreground)
{
    for(int i=0 ; i < img.height() ; i++)
    {
        for(int j=0 ; j < img.width() ; j++)
        {
            QColor couleurCalc = calc.pixel(j,i);
            QColor couleurImg = img.pixel(j,i);

            int pixelBleu = couleurImg.blue();
            int pixelRouge = couleurImg.red();
            int pixelVert = couleurImg.green();

            if(couleurCalc.blue() == 255 && couleurCalc.red() == 0 && couleurCalc.green() == 0)
            {
                background->rouge[pixelRouge] = background->rouge[pixelRouge]+1;
                background->vert[pixelVert] = background->vert[pixelVert]+1;
                background->bleu[pixelBleu] = background->bleu[pixelBleu]+1;
            }
            if(couleurCalc.red() == 255 && couleurCalc.green() == 0 && couleurCalc.blue() == 0)
            {
                foreground->rouge[pixelRouge] = foreground->rouge[pixelRouge]+1;
                foreground->vert[pixelVert] = foreground->vert[pixelVert]+1;
                foreground->bleu[pixelBleu] = foreground->bleu[pixelBleu]+1;
            }
        }
    }
}

qreal ImageGraphTreatment::differenceCouleur(QColor p1, QColor p2, qreal size)
{
    int P1rouge = p1.red();
    int P1vert = p1.green();
    int P1bleu = p1.blue();
    int P2rouge = p2.red();
    int P2vert = p2.green();
    int P2bleu = p2.blue();
    qreal ReducteurSigma = 1.0;
    qreal difference = 0;
    difference += abs(P1rouge - P2rouge);
    difference += abs(P1bleu - P2bleu);
    difference += abs(P1vert - P2vert);
    qreal poids = qExp(-((difference*difference)/((2.0*(size*size))/ReducteurSigma)));
    return poids;
}

QImage ImageGraphTreatment::colorier(GraphType* g, QImage img)
{
    int hauteur = img.height();
    int largeur = img.width();
    int noeud = 0;
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            if(g->what_segment(noeud) == GraphType::SOURCE)
            {
                QColor couleur = img.pixel(j,i);
                couleur.setRed(255);
                img.setPixelColor(j,i,couleur);
            }
            else
            {
                QColor couleur = img.pixel(j,i);
                couleur.setBlue(255);
                img.setPixelColor(j,i,couleur);
            }
            noeud++;
        }
    }
    return img;
}

/*
 * Cette fonction permet le traitement d'une image seul ou d'une image apartenant a une video
 * elle prend en parametre une image i2 qui est l'image traiter,
 * l'image précédentes i1 et l'image suivante i3 pour une video
 * i1 et i3 peuvent être mise a null pour permettre de traiter une image seul
 * cette fonction prend un graphe initialiser avec ces noeud créer en parametre
 * ainsi que l'histogramme du premier plan et second plan remplis et normaliser
 */
void ImageGraphTreatment::traitementImage(QImage i1, QImage i2,QImage i3, GraphType* g, histo* background, histo* foreground, QImage calc)
{
    //récupère la largeur et la longeur de l'image traiter
    int largeur = i2.width();
    int hauteur = i2.height();

    //calcule le nombre de noeud que comporte l'image
    int taille = largeur*hauteur;

    /*
     * initialise le compteur de noeud a 0 si nous avont pas d'image précédente
     * et a taille si nous en avons une pour récupérer les noeud de l'image précédente
     */
    int noeud;
    if(i1.isNull()){noeud = 0;}
    else{noeud=taille;}

    //calcul la discontinuité de l'image
    qreal size = i2.dotsPerMeterY()/39.3701;

    //instensiation variable utile pour le calcul du poids des arrêtes
    int rouge;
    int vert;
    int bleu;
    double HistoCouleurPuits;
    double HistoCouleurSource;
    double poidsPuits;
    double poidsSource;
    double poids;
    bool PresentCalc;

    //parcours des pixels d'une image
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            PresentCalc = false;
            //récupère la couleur d'un pixel à la position i,j
            QColor CPixel = i2.pixel(j,i);

            //récupère les composantes rvb de la couleur du pixel
            rouge = CPixel.red();
            vert = CPixel.green();
            bleu = CPixel.blue();
            if(!calc.isNull())
            {
                QColor couleurCalc = calc.pixel(j,i);
                if(couleurCalc.blue() == 255 && couleurCalc.red() == 0 && couleurCalc.green() == 0)
                {
                    g->add_tweights(noeud,0,100000);
                    PresentCalc = true;
                }
                if(couleurCalc.red() == 255 && couleurCalc.green() == 0 && couleurCalc.blue() == 0)
                {
                    g->add_tweights(noeud,100000,0);
                    PresentCalc = true;
                }
            }
            if(!PresentCalc)
            {
                /*CALCUL DU POIDS DE L'ARRETE ENTRE LE NOEUD ET LE PUITS*/
                //multiplication des valeurs des 3 histogrammes rvb
                HistoCouleurPuits = background->rouge[rouge] * background->vert[vert] * background->bleu[bleu];
                //vérification que valeur de l'histogramme soit différent de 0
                if(HistoCouleurPuits == 0){HistoCouleurPuits = 1;}
                //aplication d'un logarithme pour calculer le poids formule: -ln(Hf)
                poidsPuits = -log(HistoCouleurPuits);

                /*CALCUL DU POIDS DE L'ARRETE ENTRE LE NOEUD ET LA SOURCE*/
                //multiplication des valeurs des 3 histogrammes rvb
                HistoCouleurSource = foreground->rouge[rouge] * foreground->vert[vert] * foreground->bleu[bleu];
                //vérification que valeur de l'histogramme soit différent de 0
                if(HistoCouleurSource == 0){HistoCouleurSource = 1;}
                //aplication d'un logarithme pour calculer le poids formule: -ln(Hb)
                poidsSource = -log(HistoCouleurSource);

                //création des arrêtes entre le noeud et la source et entre le noeud et le puids avec leurs poids
                g->add_tweights(noeud,poidsSource,poidsPuits);
            }

            //ajout d'arretes avec l'image précédente
            if(!i1.isNull())
            {
                QColor CPixeli1 = i1.pixel(j,i);
                rouge = CPixeli1.red();
                vert = CPixeli1.green();
                bleu = CPixeli1.blue();
                /*CALCUL DU POIDS DE L'ARRETE ENTRE LE NOEUD ET LE PUITS*/
                //multiplication des valeurs des 3 histogrammes rvb
                HistoCouleurPuits = background->rouge[rouge] * background->vert[vert] * background->bleu[bleu];
                //vérification que valeur de l'histogramme soit différent de 0
                if(HistoCouleurPuits == 0){HistoCouleurPuits = 1;}
                //aplication d'un logarithme pour calculer le poids formule: -ln(Hf)
                poidsPuits = -log(HistoCouleurPuits);

                /*CALCUL DU POIDS DE L'ARRETE ENTRE LE NOEUD ET LA SOURCE*/
                //multiplication des valeurs des 3 histogrammes rvb
                HistoCouleurSource = foreground->rouge[rouge] * foreground->vert[vert] * foreground->bleu[bleu];
                //vérification que valeur de l'histogramme soit différent de 0
                if(HistoCouleurSource == 0){HistoCouleurSource = 1;}
                //aplication d'un logarithme pour calculer le poids formule: -ln(Hb)
                poidsSource = -log(HistoCouleurSource);

                //création des arrêtes entre le noeud et la source et entre le noeud et le puids avec leurs poids
                g->add_tweights(noeud-taille,poidsSource,poidsPuits);
                //récupère le pixel à la même position que celui traiter mais sur l'image précédente
                //calcul du poids
                poids = differenceCouleur(CPixel,CPixeli1,size);
                //ajout de l'arrête entre les deux noeuds avec le poids
                g->add_edge(noeud,noeud-taille,poids,poids);
            }

            //ajout d'arretes avec image suivante
            if(!i3.isNull())
            {
                //récupère le pixel à la même position que celui traiter mais sur l'image suivante
                QColor CPixeli3 = i3.pixel(j,i);
                rouge = CPixeli3.red();
                vert = CPixeli3.green();
                bleu = CPixeli3.blue();
                /*CALCUL DU POIDS DE L'ARRETE ENTRE LE NOEUD ET LE PUITS*/
                //multiplication des valeurs des 3 histogrammes rvb
                HistoCouleurPuits = background->rouge[rouge] * background->vert[vert] * background->bleu[bleu];
                //vérification que valeur de l'histogramme soit différent de 0
                if(HistoCouleurPuits == 0){HistoCouleurPuits = 1;}
                //aplication d'un logarithme pour calculer le poids formule: -ln(Hf)
                poidsPuits = -log(HistoCouleurPuits);

                /*CALCUL DU POIDS DE L'ARRETE ENTRE LE NOEUD ET LA SOURCE*/
                //multiplication des valeurs des 3 histogrammes rvb
                HistoCouleurSource = foreground->rouge[rouge] * foreground->vert[vert] * foreground->bleu[bleu];
                //vérification que valeur de l'histogramme soit différent de 0
                if(HistoCouleurSource == 0){HistoCouleurSource = 1;}
                //aplication d'un logarithme pour calculer le poids formule: -ln(Hb)
                poidsSource = -log(HistoCouleurSource);

                //création des arrêtes entre le noeud et la source et entre le noeud et le puids avec leurs poids
                g->add_tweights(noeud+taille,poidsSource,poidsPuits);
                //calcul du poids
                poids = differenceCouleur(CPixel,CPixeli3,size);
                //ajout de l'arrête entre les deux noeuds avec le poids
                g->add_edge(noeud,noeud+taille,poids,poids);
            }

            //traitement du noeud en bas a gauche
            //rien a faire sur ce noeud donc on passe au suivant
            if(i==hauteur-1 && j==0)
            {
              noeud++;
              continue;
            }

            //traitement premiere colonne et derniere ligne de l'image
            else if(i==hauteur-1 || j==0)
            {
              if(i==hauteur-1)
              {
                  QColor CPixel2 = i2.pixel(j-1,i);
                  poids = differenceCouleur(CPixel,CPixel2,size);
                  g->add_edge(noeud,noeud-1,poids,poids);
              }
              if(j==0)
              {
                  QColor CPixel2 = i2.pixel(j,i+1);
                  poids = differenceCouleur(CPixel,CPixel2,size);
                  g->add_edge(noeud,noeud+largeur,poids,poids);
              }
            }
            //cas général
            else
            {
              QColor CPixel2 = i2.pixel(j-1,i);
              poids = differenceCouleur(CPixel,CPixel2,size);
              g->add_edge(noeud,noeud-1,poids,poids);

              CPixel2 = i2.pixel(j,i+1);
              poids = differenceCouleur(CPixel,CPixel2,size);
              g->add_edge(noeud,noeud+largeur,poids,poids);
            }
            noeud++;
        }
    }
}

//prépare le traitement d'image en initialisant les histogrammes ainsi que le graphe et les variables nésséssaire
QImage ImageGraphTreatment::InitTraitementImage(QImage img, QImage calc)
{
   //déclaration variable
   GraphType* g;
   QImage im;
   QImage ir;

   //initialisation variable
   int largeur = img.width();
   int hauteur = img.height();
   int nombre_noeuds = largeur * hauteur;
   int nombre_aretes = nombre_noeuds * 4 + 1;
   int maxflow;

   //initialisation remplissage et noramlisation des deux histogramme
   histo background;
   background.initialisation();
   histo foreground;
   foreground.initialisation();
   creationHisto(img, calc, &background, &foreground);
   background.normalisation();
   foreground.normalisation();

   //instentiation et creation des noeud pour le graphe
   g = new GraphType(nombre_noeuds, nombre_aretes);
   for (int i = 0; i < nombre_noeuds; ++i) {
       g->add_node();
   }

   //appel de la fonction pour créer et mettre les poids sur les arrêtes
   traitementImage(im, img, im, g, &background, &foreground, calc);

   //coloration de l'image traiter
   maxflow = g->maxflow();
   cout << "image traiter" << endl;
   cout << "flowmax = " << maxflow << endl;
   ir = colorier(g,img);

   //reste du graphe pour traiter la prochaine image
   g->reset();
   return ir;
}

//permet de traiter une video
QList<QImage> ImageGraphTreatment::traitementVideo(QList<QImage> video, QImage calc)
{
    //déclation variable
    GraphType* g;
    QList<QImage> res;
    QImage im;
    QImage ir;

    //calcule du nombre d'arretes et de noeud pour initialiser le graphe
    int largeur = video.at(0).width();
    int hauteur = video.at(0).height();
    int nombre_noeuds = largeur * hauteur * 3;
    int nombre_aretes = nombre_noeuds * 6 + 1;
    int maxflow;

    //génération d'histogramme normaliser pour le premier et le second plan
    histo background;
    background.initialisation();
    histo foreground;
    foreground.initialisation();
    creationHisto(video.at(0), calc, &background, &foreground);
    background.normalisation();
    foreground.normalisation();

    //initialisation graphe
    g = new GraphType(nombre_noeuds*3, nombre_aretes*3);

    //parcours de toutes les images de la video
    for(int i=0 ; i<video.length(); i++)
    {
        //creation noeud
        for (int i = 0; i < nombre_noeuds; ++i) {
            g->add_node();
        }
        //cas ou l'image traiter est la première de la video
        if(i==0)
        {
            traitementImage(im, video.at(i),video.at(i+1), g, &background, &foreground, calc);
        }
        //cas ou l'image traiter est la dernière de la video
        else if(i==video.length()-1)
        {
            traitementImage(video.at(i-1) , video.at(i), im, g, &background, &foreground, im);
        }
        //cas générale traitement de l'image a partir de la précedente et de la suivante
        else
        {
            traitementImage(video.at(i-1) , video.at(i),video.at(i+1), g, &background, &foreground, im);
        }

        //coloration de l'image traiter
        maxflow = g->maxflow();
        cout << "image " << i << " traiter" << endl;
        cout << "flowmax = " << maxflow << endl;
        ir = colorier(g,video.at(i));
        res.insert(i,ir);

        //reste du graphe pour traiter la prochaine image
        g->reset();
    }
    //retourne la liste d'image colorier composant la video traiter
    return res;
}

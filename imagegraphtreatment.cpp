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
    qreal difference = 0;
    difference += abs(P1rouge - P2rouge);
    difference += abs(P1bleu - P2bleu);
    difference += abs(P1vert - P2vert);
    qreal poids = qExp(-((difference*difference)/(2.0*(size*size))));
    return poids;
}

GraphType* ImageGraphTreatment::initGraphe(QImage img, QImage calc){

    //Déclaration variable de retour
    GraphType* g;

    //initilisation variable
    int largeur = img.width();
    int hauteur = img.height();
    nombre_noeuds = largeur * hauteur;
    nombre_aretes = (largeur - 1)*2 + (hauteur - 1) * 2;
    g = new GraphType(nombre_noeuds, nombre_aretes);
    qreal size = img.dotsPerMeterY()/39.3701;


    //creation histogramme source et puits à partir d'un calc
    histo background;
    background.initialisation();
    histo foreground;
    foreground.initialisation();
    creationHisto(img, calc, &background, &foreground);
    background.normalisation();
    foreground.normalisation();
    //background.affichage();

    //Ajout de chaque noeuds
    for (int i = 0; i < nombre_noeuds; ++i) {
        g->add_node();
    }


    int noeud = 0;

    //Ajout des arêtes et des poids
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            QColor CPixel = img.pixel(j,i);

            //ajout des poids entre le noeud et la source, ainsi qu'entre le noeud et le puis
            int rouge = CPixel.red();
            int vert = CPixel.green();
            int bleu = CPixel.blue();

            double HistoCouleurPuits = background.rouge[rouge] * background.vert[vert] * background.bleu[bleu];
            if(HistoCouleurPuits == 0){HistoCouleurPuits = 1;}
            double poidsPuits = -log(HistoCouleurPuits);

            double HistoCouleurSource = foreground.rouge[rouge] * foreground.vert[vert] * foreground.bleu[bleu];
            if(HistoCouleurSource == 0){HistoCouleurSource = 1;}
            double poidsSource = -log(HistoCouleurSource);
            g->add_tweights(noeud,poidsSource,poidsPuits);

            //traitement noeud en bas a droite
            if(noeud==(hauteur-1)*largeur)
            {
              noeud++;
              continue;
            }
            //traitement premiere colonne et derniere ligne de l'image
            else if(i==hauteur-1 || j==0)
            {
              if(i==hauteur-1)
              {
                  QColor CPixel2 = img.pixel(j-1,i);
                  double poids = differenceCouleur(CPixel,CPixel2,size);
                  g->add_edge(noeud,noeud-1,poids,poids);
              }
              if(j==0)
              {
                  QColor CPixel2 = img.pixel(j,i+1);
                  double poids = differenceCouleur(CPixel,CPixel2,size);
                  g->add_edge(noeud,noeud+largeur,poids,poids);
              }
            }
            //cas général
            else
            {
              QColor CPixel2 = img.pixel(j-1,i);
              double poids = differenceCouleur(CPixel,CPixel2,size);
              g->add_edge(noeud,noeud-1,poids,poids);

              CPixel2 = img.pixel(j,i+1);
              poids = differenceCouleur(CPixel,CPixel2,size);
              g->add_edge(noeud,noeud+largeur,poids,poids);
            }
            noeud++;
        }
    }
    int flow = g->maxflow();
    cout << "flow = " << flow << endl;
    return g;
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

void ImageGraphTreatment::traitementImage(QImage i1, QImage i2,QImage i3, GraphType* g, histo* background, histo* foreground)
{
    int largeur = i2.width();
    int hauteur = i2.height();
    int taille = largeur*hauteur;
    int noeud = taille;
    qreal size = i2.dotsPerMeterY()/39.3701;

    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            QColor CPixel = i2.pixel(j,i);
            //ajout des poids entre le noeud et la source, ainsi qu'entre le noeud et le puis
            int rouge = CPixel.red();
            int vert = CPixel.green();
            int bleu = CPixel.blue();

            double HistoCouleurPuits = background->rouge[rouge] * background->vert[vert] * background->bleu[bleu];
            if(HistoCouleurPuits == 0){HistoCouleurPuits = 1;}
            double poidsPuits = -log(HistoCouleurPuits);

            double HistoCouleurSource = foreground->rouge[rouge] * foreground->vert[vert] * foreground->bleu[bleu];
            if(HistoCouleurSource == 0){HistoCouleurSource = 1;}
            double poidsSource = -log(HistoCouleurSource);
            g->add_tweights(noeud,poidsSource,poidsPuits);

            //ajout d'arretes avec l'image précédente
            if(!i1.isNull())
            {

                QColor CPixeli1 = i1.pixel(j,i);
                double poids = differenceCouleur(CPixel,CPixeli1,size);
                g->add_edge(noeud,noeud-taille,poids,poids);
            }

            //ajout d'arretes avec image suivante
            if(!i3.isNull())
            {
                QColor CPixeli3 = i3.pixel(j,i);
                double poids = differenceCouleur(CPixel,CPixeli3,size);
                g->add_edge(noeud,noeud+taille,poids,poids);
            }

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
                  double poids = differenceCouleur(CPixel,CPixel2,size);
                  g->add_edge(noeud,noeud-1,poids,poids);
              }
              if(j==0)
              {
                  QColor CPixel2 = i2.pixel(j,i+1);
                  double poids = differenceCouleur(CPixel,CPixel2,size);
                  g->add_edge(noeud,noeud+largeur,poids,poids);
              }
            }
            //cas général
            else
            {
              QColor CPixel2 = i2.pixel(j-1,i);
              double poids = differenceCouleur(CPixel,CPixel2,size);
              g->add_edge(noeud,noeud-1,poids,poids);

              CPixel2 = i2.pixel(j,i+1);
              poids = differenceCouleur(CPixel,CPixel2,size);
              g->add_edge(noeud,noeud+largeur,poids,poids);
            }
            noeud++;
        }
    }
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
    int nombre_noeuds = largeur * hauteur * 10;
    int nombre_aretes = nombre_noeuds * 4 + 1;

    //génération d'histogramme normaliser pour le premier et le second plan
    histo background;
    background.initialisation();
    histo foreground;
    foreground.initialisation();
    creationHisto(video.at(0), calc, &background, &foreground);
    background.normalisation();
    foreground.normalisation();

    cout << "longeur = " << video.length() << endl;
    //parcours de toutes les images de la video
    for(int i=0 ; i<video.length(); i++)
    {
        //initialisation du graphe
        g = new GraphType(nombre_noeuds*3, nombre_aretes*3);
        for (int i = 0; i < nombre_noeuds; ++i) {
            g->add_node();
        }
        //cas ou l'image traiter est la première de la video
        if(i==0)
        {
            cout << "je suis dans i==0 et i = " << i << endl;
            traitementImage(im, video.at(i),video.at(i+1), g, &background, &foreground);
        }
        //cas ou l'image traiter est la dernière de la video
        else if(i==video.length()-1)
        {
            cout << "je suis dans i==max et i = " << i << endl;
            traitementImage(video.at(i-1) , video.at(i), im, g, &background, &foreground);
        }
        //cas générale traitement de l'image a partir de la précedente et de la suivante
        else
        {
            cout << "je suis dans générale et i = " << i << endl;
            traitementImage(video.at(i-1) , video.at(i),video.at(i+1), g, &background, &foreground);
        }

        //coloration de l'image traiter
        int maxflow = g->maxflow();
        cout << "flowmax = " << maxflow << endl;
        ir = colorier(g,video.at(i));
        res.insert(i,ir);

        //reste du graphe pour traiter la prochaine image
        g->reset();
    }
    return res;
}

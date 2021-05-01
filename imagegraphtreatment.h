#ifndef IMAGEGRAPHTREATMENT_H
#define IMAGEGRAPHTREATMENT_H

#include <QCoreApplication>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <QPixmap>
#include <stdio.h>
#include "graph.h"
#include <vector>
#include <QtMath>
#include <qapplication.h>

using namespace std;
class ImageGraphTreatment
{
public:
    ImageGraphTreatment();
    typedef Graph<float, float, float> GraphType;
    struct histo
    {
        double rouge[256];
        double vert[256];
        double bleu[256];
        void initialisation();
        void normalisation();
        void affichage();
    };
    void creationHisto(QImage img, QImage calc, histo* background, histo* foreground);
    qreal differenceCouleur(QColor p1, QColor p2, qreal size);
    GraphType* initGraphe(QImage img, QImage calc);
    QImage colorier(GraphType* g, QImage img);
    void traitementImage(QImage i1, QImage i2,QImage i3, GraphType* g, histo* background, histo* foreground);
    QList<QImage> traitementVideo(QList<QImage> video, QImage calc);
};

#endif // IMAGEGRAPHTREATMENT_H

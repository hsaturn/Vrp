/* 
 * File:   Eclair.cpp
 * Author: hsaturn
 * 
 * Created on 20 novembre 2015, 02:19
 */

#include "Eclair.hpp"
Eclair::Eclair(Vector3D b,Vector3D e,double width) : begin(b), end(e), w(width)
{
    if(true)
    {
    begins.clear();
    ends.clear();
    poss.clear();
    possr.clear();
    beginsram.clear();
    rams.clear();
    Vector3D v = end-begin;
 
    int nb = myRand(0,(int)v.length()/10);
    float prec=0;
    for(int i = 1 ; i <= nb ; i++)
    {
        float m = myRand(prec,(float)i+0.5f);
        prec = m;
        Vector3D pos = begin+((v/((float)nb+1.f))*m);
        pos += Vector3D(myRand(0.0,(v.length()/((float)nb+1.f))),myRand(0.0,(v.length()/((float)nb+1.f))),myRand(0.0,(v.length()/((float)nb+1.f))));
        poss.push_back(pos);
    }
    int nbram = myRand(0,nb/5);
    for(int i = 0 ; i <= nbram ; i++)
    {
        vector<Vector3D> p;
        beginsram.push_back(myRand(0,nb));
        Vector3D beginr = poss[beginsram[i]];
        float div = myRand(8.f,2.f);
        Vector3D endr(beginr);
        endr.X += myRand(0.0,(v.X/div));
        endr.Y += myRand(0.0,(v.Y/div));
        endr.Z += myRand(0.0,(v.Z/div));
 
        Vector3D vr = endr-beginr;
        int nbr = myRand(0,nb/3);
        float precr=0;
        for(int j = 1 ; j <= nbr ; j++)
        {
            float m = myRand(precr,(float)j+0.5f);
            precr = m;
            Vector3D pos = beginr+((vr/((float)nbr+1.f))*m);
            pos += Vector3D(myRand(0.0,(vr.length()/((float)nbr+1.f))),myRand(0.0,(vr.length()/((float)nbr+1.f))),myRand(0.0,(vr.length()/((float)nbr+1.f))));
            p.push_back(pos);
        }
        rams.push_back(endr);
        possr.push_back(p);
    }
    }
}

Eclair::render(bool resetTimer)
{
    if(true)
    {
    begins.clear();
    ends.clear();
    poss.clear();
    possr.clear();
    beginsram.clear();
    rams.clear();
    Vector3D v = end-begin;
 
    int nb = myRand(0,(int)v.length()/10);
    float prec=0;
    for(int i = 1 ; i <= nb ; i++)
    {
        float m = myRand(prec,(float)i+0.5f);
        prec = m;
        Vector3D pos = begin+((v/((float)nb+1.f))*m);
        pos += Vector3D(myRand(0.0,(v.length()/((float)nb+1.f))),myRand(0.0,(v.length()/((float)nb+1.f))),myRand(0.0,(v.length()/((float)nb+1.f))));
        poss.push_back(pos);
    }
    int nbram = myRand(0,nb/5);
    for(int i = 0 ; i <= nbram ; i++)
    {
        vector<Vector3D> p;
        beginsram.push_back(myRand(0,nb));
        Vector3D beginr = poss[beginsram[i]];
        float div = myRand(8.f,2.f);
        Vector3D endr(beginr);
        endr.X += myRand(0.0,(v.X/div));
        endr.Y += myRand(0.0,(v.Y/div));
        endr.Z += myRand(0.0,(v.Z/div));
 
        Vector3D vr = endr-beginr;
        int nbr = myRand(0,nb/3);
        float precr=0;
        for(int j = 1 ; j <= nbr ; j++)
        {
            float m = myRand(precr,(float)j+0.5f);
            precr = m;
            Vector3D pos = beginr+((vr/((float)nbr+1.f))*m);
            pos += Vector3D(myRand(0.0,(vr.length()/((float)nbr+1.f))),myRand(0.0,(vr.length()/((float)nbr+1.f))),myRand(0.0,(vr.length()/((float)nbr+1.f))));
            p.push_back(pos);
        }
        rams.push_back(endr);
        possr.push_back(p);
    }
    }
}
 
void Eclair::blit3D(Vector3D cam)
{
    render(false);
    glEnable(GL_BLEND);
    Vector3D longueur = end-begin,pos = begin+(longueur/2),distance = cam-pos;
    if(distance.length() > longueur.length()*(3))
    {
        glDisable(GL_BLEND);
        return;
    }
    double visibilite = (longueur.length()/distance.length())*w;
    glPushMatrix();
    glLineWidth(visibilite);
    glBegin(GL_LINE_STRIP);
    glColor4d(1.0,1.0,1.0,1.0);
    glVertex3d(begin.X,begin.Y,begin.Z);
    for(int i = 0 ; i < poss.size() ; i++)
    {
        double c = 1.0-((i+1)*(1.0/(poss.size()+2)));
        glColor4d(c,c,1.0,c);
        glVertex3d(poss[i].X,poss[i].Y,poss[i].Z);
    }
    glColor4d(0.0,0.0,1.0,0.0);
    glVertex3d(end.X,end.Y,end.Z);
    glEnd();
 
    for(int i = 0 ; i < possr.size() ; i++)
    {
        glColor3d(1.0,(float)i/possr.size(),0.0);
        glBegin(GL_LINE_STRIP);
        glColor4d(((1.0/(poss.size()+2))+(beginsram[i]*(1.0/(poss.size()+2)))),((1.0/(poss.size()+2))+(beginsram[i]*(1.0/(poss.size()+2)))),1.0,1.0);
        glVertex3d(poss[beginsram[i]].X,poss[beginsram[i]].Y,poss[beginsram[i]].Z);
        for(int j = 0 ; j < possr[i].size() ; j++)
        {
            double c = 1.0-(((1.0/(poss.size()+2))+(beginsram[i]*(1.0/(poss.size()+2))))+((j+1)*((1.0-((1.0/(poss.size()+2))+(beginsram[i]*(1.0/(poss.size()+2)))))/(possr[i].size()+2))));
            glColor4d(c,c,1.0,c);
            glVertex3d(possr[i][j].X,possr[i][j].Y,possr[i][j].Z);
        }
        glColor4d(0.0,0.0,1.0,0.0);
        glVertex3d(rams[i].X,rams[i].Y,rams[i].Z);
        glEnd();
    }
    glColor4d(1.0,1.0,1.0,1.0);
    glPopMatrix();
    glDisable(GL_BLEND);
}


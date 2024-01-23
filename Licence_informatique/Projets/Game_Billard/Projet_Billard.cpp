

#include <math.h>
#include <Grapic.h>
#include <string.h>
#include <iostream>
#include <cmath>


using namespace grapic;
using namespace std;
const int DIMW=500;
const int MAX=50;

//////////////////////////Structures///////////////////////////////////////

struct vec2
{
	float x,y;
};

struct Color
{
    int r,g,b;
};

struct Boule
{
    vec2 Vitesse, Position, Force;
    float Masse;
    int Rayon = 5;
    Color c;
    bool affiche;
};

struct Trou
{
    vec2 Position;
    int Rayon = 5;
    Color c;
};

struct Pack
{
    Boule tab[MAX];
    int nb_Boules=7;

    //Souris
    bool mouseclic;
};

struct Trous
{
    Trou trous[MAX];
    int nb_trous = 4;
};


vec2 make_vec2 (float x, float y)
{
	vec2 c;
	c.x = x;
	c.y = y;
	return c;
}

vec2 make_vec2_exp (float r, float theta_deg)
{
	vec2 c;

	float theta_rad = (theta_deg*2*M_PI)/360;
	c.x = r*cos(theta_rad);
	c.y = r*sin(theta_rad);
	return c;
}

Color make_color (int r, int g, int b)
{
    Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

//////////////Opérateurs////////////////////////////////////////////

vec2 operator+ (vec2 a, vec2 b)
{
	return (make_vec2(a.x+b.x,a.y+b.y));
}

vec2 operator- (vec2 a, vec2 b)
{
	return (make_vec2(a.x-b.x,a.y-b.y));
}

vec2 operator* (float lambda, vec2 b)
{
	return (make_vec2(lambda*b.x,lambda*b.y));
}

vec2 operator* (vec2 a, vec2 b)
{
	return (make_vec2(a.x*b.x-a.y*b.y,a.x*b.y+a.y*b.x));
}

vec2 operator/ (vec2 v, float lambda)
{
    return make_vec2(v.x/lambda , v.y/lambda);
}

vec2 operator/ (vec2 a, vec2 b)
{
    vec2 c;
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    return c;
}

vec2 distance(vec2 a, vec2 b) //renvoie un vecteur
{
    vec2 d;
    d.x = b.x-a.x;
    d.y = b.y-a.y;

    d.x = sqrt(d.x*d.x);
    d.y = sqrt(d.y*d.y);

    return d;
}

float distancefloat(vec2 a, vec2 b) //renvoie un réel
{
    float d;
    float dx, dy;
    dx = b.x-a.x;
    dy = b.y-a.y;
    d = sqrt( dx*dx + dy*dy );


    return d;
}

vec2 Rotate (vec2 p, float cx, float cy, float theta_deg)
{
	vec2 t = make_vec2(cx,cy);
	vec2 r = make_vec2_exp(1,theta_deg);

	return ((p-t)*r)+t;
}

float calculangle (vec2 a, vec2 b) //tangente (test 3)
{
    return ( (a.x+a.y) / (b.x+b.y) );
}

float angleDegre (float x)
{
    return (x *180 / M_PI);
}




///////////////////Fonctions////////////////////////////////////




void init (Pack &P, Trous &T)
{
    //on définie la position des boules
    P.tab[0].Position = make_vec2(100, 250);
    P.tab[1].Position = P.tab[0].Position + make_vec2(200, 0);
    P.tab[2].Position = P.tab[1].Position + make_vec2(10, 10);
    P.tab[3].Position = P.tab[1].Position + make_vec2(10, -10);
    P.tab[4].Position = P.tab[2].Position + make_vec2(10, 10);
    P.tab[5].Position = P.tab[1].Position + make_vec2(20, 0);
    P.tab[6].Position = P.tab[3].Position + make_vec2(10, -10);

    //Pour chaque boule
    int i;
    for(i=0; i<P.nb_Boules; i++)
    {
        //On définie la couleur
        P.tab[i].c = make_color( 255, 0, 0);


        //On définie la vitesse
        P.tab[i].Vitesse = make_vec2(0,0);

        //On définie la Force
        P.tab[i].Force = make_vec2(0,0);

        //On définie la masse
        P.tab[i].Masse = 1.0;

        //affichage des boules
        P.tab[i].affiche = true;
    }

    //Couleur particulière de la boule 0 (boule blanche)
    P.tab[0].c = make_color(255,255,255);


    //Souris
    P.mouseclic = false;


    //position des trous
    T.trous[0].Position = make_vec2(60,160);
    T.trous[1].Position = make_vec2(440,160);
    T.trous[2].Position = make_vec2(60,340);
    T.trous[3].Position = make_vec2(440,340);


}

void draw (Pack &P, Trous &T)
{

    //Bords de la table du billard
    color(70, 30, 30);
    rectangleFill(40,140, 460,360);
    color(65,90,40);
    rectangleFill(50,150, 450,350);

    //Boules du billard
    int i;
    for(i=0; i<P.nb_Boules; i++)
    {
        if(P.tab[i].affiche == true)
        {
            color( P.tab[i].c.r, P.tab[i].c.g, P.tab[i].c.b );
            circleFill( P.tab[i].Position.x, P.tab[i].Position.y, P.tab[i].Rayon );
        }
    }

    //Affichage souris
    int x,y;
    mousePos(x,y);
    color(0,0,0);
    line(x,y , P.tab[0].Position.x, P.tab[0].Position.y );

    //Affichage trous
    int j;
    for(j=0; j<T.nb_trous; j++)
    {
        color(0,0,0);
        circleFill(T.trous[j].Position.x, T.trous[j].Position.y, T.trous[j].Rayon);
    }
}

void partAddForce (Boule &B, vec2 F) //pour test
{
    B.Force = B.Force + F;
}


void TapeBouleBlanche (Pack &p, float friction)
{
    float temps = 0.045;

    if ( isMousePressed(SDL_BUTTON_LEFT) ) //quand on clique sur la souris
         {
             if (p.mouseclic == false)
             {
                p.tab[0].Vitesse.x = 0;
                p.tab[0].Vitesse.y = 0;

                //Coordonnées souris
                int x,y;
                mousePos(x,y);

                //Changement de la vitesse
                vec2 V = make_vec2( (x - p.tab[0].Position.x), (y - p.tab[0].Position.y) );
                partAddForce(p.tab[0],V);

                p.tab[0].Vitesse = p.tab[0].Vitesse + temps*V/p.tab[0].Masse;
                p.tab[0].Vitesse = -friction*p.tab[0].Vitesse;
             }
             p.mouseclic = true;

         } else p.mouseclic = false;

        //Changement de la position
        p.tab[0].Position = p.tab[0].Position + temps*p.tab[0].Vitesse;
}


void CollisionBords (Pack &p, float friction)
{
    int i;
    for (i=0; i<p.nb_Boules; i++)
    {
        if (p.tab[i].Position.x<=55)
        {
            p.tab[i].Position.x = 55 + 55 - p.tab[i].Position.x;
            p.tab[i].Vitesse.x = -friction * p.tab[i].Vitesse.x;
        }
        if (p.tab[i].Position.x>=445)
        {
            p.tab[i].Position.x = 445 + 445 - p.tab[i].Position.x ;
            p.tab[i].Vitesse.x = -friction * p.tab[i].Vitesse.x;
        }
        if (p.tab[i].Position.y<=155)
        {
            p.tab[i].Position.y = 155 + 155 - p.tab[i].Position.y;
            p.tab[i].Vitesse.y = -friction * p.tab[i].Vitesse.y;
        }
        if (p.tab[i].Position.y>=345)
        {
            p.tab[i].Position.y = 345 + 345 - p.tab[i].Position.y ;
            p.tab[i].Vitesse.y = -friction * p.tab[i].Vitesse.y;
        }
    }
}


bool DetectionCollision (Boule b1, Boule b2)
{

    if ( distancefloat(b1.Position,b2.Position) < 10 ) //si la distance entre 2 boules est inférieur à la somme des rayons
    {

        return true;
    } else return false;


}


void CollisionsBoules (Pack &p)
{
    int i,j;


    for (i=0; i<p.nb_Boules; i++)
    {
        for(j=0; j<p.nb_Boules; j++)
        {

            if ( DetectionCollision(p.tab[i],p.tab[j]) == true && i != j ) //s'il y a collision entre 2 boules différentes
            {
                //Le plus simple
                /*
                vec2 stock = p.tab[i].Vitesse;
                p.tab[i].Vitesse = p.tab[j].Vitesse;
                p.tab[j].Vitesse = stock;
                */




                //test 4

                int angle = ( atan2(p.tab[j].Position.y-p.tab[i].Position.y , p.tab[j].Position.x-p.tab[i].Position.x ) );
                angle = angleDegre(angle);

                //cout<<angle<<endl;

                //rotation du vecteur vitesse
                vec2 c1 = Rotate(p.tab[i].Vitesse, p.tab[i].Position.x, p.tab[i].Position.y, angle);
                vec2 c2 = Rotate(p.tab[j].Vitesse, p.tab[j].Position.x, p.tab[j].Position.y, angle);

                //changement vitesses (selon définition choc élastique)
                p.tab[i].Vitesse.x = ( (p.tab[i].Masse-p.tab[j].Masse) / (p.tab[i].Masse+p.tab[j].Masse) )*c1.x + ( p.tab[j].Masse / (p.tab[i].Masse+p.tab[j].Masse) )  *2*c2.x ;
                p.tab[i].Vitesse.y = c1.y;

                p.tab[j].Vitesse.x = ( (p.tab[i].Masse-p.tab[j].Masse) / (p.tab[i].Masse+p.tab[j].Masse) )*c2.x + ( p.tab[j].Masse / (p.tab[i].Masse+p.tab[j].Masse) )  *2*c1.x ;
                p.tab[j].Vitesse.y = c2.y;

                //retour de rotation
                p.tab[i].Vitesse = Rotate(p.tab[i].Vitesse, p.tab[i].Position.x, p.tab[i].Position.y, -angle);
                p.tab[j].Vitesse = Rotate(p.tab[j].Vitesse, p.tab[j].Position.x, p.tab[j].Position.y, -angle);

                //friction
                p.tab[i].Vitesse = 0.1*p.tab[i].Vitesse;
                p.tab[j].Vitesse = 0.1*p.tab[j].Vitesse;




                //test 4.1
                /*
                //changement vitesse
                vec2 stock = p.tab[i].Vitesse;

                if (angle<=10 || angle>=170)
                {
                    p.tab[i].Vitesse = -1*p.tab[i].Vitesse;
                    p.tab[i].Position = p.tab[i].Position + p.tab[i].Vitesse;

                    p.tab[j].Vitesse = stock;
                    p.tab[j].Position = p.tab[j].Position + p.tab[j].Vitesse;
                }

                if ( (angle<= 30 && angle>10) || (angle<170 && angle>=150) )
                {
                    p.tab[i].Vitesse = Rotate(p.tab[i].Vitesse, p.tab[i].Position.x,p.tab[i].Position.y, -20 );
                    p.tab[i].Vitesse = -0.25 * p.tab[i].Vitesse;
                    p.tab[i].Position = p.tab[i].Position + p.tab[i].Vitesse;

                    p.tab[j].Vitesse = Rotate(p.tab[j].Vitesse, p.tab[j].Position.x,p.tab[j].Position.y, 20 );
                    p.tab[j].Vitesse = 0.75 * stock;
                    p.tab[j].Position = p.tab[j].Position + p.tab[j].Vitesse;
                }

                if ( (angle>30 && angle<=45) || (angle<150 && angle>=135) )
                {
                    p.tab[i].Vitesse = Rotate(p.tab[i].Vitesse, p.tab[i].Position.x,p.tab[i].Position.y, -40 );
                    p.tab[i].Vitesse = -0.5 * p.tab[i].Vitesse;
                    p.tab[i].Position = p.tab[i].Position + p.tab[i].Vitesse;

                    p.tab[j].Vitesse = Rotate(p.tab[j].Vitesse, p.tab[j].Position.x,p.tab[j].Position.y, 40 );
                    p.tab[j].Vitesse = 0.5 * stock;
                    p.tab[j].Position = p.tab[j].Position + p.tab[j].Vitesse;
                }

                if ( (angle>45 && angle<=60) || (angle<135 && angle>=120) )
                {
                    p.tab[i].Vitesse = Rotate(p.tab[i].Vitesse, p.tab[i].Position.x,p.tab[i].Position.y, -50 );
                    p.tab[i].Vitesse = -0.75 * p.tab[i].Vitesse;
                    p.tab[i].Position = p.tab[i].Position + p.tab[i].Vitesse;

                    p.tab[j].Vitesse = Rotate(p.tab[j].Vitesse, p.tab[j].Position.x,p.tab[j].Position.y, 50 );
                    p.tab[j].Vitesse = 0.25 * stock;
                    p.tab[j].Position = p.tab[j].Position + p.tab[j].Vitesse;
                }

                if( (angle>60 && angle<=85) || (angle<120 && angle>=95) )
                {
                    p.tab[i].Vitesse = Rotate(p.tab[i].Vitesse, p.tab[i].Position.x,p.tab[i].Position.y, -70 );
                    p.tab[i].Vitesse = -0.8 * p.tab[i].Vitesse;
                    p.tab[i].Position = p.tab[i].Position + p.tab[i].Vitesse;

                    p.tab[j].Vitesse = Rotate(p.tab[j].Vitesse, p.tab[j].Position.x,p.tab[j].Position.y, 70 );
                    p.tab[j].Vitesse = 0.2 * stock;
                    p.tab[j].Position = p.tab[j].Position + p.tab[j].Vitesse;
                }

                if (angle>85 && angle<95)
                {
                    p.tab[i].Vitesse = Rotate(p.tab[i].Vitesse, p.tab[i].Position.x,p.tab[i].Position.y, 90 );
                    p.tab[i].Vitesse = -0.9 * p.tab[i].Vitesse;
                    p.tab[i].Position = p.tab[i].Position + p.tab[i].Vitesse;

                    p.tab[j].Vitesse = Rotate(p.tab[j].Vitesse, p.tab[j].Position.x,p.tab[j].Position.y, 90 );
                    p.tab[j].Vitesse = 0.1 * stock;
                    p.tab[j].Position = p.tab[j].Position + p.tab[j].Vitesse;

                }
                */

                //test 3
                /*
                //création d'un plan cartésien avec 2 vecteurs
                vec2 plan_y = distance( p.tab[i].Position,p.tab[j].Position );

                vec2 plan_x = plan_y;
                plan_x = Rotate(plan_x, plan_x.x, plan_x.y, 90);

                cout<<plan_x.x<<plan_x.y<<endl;


                //Avant choc
                float angleB1 = calculangle(p.tab[i].Vitesse, plan_x);
                angleB1 = angleDegre(angleB1);

                float angleB2 = calculangle(p.tab[j].Vitesse, plan_x);
                angleB2 = angleDegre(angleB2);
                cout<<angleB1<<angleB2<<endl;

                //Après choc
                float angleB1p = (sin(angleB2)/cos(angleB1)) * (p.tab[j].Vitesse/p.tab[i].Vitesse) ;

               */

               //Test 3.1
               /*
                p.tab[i].Vitesse.x = sqrt( (sin(angleB2) * p.tab[j].Vitesse.x)*(sin(angleB2) * p.tab[j].Vitesse.x) + (cos(angleB1)*p.tab[i].Vitesse.x)*(cos(angleB1)*p.tab[i].Vitesse.x) );
                p.tab[i].Vitesse.y = sqrt( (sin(angleB2) * p.tab[j].Vitesse.y)*(sin(angleB2) * p.tab[j].Vitesse.y) + (cos(angleB1)*p.tab[i].Vitesse.y)*(cos(angleB1)*p.tab[i].Vitesse.y) );

                p.tab[j].Vitesse.x = sqrt( (sin(angleB1) * p.tab[i].Vitesse.x)*(sin(angleB1) * p.tab[i].Vitesse.x) + (cos(angleB2)*p.tab[j].Vitesse.x)*(cos(angleB2)*p.tab[j].Vitesse.x) );
                p.tab[j].Vitesse.y = sqrt( (sin(angleB1) * p.tab[i].Vitesse.y)*(sin(angleB1) * p.tab[i].Vitesse.y) + (cos(angleB2)*p.tab[j].Vitesse.y)*(cos(angleB2)*p.tab[j].Vitesse.y) );

                p.tab[i].Position = p.tab[i].Position + p.tab[i].Vitesse;
                p.tab[j].Position = p.tab[j].Position + p.tab[j].Vitesse;

                */

                //test 2
                /*
                //distance entre les 2 boules
                float dist = distancefloat(p.tab[i].Position,p.tab[j].Position);

                //différence de vitesse entre les 2 boules
                vec2 DifVit;
                DifVit.x = p.tab[i].Vitesse.x - p.tab[j].Vitesse.x;
                DifVit.y = p.tab[i].Vitesse.y - p.tab[j].Vitesse.y;

                //différence de position entre les 2 boules
                vec2 DifPos;
                DifPos.x = p.tab[j].Position.x - p.tab[i].Position.x;
                DifPos.y = p.tab[j].Position.y - p.tab[i].Position.y;

                if ( (DifVit.x*DifPos.x) + (DifVit.y*DifPos.y) >= 0 )
                {
                    float angle = atan2( (p.tab[j].Position.y + p.tab[i].Position.y), (p.tab[j].Position.x + p.tab[i].Position.x) );
                    float angleDegre = angle * 180 / M_PI ; //angle en radian --> angle en degré

                    //p.tab[i].Vitesse = Rotate(p.tab[i].Position,p.tab[i].Position.x,p.tab[i].Position.y,angleDegre);
                    //p.tab[j].Vitesse = Rotate(p.tab[j].Position,p.tab[j].Position.x,p.tab[j].Position.y,angleDegre);

                    vec2 u1 = Rotate(p.tab[i].Vitesse, p.tab[i].Position.x,p.tab[i].Position.y, angleDegre);
                    vec2 u2 = Rotate(p.tab[j].Vitesse, p.tab[j].Position.x,p.tab[j].Position.y, angleDegre);

                    vec2 v1 = make_vec2(u2.x, u1.y);
                    vec2 v2 = make_vec2(u1.x,u2.y);

                    vec2 vecfin1 = Rotate(v1, p.tab[i].Position.x, p.tab[i].Position.y, angleDegre);
                    vec2 vecfin2 = Rotate(v2, p.tab[j].Position.x, p.tab[j].Position.y, angleDegre);

                    p.tab[i].Vitesse = vecfin1;
                    p.tab[j].Vitesse = vecfin2;

                    p.tab[i].Position = p.tab[i].Position + p.tab[i].Vitesse;
                    p.tab[j].Position = p.tab[j].Position + p.tab[j].Vitesse;


                }
                */

                //Test 1
                /*
                float angle = atan2( (p.tab[j].Position.y + p.tab[i].Position.y), (p.tab[j].Position.x + p.tab[i].Position.x) );

                float angleDegre = angle * 180 / M_PI ; //angle en radian --> angle en degré

                p.tab[i].Vitesse = angleDegre * p.tab[i].Vitesse ;
                p.tab[j].Vitesse = angleDegre * p.tab[j].Vitesse ;

                //distance entre les 2 boules
                float dist = distancefloat( p.tab[i].Position,p.tab[j].Position );

                //calcul de la normal
                vec2 normal;
                normal.x = (p.tab[j].Position.x - p.tab[i].Position.x) / dist;
                normal.y = (p.tab[j].Position.y - p.tab[i].Position.y) / dist;

                //calcul de la tangente
                vec2 tangente;
                tangente.x = -normal.x;
                tangente.y = normal.y;

                float dptan1 = p.tab[i].Vitesse.x * tangente.x + p.tab[i].Vitesse.y * tangente.y;
                float dptan2 = p.tab[j].Vitesse.x * tangente.x + p.tab[j].Vitesse.y * tangente.y;

                //changement de direction
                p.tab[i].Vitesse.x = tangente.x * dptan1;
                p.tab[i].Vitesse.y = tangente.y * dptan1;
                p.tab[j].Vitesse.x = tangente.x * dptan2;
                p.tab[j].Vitesse.x = tangente.y * dptan2;

                //changement de position
                p.tab[i].Position = p.tab[i].Position + p.tab[i].Vitesse;
                p.tab[j].Position = p.tab[j].Position + p.tab[j].Vitesse;
                */
            }
        }
        //changement de position
        p.tab[i].Position = p.tab[i].Position + p.tab[i].Vitesse;
        p.tab[j].Position = p.tab[j].Position + p.tab[j].Vitesse;
    }
}


void TombeDansLeTrou (Pack &p, Trous &t)
{
    int i,j;

    for (i=0; i<p.nb_Boules; i++)
    {
        for (j=0; j<t.nb_trous; j++)
        {

            if (distancefloat(p.tab[i].Position,t.trous[j].Position) < 8) //les boules tombent dans un trou
            {
                p.tab[i].affiche=false; //enlève l'affichage de la boule

            }
        }
    }

}


void Endgame (Pack &p)
{
    int i;

    for(i=0; i<p.nb_Boules; i++)
    {

        if (p.tab[i].affiche==false) //si une boule tombe dans un trou
        {
            p.tab[i].Position=make_vec2(55,155); //place la boule dans un coin de la table
            p.tab[i].Vitesse=make_vec2(0,0); //on remet à zero la vitesse pour qu'elle ne se déplace pas
        }
    }

    if (p.tab[0].affiche==false) //si la boule blanche tombe dans un trou
    {
        print(230,100,"Perdu !"); //affiche perdu
    }

    for(i=1; i<p.nb_Boules; i++)
    {
        if (p.tab[0].affiche==true && p.tab[i].affiche==false) //si on fait tomber une des boules dans un trou
        {
            print(230,100,"Bien joue !");
        }
    }

}



int main (int, char**)
{

    bool stop = false;
	winInit("Billard",DIMW,DIMW);
	backgroundColor(150,90,90);
	float friction = 0.15;


	Pack p; //les boules
	Trous t; //les trous

	init(p,t);

	while (!stop)
    {
        winClear();

        draw(p,t);

        TapeBouleBlanche(p,friction);

        CollisionBords(p,friction);
        CollisionsBoules(p);

        TombeDansLeTrou(p,t);
        Endgame(p);

        stop = winDisplay();
    }

    pressSpace();


	winQuit();
	return 0;
}

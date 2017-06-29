#include <iostream.h>
#include <conio.h>
#include <stdlib.h>


#define VERSION 0.83
#define MAX 100
#define ALPH 26

//Sert pour l'automate en Etoile
struct Atome
{
   char fleche;
   int noeud1,noeud2;
};

//Sert pour l'automate D et C
struct Atome_Deterc
{
   int noeud1[MAX],noeud2[MAX];
   char fleche;
};

struct M
{
   int n[MAX];
};


struct NAtome
{
   int n1,n2,etat;
   char fleche;

};

char *X=new char[MAX];          //ON Y STOCK L'ensemble X
char *Alphabet=new char[ALPH];  //ON Y STOCK l'Alphabet
Atome *Etoile;                //ON Y STOCK L'automate en Etoile
NAtome *A;
Atome_Deterc *ADC=new Atome_Deterc[MAX];   // Automate D et C :ici new obliger
int *IN1=new int [MAX];     //Longueur d'etat1  de ADC
int *IN2=new int [MAX];     //Longueur d'etat2  de ADC
M *Marque=new M [MAX];  //Pour le Marquage
int *IMQ=new int [MAX];
int lmq=0;
int lgai=0;                    //Longueur de l'Alphabet
int lgxi=0;                    //Longueur de X
int tt=1;                      //Longueur de l'automate en Etoile
int etat=1;
int Mots=0;                    //Nombre de mots de X
int lgadc=0;                   //Longueur de l'ADC

int c=0;
int cnbe=0;

int **M1;
int **M2;    //Les 2 matrices
int *Pos;
int pos=0;


//Permet de Saisir l'alphabet
void Saisie_Alphabet()
{
   char temp;
   cout<<"Entrer votre alphabet A:  (ex:abc.)"<<endl;
   cin>>temp;
   while(temp!='.')
   {
      Alphabet[lgai++]=temp;
      cin>>temp;
   }
}

//Affiche l'alphabet
void Affiche_Al()
{
   cout<<"Alphabet :";
   for(int i=0;i<lgai;i++)
      cout<<Alphabet[i]<<" ";
   cout<<endl;
}

//Permet de saisir X
void Saisie_X()
{
   char temp;
   cout<<"Entrer votre ensemble de mots finis de X:"<<endl;
   cin>>temp;
   while(temp!='.')
   {
      if(temp==',')   Mots++;
      X[lgxi++]=temp;
      cin>>temp;
   }
   Etoile=new Atome[lgxi];   //Allocation memoire de l'automate Etoile
   cout<<"Allocation de l'automate en Etoile"<<endl;
}

//Affiche X
void Affiche_X()
{
   cout<<"X: ";
   for(int i=0;i<lgxi;i++)
      cout<<X[i];
   cout<<endl;
}



//Permet de Creer l'automate en Etoile
void Creer_Etoile()
{
   int i=0;
   int i3=1;
   cout<<"Creation de l'automate en Etoile"<<endl;
   do{
      if(X[i+1]!=',')
      {
	 Etoile[tt].noeud1=1;
	 Etoile[tt].noeud2=++etat;
	 Etoile[tt].fleche=X[i++];
      }
      else
      {
	 Etoile[tt].noeud1=1;
	 Etoile[tt].noeud2=1;
	 Etoile[tt].fleche=X[i];
	 i+=2;
      }
      tt++;
      i3=1;
      while(i3!=0)
      {
	 if(X[i+1]==',')
	 {
	    Etoile[tt].noeud1=etat;
	    Etoile[tt].noeud2=1;
	    Etoile[tt].fleche=X[i++];
	    tt++;
	    i++;
	    i3=0;
	 }
	 else
	 {
	    Etoile[tt].noeud1=etat;
	    Etoile[tt].noeud2=++etat;
	    Etoile[tt].fleche=X[i++];
	    tt++;
	 }
      }
   }while(i!=lgxi);
}





//Affiche l'automate en Etoile
void Affiche_Etoile()
{
   int i2;
   cout<<"Affichage de l'automate Etoile de X^*:"<<endl;
   for(i2=1;i2<lgxi-Mots+1;i2++){
      cout<<Etoile[i2].noeud1<<" "<<Etoile[i2].noeud2<<" "<<Etoile[i2].fleche<<endl;
   }
}

void Affiche_Marque()
{
   int i;
   cout<<"MARQUAGE DE :";
   if(IMQ[lmq]==0)
   {
      cout<<Marque[lmq].n[0]<<" ";
   }
   else
   {
      for(i=0;i<IMQ[lmq];i++)
	 cout<<Marque[lmq].n[i]<<" ";
   }
   cout<<" Lg :"<<IMQ[lmq]<<endl;
}


void Copie_Marque2(int ad,int longue)
{
   int i;
   for(i=0;i<longue;i++)
      Marque[lmq].n[i]=ADC[ad].noeud2[i];
   IMQ[lmq]=longue;
   //  Affiche_Marque();
   lmq++;
}

//Pour Noeud2
int Appartient_Existe_Marquage2(int ad,int longue)
{
   int i,i2;
   int resultat=999;
   // cout<<"lmq :"<<lmq<<" ad:"<<ad<<" lg:"<<longue<<endl;
   for(i=0;i<lmq;i++)
   {
      if(IMQ[i]==longue)
      {   resultat=0;
	 for(i2=0;i2<longue;i2++)
	 {
	    // cout<<Marque[i].n[i2]<<"/"<<ADC[ad].noeud2[i2]<<"  ";
	    if(Marque[i].n[i2]==ADC[ad].noeud2[i2])
	       resultat++;
	    else
	       resultat+=10;
	 }
	 if((resultat)==longue)
	    return(0);
      }
   }
   return(resultat);
}

//Permet de Creer l'automate Deterministe et Complet
void Creer_Determinisation()
{
   int adci=0,irecopie=0,i13=0,i14=0,i12=0;
   int in1,in2,itemp,iaf,iaem=0;
   int antilmq=1;
   //PREMIER PARTIE
   cout<<"Creation de l'automate Deterministe et Complet"<<endl;
   for(iaf=0;iaf<lgai;iaf++)
   {
      in1=0;
      in2=0;
      ADC[adci].noeud1[in1]=1;
      ADC[adci].fleche=Alphabet[iaf];
      for(itemp=1;itemp<tt;itemp++)
      {
	 if((Etoile[itemp].noeud1==ADC[adci].noeud1[in1])
	    &&(Etoile[itemp].fleche==Alphabet[iaf]))
	 {
	    ADC[adci].noeud2[in2++]=Etoile[itemp].noeud2;
	 }
      }
      if(in2==0)
      {
	 ADC[adci].noeud2[in2++]=-1;
      }
      IN2[adci]=in2;
      IN1[adci]=in1;
      adci++;
   }
   Marque[0].n[0]=1;
   IMQ[lmq]=1;
   //	Affiche_Marque();
   lmq++;
   for(int i=0;i<lgai;i++)
   {
      iaem=Appartient_Existe_Marquage2(i,IN2[i]);
      if(iaem!=0)
	 Copie_Marque2(i,IN2[i]);
   }
   //FIN DE LA PREMIERE PARTIE
   //DEUXIEME PARTIE
   do{
      //NOUVELLE RECOPIE
      for(i14=adci;i14<adci+lgai;i14++)
      {
	 for(irecopie=0;irecopie<IMQ[antilmq];irecopie++)
	 {
	    ADC[i14].noeud1[irecopie]=Marque[antilmq].n[irecopie];
	 }
	 IN1[i14]=IMQ[antilmq];
	 ADC[i14].fleche=Alphabet[i14-adci];
      }
      //FIN RECOPIE
      for(i14=0;i14<lgai;i14++)
      {
	 i13=0;
	 for(i12=0;i12<IN1[adci];i12++)
	 {
	    for(itemp=1;itemp<tt;itemp++)
	    {
	       if((ADC[adci].noeud1[i12]==Etoile[itemp].noeud1)
		  &&(ADC[adci].fleche==Etoile[itemp].fleche))
	       {

		  ADC[adci].noeud2[i13]=Etoile[itemp].noeud2;
		  i13++;
	       }
	    }
	 }
	 if(i13==0){
	    ADC[adci].noeud2[i13]=-1;
	    i13++;
	 }
	 IN2[adci]=i13;
	 //ESSAI
	 iaem=Appartient_Existe_Marquage2(adci,i13);
	 //cout<<"IAEM :"<<iaem<<endl;
	 if(adci==MAX)
	 {
	    cout<<"Pas assez de memoire"<<endl;
	    exit(0);
	 }
	 adci++;
	 if(iaem!=0){
	    Copie_Marque2(adci-1,i13);
	 }
	 //
      }
      antilmq++;
   }while(antilmq!=lmq);
   //FIN DEX
   lgadc=adci;
}

//Affiche l'automate Deterministe et Complet
void Afficher_Determinisation()
{
   int i,i1,i2;
   cout<<"Affichage de l'automate D&C :"<<endl;
   for(i1=0;i1<lgadc;i1++)
   {
      if(IN1[i1]==0){
	 cout<<ADC[i1].noeud1[0]<<" ";  }
      else
      {
	 for(i2=0;i2<IN1[i1];i2++)
	    cout<<ADC[i1].noeud1[i2]<<" ";
      }
      cout<<ADC[i1].fleche<<" ";
      if(IN2[i1]==0)
	 cout<<ADC[i1].noeud2[0]<<" ";
      else
      {
	 for(i=0;i<IN2[i1];i++)
	    cout<<ADC[i1].noeud2[i]<<" ";
      }
      cout<<endl;
   }
}


void Delete_1()
{
   delete []Alphabet;
   cout<<"Destruction de l'alphabet"<<endl;
   delete []ADC;
   cout<<"Destruction de l'automate deterministe et complet "<<endl;
   delete []IN1;
   delete []IN2;
   cout<<"Destruction de IN1 et IN2"<<endl;
   delete []Marque;
   delete []IMQ;
   cout<<"Destruction de Marque et IMQ"<<endl;

}

void Transformer()
{
   int i,i2=0,ii,i3=0,iaem=0;
   delete []X;
   cout<<"Destruction de X"<<endl;
   delete []Etoile;
   cout<<"Destruction de l'automate en Etoile"<<endl;
   A=new NAtome[lmq*lgai];   //Allocation memoire de l'automate Etoile
   cout<<"Allocation de NAtome de taille :"<<lgadc<<endl;
   cout<<"Construction de A"<<endl;
   for(i3=0;i3<lmq;i3++)
   {
      i2++;
      for(i=0;i<lgadc;i++)
      {
	 iaem=0;
	 if(IMQ[i3]==1 && IN1[i]==0)
	 {
	    if(ADC[i].noeud1[0]!=Marque[i3].n[0])
	       iaem++;
	 }
	 else
	 {
	    if(IMQ[i3]==IN1[i])
	    {
	       for(ii=0;ii<IN1[i];ii++)
	       {
		  if(ADC[i].noeud1[ii]!=Marque[i3].n[ii])
		     iaem++;
	       }
	    }
	    else
	    {
	       iaem++;
	    }
	 }
	 if(iaem==0)
	 {
	    A[i].n1=i2;
	    A[i].fleche=ADC[i].fleche;
	    if(A[i].n1==1)
	       A[i].etat=1;
	    else
	    {
	       if(ADC[i].noeud1[0]==1 && IN1[i]!=0)
		  A[i].etat=3;
	       else
		  A[i].etat=2;
	    }
	 }
	 iaem=0;
	 if(IMQ[i3]==IN2[i])
	 {
	    for(ii=0;ii<IN2[i];ii++)
	    {
	       if(ADC[i].noeud2[ii]!=Marque[i3].n[ii])
		  iaem++;
	    }
	 }
	 else
	 {
	    iaem++;
	 }
	 if(iaem==0)
	 {
	    A[i].n2=i2;
	 }
      }

   }
   //Aj:
   c=lmq;
   Delete_1();
}


void Affiche_Transformation()
{
   cout<<"Affichage de l'automate :"<<endl;
   for(int i=0;i<(lmq*lgai);i++)
   {
      cout<<A[i].n1<<" "<<A[i].fleche<<" "<<A[i].n2<<" Etat :"<<A[i].etat<<endl;
   }
}

void Entrer_Etat()
{
   int i,i2,nbe;
   cout<<"Entrer le nombre d'etats"<<endl;
   cin>>nbe;
   nbe++;
   A=new NAtome[nbe];
   Saisie_Alphabet();
   //c=nbe;
   nbe=nbe*lgai;
   for(i=0;i<nbe;i++)
   {
      //    cout<<"n1:";
      cin>>A[i].n1;
      //  cout<<"n2:";
      cin>>A[i].n2;
      //  cout<<"-->";
      cin>>A[i].fleche;
      // cout<<"Etat:";
      cin>>A[i].etat;

   }
   c=nbe;
   /* for(i=0;i<nbe;i++)
   {
   cout<<A[i].n1<<" "<<A[i].fleche<<" "<<A[i].n2<<" Etat :"<<A[i].etat<<endl;
}*/

}

void Init_Deb_M()
{
   int i;
   *M1=new int[c];
   *M2=new int[c];
   for(i=0;i<c;i++)
   {
      M1[i]=new int[c];
      M2[i]=new int[c];
   }
}


void Init_Matrice_M1()
{
   int i,i1;
   for(i=0;i<c;i++)
   {
      for(i1=0;i1<c;i1++)
      {
	 M1[i][i1]=-1;
      }
   }
}



void Init_Matrice_M2()
{
   int i,i1;
   for(i=0;i<c;i++)
   {
      for(i1=0;i1<c;i1++)
      {
	 M2[i][i1]=-1;
      }
   }
}


void Copie_Matrice_1_vers_2()
{
   int i,i1;
   Init_Matrice_M2();
   for(i=0;i<c;i++)
   {
      for(i1=0;i1<c;i1++)
      {
	 M2[i][i1]=M1[i][i1];
      }
   }
}

void Copie_Matrice_2_vers_1()
{
   int i,i1;
   Init_Matrice_M1();
   for(i=0;i<c;i++)
   {
      for(i1=0;i1<c;i1++)
      {
	 M1[i][i1]=M2[i][i1];
      }
   }
}

//retourne 0 si Idem
int Matrice_Idem()
{
   int i,i1,res=1;
   for(i=0;i<c;i++)
   {
      for(i1=0;i1<c;i1++)
      {
	 if(M2[i][i1]!=M1[i][i1])
	    return(res);
      }
   }
   res=0;
   return(res);
}

void Affiche_M1()
{
   int i,i1;
   for(i=0;i<c;i++)
   {
      for(i1=0;i1<c;i1++)
      {
	 cout<<M1[i][i1]<<" ";
      }
      cout<<endl;
   }
}

void Affiche_M2()
{
   int i,i1;
   for(i=0;i<c;i++)
   {
      for(i1=0;i1<c;i1++)
      {
	 cout<<M2[i][i1]<<" ";
      }
      cout<<endl;
   }
}

//retourne le SSGRP
int Appartient_SSGrp(int nv1,int grp,int r)
{
   int ii=0;
   while(M1[grp][ii]!=-1)
   {
      if(A[nv1].n2==A[M1[grp][ii]].n1)
	 return(grp);
      ii++;
   }
   ii=0;
   while(M1[r][ii]!=-1)
   {
      if(A[nv1].n2==A[M1[r][ii]].n1)
	 return(r);
      ii++;

   }
   return(-1);
}



void Verifie_Position(int ipos,int g)
{
   int i=0,i1=0;
   for(i=0;i<ipos;i++)
   {
      if(Pos[i]!=Pos[++i])
      {
	 cout<<"Bad :";
      }
      cout<<Pos[i-1]<<"/"<<Pos[i]<<endl;
   }

}

void Appartient()
{
   int n3,n4,i=0,grp=0;
   Pos=new int[c];
   while(M1[grp][i+lgai]!=-1)
   {
      Pos[pos++]=Appartient_SSGrp(M1[grp][i],grp,grp+1);
      Pos[pos++]=Appartient_SSGrp(M1[grp][i+lgai],grp,grp+1);
      i++;
   }
   for(i=0;i<pos;i++)
   {
      cout<<Pos[i]<<" ";
   }
   Verifie_Position(pos,grp);
   cout<<endl;
   pos=0;
   cout<<"Etage suivant:"<<endl;
   grp++;
   i=0;
   while(M1[grp][i+lgai]!=-1)
   {
      Pos[pos++]=Appartient_SSGrp(M1[grp][i],grp,grp-1);
      Pos[pos++]=Appartient_SSGrp(M1[grp][i+lgai],grp,grp-1);
      i++;
   }
   Verifie_Position(pos,grp);
   for(i=0;i<pos;i++)
   {
      cout<<Pos[i]<<" ";
   }
}



void Minimisation()
{
   int ic1=0,ic2=0,i=0;
   int cx=0;
   Init_Deb_M();
   Init_Matrice_M1();
   Init_Matrice_M2();
   //Premiere partie
   for(i=0;i<c;i++)
   {
      if(A[i].etat==1 || A[i].etat==3)
	 M1[cx][ic1++]=i;      //Position
      else
	 M1[cx+1][ic2++]=i;
   }
   Appartient();
   /*
   while(Matrice_Idem){}
   *///Deuxieme Partie
}




//Permet de liberer la memoire allouer
void Delete_Totale()
{
   delete []A;
   delete []Pos;

}

void Old()
{
   Entrer_Etat();
   Minimisation();
}

//Procedure Principale
void main()
{
     cout<<"VERSION "<<VERSION<<endl;
   Old();
   Delete_Totale();
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----FONCTIONS GENERALES



//Afin d'eviter un "buffer overflow",
//j'implemante une fonction qui va vider le buffer clavier.

void clear_buffer()
{
  int c = 0;
  while (c != '\n' && c != EOF)
    {
      c = getchar();
    }
}


//La fonction ci-dessous va se comporter comme la fonction fgets
//puis va supprimer le \n en fin de chaine.
//Si la chaine de caracteres en argument est trop longue ou nulle
//la fonction vide le buffer
int lire(char *chaine, int longueur)
{
  char *positionEntree = NULL;

  if (fgets(chaine, longueur, stdin) != NULL)
    {
      positionEntree = strchr(chaine, '\n');
      if (positionEntree != NULL)
        {
          *positionEntree = '\0';
        }
      else
        {
          clear_buffer();
        }
      return 1;
    }
  else
    {
      clear_buffer();
      return 0;
    }
}

void clear_screen ()
{
  printf ("\n===============================\n\n");
}

//Ne nettoie pas l'ecran mais permet une meilleure visibilite des etapes importantes.

//-----Definition, affichage et creation d'un attaque-------
typedef struct attaque *Attaque;
struct attaque
{
  char* nom;
  int force;
  float prob;
};

Attaque new_attack (char* name, int puiss, float proba)
{
  Attaque a = malloc (sizeof (struct attaque));
  a->nom = name;
  a->force = puiss;
  a->prob = proba;
  return (a);
}

void get_attack (Attaque atq)
{
  printf ("[%s, %d, %f]\n",atq->nom, atq->force, atq->prob);
}





//-----Definition, affichage et cration d'un pokemon-----
typedef struct pokemon *Pokemon;
struct pokemon
{
  char* nom;
  int hp_max;
  int hp;
  Attaque attaques[4];
};


Pokemon new_pokemon (char* name, int pv_max, Attaque a1, Attaque a2, Attaque a3, Attaque a4)
{
  Pokemon pkmn = malloc (sizeof (struct pokemon));
  pkmn->nom = name;
  pkmn->hp_max = pv_max;
  pkmn->hp = pv_max;
  pkmn->attaques[0] = a1;
  pkmn->attaques[1] = a2;
  pkmn->attaques[2] = a3;
  pkmn->attaques[3] = a4;
  return (pkmn);
}


void get_pokemon (int i, Pokemon pkmn)
{
  if (i == 0)
    {
      printf ("%s    %d/%d\n", pkmn->nom , pkmn->hp, pkmn->hp_max);
    }
  else
    {
      printf ("%s    %d/%d\n", pkmn->nom , pkmn->hp, pkmn->hp_max);
      printf ("1.%s\n2.%s\n3.%s\n4.%s\n", pkmn->attaques[0]->nom, pkmn->attaques[1]->nom, pkmn->attaques[2]->nom, pkmn->attaques[3]->nom);
    }
}


//-----Implementation de la fonction hit
void hit (Attaque atq, Pokemon x, Pokemon y)
{
  float i = (float)(rand () % 100)/100;
  if (i <= atq->prob )
    {
      y->hp -= atq->force;
      printf("%s utilise %s <Appuyez sur ENTREE pour continuer>\n", x->nom, atq->nom);
      clear_buffer();
      printf("%s a subi %d points de degats <Appuyez sur ENTREE pour continuer>\n", y->nom, atq->force);
    }
  else
    {
      printf("%s utilise %s <Appuyez sur ENTREE pour continuer>\n", x->nom, atq->nom);
      clear_buffer();
      printf("mais %s esquive <Appuyez sur ENTREE pour continuer>\n", y->nom);
    }
    clear_buffer();
    clear_screen ();
}



//-----Implementation de la fonction combat

int combat (Pokemon x, Pokemon y, int i) //L'entier i va permettre de compter le nombre total d'actions réalisées.
//Si ce nombre est impair alors le vainqueur est le dresseur 1, sinon le dresseur 2 gagne cette manche.
{
  int val = 0; //val va etre la condition pour sortir de la boucle while.
  //L'utilisateur ne pourra pas sortir tant qu'il n'aura pas fait une action correcte
  printf("---------Pokemon de l'adversaire---------\n");
  get_pokemon (0, y);
  printf("\n---------Votre Pokemon---------\n");
  get_pokemon (1, x);

  while (val == 0)
    {
         printf("\n Dresseur %d, veuillez selectionner une attaque \n", (i % 2) + 1);
      int num_atq = getc(stdin);//getc va renvoyer le code ASCII du premier caractere de la chaine rentree par l'utilisateur.
      clear_buffer();//on nettoie le buffer au cas ou plus d'un caractere a ete utilise.
      switch (num_atq)//on filtre sur le code ASCII
        {
        case 49 ://Si l'utilisateur a tape 1 comme premier caractere
        {
          hit (x->attaques[0],x , y);
          val = 1;
        }
        break;
        case 50 : //Si l'utilisateur a tape 2 comme premier caractere
        {
          hit (x->attaques[1],x , y);
          val = 1;
        }
        break;
        case 51 : //Si l'utilisateur a tape 3 comme premier caractere
        {
          hit (x->attaques[2],x , y);
          val = 1;
        }
        break;
        case 52 : //Si l'utilisateur a tape 4 comme premier caractere
        {
          hit (x->attaques[3],x , y);
          val = 1;
        }
        break;
        default : //Si l'utilisateur a tape autre chose comme premier caractere
          printf ("Action incorrecte, l'attaque doit etre comprise entre 1 et 4.\n\n");
          break;
        }
    }

  if (y->hp <= 0)
    {
      return (i % 2);
    }
  i++;
  return (combat(y, x, i));
}

//-----Creation d'un dersseur
typedef struct pokemon_list *Liste;
struct pokemon_list
{
  Pokemon starter;
  Liste suivant;
};
//pokemon_list est une liste chainee de pokemons.

typedef struct dresseur *Dresseur;
struct dresseur
{
  char *nom;
  Liste pokemons;
};

Liste cons (Pokemon pkmn, Liste l)
{
  Liste p;
  p = malloc(sizeof(struct pokemon_list));
  p->starter = pkmn;
  p->suivant = l;
  return (p);
}

Liste new_pok (Pokemon pkmn, Liste l)
{
  Liste originale = l;
  Liste p = cons(pkmn, NULL);
  if (l==NULL) return (p);
  while (l->suivant != NULL)
    {
      l=l->suivant;
    };
  l->suivant = p;
  return (originale);
}

void get_pokemons(Liste l)
{
  if (l==NULL)
    {
      printf("Personne!\n");
      return ;
    };
  while (l->suivant != NULL)
    {
      printf("%s, ",l->starter->nom);
      l=l->suivant;
    }
  printf("et %s\n", l->starter->nom);
}



void get_dresseur (Dresseur d)
{
  printf ("%s et son equipe constitue de :\n",d->nom);
  get_pokemons (d->pokemons);
}


Dresseur add_pok (Pokemon pkmn, Dresseur d)
{
  d->pokemons = new_pok (pkmn, d->pokemons);
  return (d);
}

Dresseur name_dresseur (char* name)
{
  Dresseur d = malloc(sizeof(struct dresseur));
  d->nom = name;
  d->pokemons = NULL;
  return (d);
}


void combat_final_aux (Dresseur d1, Dresseur d2)
{
  FILE* logs = NULL;
  logs = fopen ("logs.txt","a+");
  int res;
  while ((d1->pokemons != NULL) || (d2->pokemons != NULL))
    {
      res = combat (d1->pokemons->starter, d2->pokemons->starter, 0);
      if (res == 0)
        {
          fprintf (logs, "%s vs %s || %s vs %s || Vainqueur : %s\n", d1->nom, d2->nom, d1->pokemons->starter->nom, d2->pokemons->starter->nom, d1->pokemons->starter->nom);
          if (d2->pokemons->suivant == NULL)
            {
                printf ("%s est K.O\n\n", d2->pokemons->starter->nom);
                printf ("%s remporte le duel!\n", d1->nom);
              fprintf(logs,"Fin du duel\nLe vainqueur est %s\n\n", d1->nom);
              fclose(logs);
              break;
            }
          printf ("%s est K.O\n\n", d2->pokemons->starter->nom);
          d2->pokemons = d2->pokemons->suivant;
        }
      else
        {
          fprintf (logs, "%s vs % || %s vs %s || Vainqueur : %s\n", d1->nom, d2->nom, d1->pokemons->starter->nom, d2->pokemons->starter->nom, d2->pokemons->starter->nom);
          if (d1->pokemons->suivant == NULL)
            {
              printf ("%s remporte le duel!\n", d2->nom);
              fprintf(logs,"Fin du duel\nLe vainqueur est %s\n\n", d2->nom);
              fclose(logs);
              break;
            }
          printf("%s est K.O\n\n",d1->pokemons->starter->nom);

          d1->pokemons = d1->pokemons->suivant;
        };
        clear_screen ();
    }

}


void combat_final ()
{
  //Entrée du nom du 1er dresseur
  printf("Dresseur 1, quel est votre nom\n");
  char name_1[20];
  lire (name_1,20);
  Dresseur d1 = name_dresseur (name_1);


  //Ajout des pokemons du dresseur 1
  add_pok (new_pokemon ("Roucarnage", 370, new_attack("Aeropique", 60, 1.0), new_attack("Vol", 90, 0.65), new_attack("Cru-Aile", 60, 1.0), new_attack("Lame d'Air", 75, 0.85)), d1);
  add_pok (new_pokemon ("Salameche", 282, new_attack("Deflagration", 120, 0.45), new_attack("Lance-Flamme", 95, 0.65), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d1);
  add_pok (new_pokemon ("Bulbizarre", 294, new_attack("Tonnerre", 50, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d1);
  add_pok (new_pokemon ("Ronflex", 524, new_attack("Tonnerre", 50, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d1);
  add_pok (new_pokemon ("Krabboss", 314, new_attack("Tonnerre", 50, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d1);
  add_pok (new_pokemon ("Pikachu", 274, new_attack("Tonnerre", 50, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d1);



  //Entrée du nom du deuxième dresseur
  printf("\n\nDresseur 2, quel est votre nom?\n");
  char name_2[20];
  lire (name_2, 20);
  Dresseur d2 = name_dresseur (name_2);

  //Ajout des pokemons du dresseur 2
  add_pok (new_pokemon ("Aquali", 50, new_attack("Tonnerre", 25, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d2);
  add_pok (new_pokemon ("Elektek", 50, new_attack("Tonnerre", 50, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d2);
  add_pok (new_pokemon ("Mackogneur", 50, new_attack("Tonnerre", 50, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d2);
  add_pok (new_pokemon ("Racaillou", 50, new_attack("Tonnerre", 50, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d2);
  add_pok (new_pokemon ("Smogo", 50, new_attack("Tonnerre", 50, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d2);
  add_pok (new_pokemon ("Leviator", 50, new_attack("Tonnerre", 50, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d2);

  clear_screen ();
  printf ("Bienvenue a la finale de ce tournoi Pokemon opposant :\n\n" );
  get_dresseur(d1);
  printf("\net\n\n");
  get_dresseur(d2);
  printf("\nQue le combat commence! <Appuyez sur ENTREE pour continuer>\n");
  clear_buffer();
  clear_screen ();
  combat_final_aux (d1, d2);


}

int main ()
{

  combat_final ();
  return (0);
}

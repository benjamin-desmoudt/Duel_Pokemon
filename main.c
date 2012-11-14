#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void clear_buffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

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



void hit (Attaque atq, Pokemon x, Pokemon y)
{
  float i = (float)(rand () % 100)/100;
  if (i <= atq->prob )
    {
      y->hp -= atq->force;
      printf("%s utilise %s\n", x->nom, atq->nom);
      printf("%s a subi %d points de degats\n\n\n", y->nom, atq->force);
    }
  else
    {
      printf("%s utilise %s\n", x->nom, atq->nom);
      printf("mais %s esquive\n\n\n", y->nom);
    }
}


int combat (Pokemon x, Pokemon y, int i)
{
  printf("---------Pokemon de l'adversaire---------\n");
  get_pokemon (0, y);
  printf("\n---------Votre Pokemon---------\n");
  get_pokemon (1, x);
   printf("\n Veuillez selectionner une attaque \n");
   int num_atq = getc(stdin);
  switch (num_atq)
  {
      case 49 :
      {
       hit (x->attaques[0],x , y);
      clear_buffer();
      }

      break;
      case 50 :
      {
      hit (x->attaques[1],x , y);
      clear_buffer();
      }
      break;
      case 51 :
      {
      hit (x->attaques[2],x , y);
      clear_buffer();
      }
      break;
      case 52 :
      {
      hit (x->attaques[3],x , y);
      clear_buffer();
      }
      break;
      default :
      printf ("Action incorrecte, l'attaque doit etre comprise entre 1 et 4.\n\n");
      break;

  }

  if (y->hp <= 0)
    {
      return (i % 2);
    }
  i++;

  return (combat(y, x, i));
}


typedef struct pokemon_list *Liste;
struct pokemon_list
{
  Pokemon starter;
  Liste suivant;
};


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
      printf("[]\n");
      return ;
    };
  printf("[");
  while (l->suivant != NULL)
    {
      printf("%s; ",l->starter->nom);
      l=l->suivant;
    }
  printf("%s]\n", l->starter->nom);
}



void get_dresseur (Dresseur d)
{
  printf ("%s\n",d->nom);
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

void combat_final ()
{
  //Entrée du nom du 1er dresseur
  printf("Dresseur 1, veuilez entrer votre nom. (20 caracteres max.)\n");
  char name_1[20];
  lire (name_1,20);
  Dresseur d1 = name_dresseur (name_1);


  //Ajout des pokemonsdu dresseur 1
  add_pok (new_pokemon ("Pikachu", 50, new_attack("Tonnerre", 25, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d1);
  add_pok (new_pokemon ("Raichu", 50, new_attack("Tonnerre", 50, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d1);
  get_dresseur (d1);


  //Entrée du nom du deuxième dresseur
  printf("Dresseur 2, veuillez entrer votre nom. (20 caracteres max)\n");
  char name_2[20];
  lire (name_2, 20);
  Dresseur d2 = name_dresseur (name_2);

  //Ajout des pokemons du dresseur 2
  add_pok (new_pokemon ("Ratata", 50, new_attack("Tonnerre", 25, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d2);
  add_pok (new_pokemon ("Elektek", 50, new_attack("Tonnerre", 50, 1.0), new_attack("Fatal Foudre", 50, 0.0), new_attack("Charge", 50, 0.6), new_attack("Tacle", 50, 0.6)), d2);
  get_dresseur (d2);


  int res;
  while ((d1->pokemons != NULL) || (d2->pokemons != NULL))
    {
      res = combat (d1->pokemons->starter, d2->pokemons->starter, 0);
      if (res == 0)
        {
          if (d2->pokemons->suivant == NULL)
            {
              printf ("%s remporte le duel!\n", name_1);
              break;
            }

          d2->pokemons = d2->pokemons->suivant;
        }
      else
        {
          if (d1->pokemons->suivant == NULL)
            {
              printf ("%s remporte le duel!\n", name_2);
              break;
            }
          d1->pokemons = d1->pokemons->suivant;
        };
    }
}

int main ()
{
  combat_final ();
  return (0);
}

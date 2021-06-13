# Projet de LP25

L'évaluation de la partie programmation en langage C de LP25 se base sur un projet à réaliser en groupes de 4 étudiants. Ce projet doit être réalisé en langage C.

## Descriptif global du projet

Ce projet a pour but d'écrire un programme qui va scanner récursivement un répertoire, et en extraire l'arborescence complète de ce répertoire. Cette arborescence devra être stockée dans un premier temps dans la mémoire de l'ordinateur au cours de l'exécution du programme, puis ce contenu, une fois trié, devra être écrit dans un fichier.

## Description détaillées des fonctionnalités

Les fonctionnalités du programme à développer sont détaillées dans les paragraphes ci-dessous. Le cas échéant, le paragraphe vous donne le paramètre par défaut utilisé par cette fonctionnalité, en l'absence de toute option passée au programme.

### Scan du répertoire

Par défaut : scanne `.`

En partant du répertoire indiqué, le programme va créer la liste des fichiers et répertoires contenus (à l'exception de `.` et `..`). Attention, il ne faut pas suivre les liens symboliques (il y a risque de boucle infinie). Puis, le programme rentre successivement dans les éventuels répertoires contenus par le répertoire cible et procède au même traitement, jusqu'à atteindre l'extrémité de l'arborescence du dossier passé en paramètre.

Vous traiterez les types de fichiers de la manière suivante :

 - fichier : nom, taille, date de modification, somme MD5
 - dossier : nom, date de modification, puis traiter son contenu
 - lien symbolique : nom, fichier pointé, date de modification
 - autre : nom, date de modification

Évidemment, il faut tester que le paramètre donnant la cible du scan est bien un dossier valide.

### Stockage en mémoire

Le stockage en mémoire est fait sous forme arborescente. L'idée est de partir d'une structure composée de dossiers et de fichiers, et de chaîner ces éléments.

Deux structures définiront le contenu en mémoire, sous forme de listes chaînées :

```c
#include <linux/limits.h>
#include <time.h>
#include <openssl/md5.h>

typedef enum {DIRECTORY, REGULAR_FILE, OTHER_TYPE} e_type;

typedef struct _file {
	e_type file_type;
	char name[NAME_MAX+1];
	time_t mod_time;
	uint64_t file_size;
	u_char md5sum[MD5_DIGEST_LENGTH];
	struct _file *next_file;
} s_file;

typedef struct _directory {
	char name[NAME_MAX+1];
	time_t mod_time;
	struct _directory *subdirs;
	s_file *files;
	struct _directory *next_dir;
} s_directory;
```

La structure `s_file` ne requiert pas de valeurs dans tous ses champs, en fonction du type réel du fichier considéré (fichier standard, ou fichier spécial incluant les liens symboliques).

### sauvegarde dans un fichier

Par défaut dans `~/.filescanner/yyyy-MM-dd-hh:mm:ss.scan`. Créer le dossier `~/.filescanner` si nécessaire.

Le fichier contient une ligne par entrée. Les informations sont stockées séparées par des tabulations (caractère `'\t'`).

Une ligne de répertoire se présente sous la forme suivante : `0 date chemin/` où :

 - `0` est la valeur pour le type DIRECTORY dans l'énumération `e_type`;
 - `date` est la date de modification du répertoire;
 - `chemin` est son chemin complet (il faut y penser quand vous parcourez la mémoire) terminé par un `/`. Les deux champs sont séparés par une tabulation.

Une ligne de fichier standard se présente sous la forme `1 date taille MD5 chemin` où :

 - `1` est le code de `REGULAR_FILE`;
 - `date` est la date comme définie pour le dossier;
 - `taille` est la taille en octets du fichier;
 - `MD5` est la somme MD5 du fichier en représentation hexadécimale;
 - `chemin` est le chemin complet vers le fichier depuis la racine du scan. Pour les fichiers, il n'y a pas de `/` à la fin du chemin.

Une ligne de fichier autre (lien, etc.) se présente sous la forme `2 date chemin` où `2` est le code de `OTHER_TYPE`, et `date` et `chemin` sont définis de la même manière que pour un fichier standard.

L'écriture dans le fichier doit être faite dans l'ordre suivant :

 - le dossier en cours de traitement
 - ses fichiers
 - ses sous dossiers
 - puis on traite les contenus des sous dossiers

### Paramétrage du comportement

Le comportement de l'application peut être paramétré suivant plusieurs options :

 - L'option **-o** suivie d'un argument permet de spécifier le fichier dans lequel la sauvegarde de la liste de l'arborescence se fera.
 - L'option **-s** sans argument active le calcul des sommes MD5 des fichiers.
 - L'option **-i** suivie d'un argument spécifie le dossier à analyser.

## Décomposition du programme

Le programme sera séparé en plusieurs fichiers de code, chacun ayant un rôle précis.

### main

Le fichier `main.c` contiendra la fonction `main` qui aura deux rôles :

 - Tester les options passées au programme pour son paramétrage, en utilisant getopt;
 - Appeler les fonctions des autres fichiers pour scanner le répertoire cible, construire la structure en mémoire, et l'écrire dans le fichier de scan.

### Scan

Les fichiers `scan.h` et `scan.c` contiendront respectivement les signatures et les définitions des fonctions nécessaires au scan, notamment :

```c
s_directory *process_dir(char *path);
s_file *process_file(char *path);
```
Ces fonctions traitent respectivement les dossiers (en lisant leur contenu) et les fichiers (en testant leur type et en calculant les informations nécessaires). Elles renvoient un pointeur vers une structure de dossier ou de fichier, NULL en cas de problème. Le pointeur renvoyé a pour but d'ajouter l'élément à la liste en mémoire. Il ne faut pas oublier de libérer tous ces pointeurs à la fin de l'exécution du programme. Ceci sera vérifié et participera à la note du projet.

### Save

Les fichiers `save.h` et `save.c` contiendront respectivement les signatures et les définitions des fonctions pour la sauvegarde de la structure en mémoire résultant du scan. _A minima_, on y trouvera :

```c
int save_to_file(s_directory *root, char *path_to_target);
```
Cette fonction écrira dans le fichier accessible par le chemin `path_to_target` l'ensemble des informations de la structure pointée par `root`. La fonction renvoie 0 en cas d'échec, -1 sinon. Il est fortement conseillé de créer des fonctions additionnelles pour traiter séparément l'écriture des différents types d'enregistrements.

### Tree

Les fichiers `tree.h` et `tree.c` contiendront les signatures et les définitions des fonctions pour manipuler l'arborescence sauvée en mémoire. Notamment, on y trouvera :

```c
int append_subdir(s_directory *child, s_directory *parent);
int append_file(s_file *child, s_directory *parent);
void clear_files(s_directory *parent);
void clear_subdirs(s_directory *parent);
```

Les fonctions `append_subdir` et `append_file` vont respectivement ajouter un sous-répertoire ou un fichier pointés par `child` à la liste des sous répertoires ou des fichiers du noeud pointé par `parent`

Les fonctions `clear_files` et `clear_subdirs` vont respectivement libérer la mémoire des listes des fichiers et des sous répertoires du noeud pointé par `parent`.

Attention ! La fonction `clear_subdirs` doit d'abord libérer récursivement le contenu des sous-répertoires avant de pouvoir les libérer.

### MD5

Les fichiers `md5sum.h` et `md5sum.c` contiendront respectivement les signatures et les définitions des fonctions relatives au calcul de la somme MD5 d'un fichier, notamment la fonction suivante :

```c
int compute_md5(char *path, unsigned char buffer[]);
```
Cette fonction calcule la somme MD5 du fichier accessible par le chemin `path`, et en stocke la valeur dans le tableau de caractères non signés `buffer`.

Vous pouvez utiliser d'autres fonctions appelées par celle-ci pour faciliter votre développement.

## Méthode de travail

Afin de limiter vos conflits de code (i.e. des modifications incompatibles sur des fichiers), il est conseillé que vous vous répartissiez les tâches sur des fonctionnalités différentes, en changeant périodiquement de rôle afin de permettre à chacun de s'approprier les notions et la technique relative à l'ensemble du projet.

Il vous sera également nécessaire de planifier des synchronisations **au minimum toutes les deux semaines, de préférence chaque semaine**. Ces points d'avancement feront l'objet de compte rendus qui seront à inclure au rapport du projet.

## Bonus

Quelques points de bonus sont réservés aux groupes dont la sauvegarde dans un fichier se fera, non plus en mode texte, mais dans une base de données SQLite. Vous pouvez consulter la [documentation de l'API SQLite pour C/C++](https://www.sqlite.org/cintro.html) pour plus d'informations.

## Rendu

Le document principal à remettre est le code du projet, sur git-info, accompagné du Makefile permettant sa compilation, et d'un README.md qui décrit les éventuelles bibliothèques à installer, ainsi que l'utilisation de l'application.

Par ailleurs, un rapport succinct sera à remettre, en deux temps : à mi-projet (date à définir), les choix arrêtés par le groupe devront être définis et justifiés (choix ou non de l'intégration de SQLite, ajout de fonctions dans les différents modules du programme, etc.).

Ce rapport sera ensuite enrichi du RETEX du projet une fois celui-ci terminé. Pour réaliser ce RETEX, vous prendrez à tour de rôle la direction de la rédaction d'un "compte rendu de mission", dans lequel vous interrogerez vos camarades de projet sur les points qui vous paraissent nécessiter une amélioration et/ou un éclaircissement, ainsi que sur la question suivante : _"En se basant sur le déroulement du projet, aujourd'hui que ferais-tu différemment ?"_. Les compte rendus de tous les membres du groupes devront ensuite être synthétisés en RETEX et ajoutés au rapport pour produire le document final à rendre en fin de semestre.

Il vous sera probablement également demandé de présenter ce rapport et le projet dans un exposé oral, soit en face à face, soit en visioconférence.

## Annexes

Dans ces annexes, quelques informations vous sont données sur l'utilisation des API non vues en cours, TD ou TP.

### MD5

La bibliothèque pour calculer des sommes MD5 est fournie avec OpenSSL. Pour l'utiliser, il faut inclure `openssl/MD5.h` dans votre fichier source. Puis, vous devez lier la bibliothèque dynamique libcrypto à votre programme (option `-lcrypto` à l'édition des liens)

Il est nécessaire d'ouvrir le fichier dont vous calculez la somme MD5 et de le lire par parties (à vous de choisir la taille des éléments que vous traitez) afin de permettre à MD5 de calculer la somme sur la taille complète du fichier.

Quelques informations supplémentaires au sujet de MD5 vous seront données ultérieurement.

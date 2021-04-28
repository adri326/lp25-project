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

## Bonus

Quelques points de bonus sont réservés aux groupes dont la sauvegarde dans un fichier se fera, non plus en mode texte, mais dans une base de données SQLite. Vous pouvez consulter la [documentation de l'API SQLite pour C/C++](https://www.sqlite.org/cintro.html) pour plus d'informations.

## Rendu

Le code du projet, sur git-info, avec un Makefile permettant la compilation du projet, et un README.md qui décrit les éventuelles bibliothèques à installer, ainsi que l'utilisation de l'application.

## Annexes

Dans ces annexes, quelques informations vous sont données sur l'utilisation des API non vues en cours, TD ou TP.

### MD5

La bibliothèque pour calculer des sommes MD5 est fournie avec OpenSSL. Pour l'utiliser, il faut inclure `openssl/MD5.h` dans votre fichier source. Puis, vous devez lier la bibliothèque dynamique libcrypto à votre programme (option `-lcrypto` à l'édition des liens)

Il est nécessaire d'ouvrir le fichier dont vous calculez la somme MD5 et de le lire par parties (à vous de choisir la taille des éléments que vous traitez) afin de permettre à MD5 de calculer la somme sur la taille complète du fichier.

Quelques informations supplémentaires au sujet de MD5 vous seront données ultérieurement.

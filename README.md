# Projet de LP25

## 1. Les bibliothèques à installer

La bibliothèque md5 d'OPENSSL est nécessaire, sinon il faut celle de BSD.
La bibliothèque devra être indiqué lors de l'utilisation du make : soit "make build OPENSSL_MD5=1" ou "make build BSD_MD5=1".

## 2. Utilisation de l'application

L'exécutable se trouve dans "./build/bin/projet" par défaut.
Afin d'utiliser l'application, on peut donner les arguments suivants :
    - "-i" suivi du dossier à analyser, sinon le dossier actuel sera analysé.
    - "-s" sans argument afin d'activer le calcul des sommes MD5, sinon la somme n'est pas calculé et la valeur sera 0.
    - "-o" suivi d'un fichier dans lequel la sauvegarde de la liste de l'arborescence se fera, sinon la sauvegarde se fera dans "./result_of_scan.txt".
    - "-v" pour activer le mode verbose

## 3. Tests automatisés

Des tests automatisés se trouvent dans `test/`. Pour les compiler, vous aurez besoin de `cmake` et de la [librairie `check`](https://libcheck.github.io/check/).
Ensuite, compilez le programme avec `cmake`:

```sh
# Créer le dossier de sortie:
mkdir -p build
cd build

# Compiler le projet et les tests:
cmake ..
make

# Lancer les tests:
./lp25-project-test
```

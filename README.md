# PLD - COMP

Ceci est un compilateur pour les sous-ensembles du langage C suivants: x86, (MSP430, ARM)

## Installation

Antlr4 ( à faire qu'une seule fois sur chaque machine - dans le répertoire local)

```bash
./install-antlr.sh
```

## Utilisation
Compilation sous Linux
```
cd ./compiler
make ou runmake_ubuntu.sh
```
Lancement du script de test
```
cd ./tests
python3 ifcc-test.py ./testfiles/
```
Lancement d'un seul test
```
cd ./tests
python3 ifcc-test.py ./testfiles/_name_test.c
```
Visualisation de l'AST
```
cd ./compiler
make gui FILE=../tests/testfiles/_name_test.c
```
## Fonctionnalités implémentés

- Commentaires sont ignorés
- Variables
- Constante entière et caractère avec simple quote (type int et char)
- Déclaration de variables (n'importe où)
- Affectation simple et affectation lors de la déclaration de la variable
- Vérification statique des variables:
	- Vérifier qu'une variable utilisée dans une expression a été déclarée 
	- Vérifier qu'une variable n’est pas déclarée plusieurs fois
	- Vérifier qu'une variable déclarée est utilisée au moins une fois
- Expressions arithmétiques:
	- Opérations arithmétiques: +,-,*,/
	- Opérations logiques bit-à-bit : |, &, ^
	- Opérations de comparaison : ==, !=, <, >, <=, >=
	- Opérations unaires : ! et - 
- Structure de bloc {}
- Structures de contrôles if, else, while
- Return expression (n'importe où)
- Définition de fonctions sans paramètres avec type de retour int, char ou void
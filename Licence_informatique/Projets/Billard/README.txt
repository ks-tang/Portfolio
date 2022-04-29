Billard
TANG Kévin

DESCRIPTION
Mon projet est une simulation de collisions de boules de billard.
La distance entre le clic de la souris et la boule blanche détermine la puissance du tir. La boule avance dans la direction opposé au clic.

BUT
Le but est de mettre toutes les boules rouges dans les trous sans faire tomber la boule blanche dans ces mêmes trous.

EXPLICATION DU CODE
Le programme commence par afficher la table, les boules et les trous.
Pour faire avancer la boule blanche, il ajoute de la force à la boule (comme avec la gravité dans le TP particules)

Il gère les collisions avec les bords de la table (comme on a vu en TD particules)

Il détecte les collisions entre les boules (si la distance entre deux boules est inférieur à la somme des rayons des boules).
S'il y a collisions entre deux boules, il calcule l'angle d'impact et change l'angle des vecteurs vitesse des boules pour les faire changer de direction.

Meme méthode lorsque les boules tombent dans les "trous" : selon la distance avec le centre du trou et la boule.



HISTORIQUE

- Semaine 1 :
J'ai réalisé l'affichage de la table et des boules.



- Semaine 2 :
J'ai implanté le tir de la boule blanche sur un clic de la souris.
Et la collision avec les bords de la table.

Problèmes rencontrées :
Je n'ai plus internet chez moi depuis le 5 avril (semaine 2).
Aucun rdv disponible avec un technicien de mon opérateur.
Un de mes voisins a bien voulu partager son réseau wifi, nous sommes 3 voisins dessus.

Je ne parviens pas à faire un mouvement progressif de la boule blanche.



- Semaine 3 :
J'ai réussi à faire un mouvement progressif de la boule.

Problèmes:
Ma connexion internet fonctionne de nouveau mais s'arrête plusieurs fois par jour.
Les boules ne s'arrêtent pas même avec une grande friction.



- Semaine 4 :
Après de nombreux essais, j'ai réussi à implémenter les chocs entre les boules (j'ai laissé les tests effectués).
J'ai aussi ajouté les trous de la table de billard.

Problème : à chaque collision entre 2 boules, il y a une sorte d'accélération.
C'est pas voulu mais j'aime bien ;) Je pense que c'est parce que la fonction se fait plusieurs fois pour une même collision.
Les boules ne s'arrêtent toujours pas...


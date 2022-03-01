
<h1 style="text-align: center"><u>Rapport séance finale (28/02/2022)</u></h1>

<h2>correction d'une erreur avec les encodeurs</h2>
<div style="text-align: justify">

Lors des tests avec le lidar j'ai remarqué que les moteurs avait tendance à tourner assez rapidement même lorsque que je demandait une vitesse basse aux encodeurs.

Après avoir vérifié le code j'ai trouvé une erreur de conversion qui faisait que les encodeurs avait tendance à pousser les moteurs aux maximum assez souvent ce qui faisait perdre en précision.

une fois les modifications faites, j'ai essayé d'améliorer le programme <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_avancé/turnAround/turnAround.ino"> turnAround</a> qui fait décrire au robot un carré en diminuant fortement la vitesse de rotation.

le résultat à été concluant avec une précision bien plus haute.

</div>

<h2>batterie pour le lidar</h2>

<div style="text-align: justify">

la nouvelle batterie suffit à peine pour alimenter le lidar, il faut souvent débrancher et rebrancher le fil pour le lancer. Mais je pense que ce sera suffisant pour faire une demonstration. La nouvelle batterie étant plus volumineuse que l'autre, je l'ai déplacé sous le châssis

</div>

<h2>simulations avancé</h2>

<div style="text-align: justify">

après plusieurs heures de simulations les résultats sont présents mais ils ne sont pas assez bon pour pouvoir être mis en place dans le robot (pas assez efficace). suite aux problèmes de triangulation, j'ai préféré entrainner le robot uniquement à éviter des obstacles en le faisant tourner dans un circuit.

Cependant il semble qu'il n'y ait quelque chose que je n'ai pas encore comprise sur les algorithme génétique car malgrés des douzaines de modifications dans l'algorithme (degrés de modification à chaque génération, structure du réseaux de neurones, taille du tableau de données reçu du lidar ...) je n'ai pas réussi à avoir un résultat efficace dans des un environment quelconque.

</div><h2>bluetooth</h2>

<div style="text-align: justify">

les commandes bluetooth ont été ajoutés à l'exception du choix de la cible puisque le robot ne pourra pas atteindre une cible en fin de compte.

</div>

<h2>programme de demo</h2>

<div style="text-align: justify">

j'ai créé le programme de demo, ce programme compile toutes les avancées du projet en un seul programme. Le robot commence par initialiser tous les capteurs. puis entre dans la boucle.

à chaque tour de boucle, le robot effectue forcément les tâches suivantes : 

    -faire un scan complet (360°) avec le lidar, si le lidar n'est pas en marche, 
    le programme boucle sur cette partie jusqu'à ce qu'il se rallume.
    -detecter les commandes bluetooth
    -calculer la nouvelle position du robot.
    -mettre à jour les moteurs (feedBack loop avec les encodeurs)

ensuite le robot effectue une des deux actions suivantes en fonction de son état : 

    -avancer tout droit en detectant s'il y a un obstacle devant lui;
    -detecter le point le plus lointain puis tourner jusqu'à être orienté dans la direction de ce point.

Au final, le robot est capable de se déplacer tout en évitant les obstacle.

</div>

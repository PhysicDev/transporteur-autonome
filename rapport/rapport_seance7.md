
<h1 style="text-align: center"><u>Rapport séance 7 (21/02/2022)</u></h1>

<h2>travail sur la triangulation</h2>
<div style="text-align: justify">

J'ai travaillé sur la manière de suivre la position du robot avec le temps.

Etant donné que le robot est censé se déplacer sur un plan en 2D. J'ai besoin de deux informations
pour connaître la position du robot: la vitesse angulaire et la vitesse tangentielle.

Pour la vitesse tangentielle, il suffit de faire la moyenne de la vitesses des roues (que l'on obtient avec les encodeurs)

Pour la vitesse angulaire, c'est plus compliqué. L'accéléromètre était censé mesurer les mouvements du robot et 
le magnétomètre devait servir à le calibrer. Mais puisque le magnétomètre est instable avec le temps j'ai du le mettre de côté.
J'ai remplacer le magnétomètre par un programme de calibrage de l'accéléromètre, qui effectue des mesures avec le véhicule à l'arrêt
Pour estimer les décalage dans les données. Ce programme stocke égalment les résultats dans la mémoire EEPROM de l'arduino pour permettre d'éviter d'avoir à les recalculer à chaque fois (pour l'instant je les recalcule tout de même pour avoir des résultat plus juste)

le code est disponible <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_avancé/calibrage_MPU6050/calibrage_MPU6050.ino"> ici</a>.

Après quelque tests, j'ai réussi à suivre l'orientation de la voiture avec une grande précision mais seulement si le robot tourne à faible
vitesse.

Cependant le magnétomètre s'avère être plus précis pour des mouvements rapide, j'ai donc fait en sorte que le programme utilise le magnétomètre au lieu de l'accéléromètre quand les vitesses sont élévé.
Le reste du temps, le programme calcule le décalage qu'accumule l'accéléromètre avec le temps pour faire des mesures correcte lorsqu'il est utilisé. 

le code est disponible <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_avancé/positionTest/positionTest.ino"> ici</a>. (sans les encodeurs car ils ça surchagerait inutilment le code).

cependant les tests en réel (avec les moteurs ont révélé un décalage important pour les angles (pour des rotations de 90° , le robot à une marge d'erreur de 30°)). Je pense que c'est du en partie à un problème de code mais l'idée d'avoir un cible précise semble être comprimise (une direction global vers laquelle aller est plus envisageable)

le code est disponible <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_avancé/turnAround/turnAround.ino"> ici</a>. (sans les encodeurs car ils ça surchagerait inutilment le code).

</div>

<h2>batterie trop faible pour le lidar</h2>

<div style="text-align: justify">

les tests du nouveau châsis ont montré que la batterie de portable est incapable d'allimenter le lidar (500 mA est trop faible). Le lidar fonctionne avec une alim de (2000 mA), il faudra donc la remplacer.


</div>

<h2>première simulations</h2>

<div style="text-align: justify">

le programme de simulations et fonctionel et j'ai commencer à entrainer les réseaux de neurones, le programme réussi à faire converger les voiture vers la cible mais elle semble assez inefficace pour ce qui est d'éviter les obstacles.

Je pense qu'il faut entrainner les voitures dans un environment type circuit pour qu'elles apprennent à éviter les obstacles puis entrainner un autre réseaux de neuronnes avec les coordonées de la cible .

J'ai aussi décidé de changer les sorties du réseaux de neurones, au lieu de prendre la vitesse des deux moteurs, trop vague, l'IA aura le choix entre plusieurs action basique: avancer, reculer, tourner à gauche etc ... Un programme changera ensuite la vitesse des moteurs en fonction du choix dont le neurone se sera le plus activé.

(j'ai mis en ligne la nouvelle version du simulateur mais il utilise une version modifié de ma librairie NeuralNet, c'est pour ça que je l'ajoute aussi au code même si elle existe déjà dans un autre repository)

voici une <a href="https://www.youtube.com/watch?v=2j0NKs4VKWg">courte demonstration</a> des capacité actuelle des réseaux de neurones.

</div><h2>bluetooth</h2>

<div style="text-align: justify">

Le dernier composant manquant au projet est le capteur bluetooth, il permettra de commander le robot par téléphone. Les commandes que j'ai prévu de mettre en place sont les suivantes : 

    start : pour mettre en marche le robot;
    stop : pour arrêter les moteurs;
    calibrage : stoppe les moteurs et lance un calibrage de l'accéléromètre;
    target : pour choisir la cible du robot;
    reset : relance le programme (en cas de problème).

</div>

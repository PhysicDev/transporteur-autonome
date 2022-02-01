
<h1 style="text-align: center"><u>Rapport séance 5 (31/01/2022)</u></h1>

<h2>Problème avec le LIDAR</h2>
<div style="text-align: justify">

Le lidar a posé beaucoup de problèmes. J'ai tout d'abord cherché ce qui ne marchait pas dans le code d'exemple de la librairie. J'ai compris que l'erreur venait du fait que les requêtes du lidar étaient TIMEDOUT.

 J'ai donc verifié que j'avais bien configuré la sortie Serial. Mais il était bien configuré, j'ai donc changé de port Serial dans le cas où celui que j'utilisait était cassé sans succés.
 
  J'ai ensuite créé un <a href="">code de test</a> pour verifier que les ports Serial fonctionnaient correctement(ils fonctionnaient correctement). J'ai ensuite verifé que les fils n'était pas cassé. Vu qu'ils fonctionnaient tous j'ai créé un autre code de test dans lequel j'ai essayé d'envoyer directement les paquets du protocole utilisé par le lidar octet par octet sans passer par la librairie.

  après tous ces essais je n'avait plus aucune idée de quoi faire car tout les composants marchait de leur côté (Serial, fils, Lidar, arduino ...) mais ne fonctionnaient pas tous esemble.

  j'ai fait un dernier essai durant la séance: j'ai branché la puce censé piloter le lidar lorque je le relie à l'ordinateur mais au lieu de brancher le lidar à la puce je l'ai relier à l'arduino pour voir ce que la puce envoyait au lidar pour le faire marcher. J'ai remarqué que la puce envoyait une requête que je n'avait jamais vu dans la documentation du Lidar. J'ai donc essayé d'envoyer cette requête dans mon code arduino pour voir si ça resoudrait le problème. La requête n'a pas eu l'air d'avoir de l'effet mais ensuite lorque j'ai réesayyer le code de départ ,le lidar se mettait à fonctionner correctement.

  Je ne suis pas sûr que la requête soit la cause de la résolution du problème et je ne sais pas si le problème et réelement résolu.

  en tout cas le lidar est maintenant fonctionel et je peux récuperer ses données sur l'arduino.

Tous les capteurs sont maintenant configurés.

note : après recherche je me suis rendu compte que le lidar manquait de puissance car j'utilisait l'alim exterieure uniquement pour le moteur ce qui ne laissait pas asser de puissance au composant du lidar (sur tous les sites que j'ai visité ils indiquent la connection V5.0 doit être branché sur l'arduino mais je suppose qu'avec tous les composant déjà branché la puissance était tout de même trop basse).

les codes de tests sont disponibles <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_basiques/lidar_working/lidar_working.ino"> ici</a>.

</div>

<h2>Problème de Calibrage du magnétomètre</h2>

<div style="text-align: justify">

J'ai remarqué un réultat étrange de la mesure du magnétomètre alors que j'atait en train de compiler l'ensemble des codes de tests des capteurs du projet en un seul code.

Les valeurs du magnétomètre (notament la valeur yaw axis) avaient tendance à dériver avec le temps. (à raison d'un degré par minute dans le pire des cas) ce qui est intolérable pour l'orientation du robot. Après plusieurs essai j'ai noté que cette dérive n'était pas tout le temps présente .


</div>

<h2>compilation des codes de tests des capteurs</h2>

<div style="text-align: justify">

j'ai rassemblé tout les codes que j'ai fait jusqu'à maintenant en un seul fichier, avec des fonctions pour chaque capteur afin de pouvoir facilment exploiter leur donné à l'avenir

</br>
</div>

<h2>compilation des codes de tests des capteurs</h2>

<div style="text-align: justify">

j'ai rassemblé tout les codes que j'ai fait jusqu'à maintenant en un seul fichier, avec des fonctions pour chaque capteur afin de pouvoir facilment exploiter leurs données à l'avenir. Il se trouve <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_avancé/allSensors/allSensors.ino"> ici</a>.

</div><h2>programmation du logiciel de simulation</h2>

<div style="text-align: justify">

j'ai commencé à programmer mon logiciel de simulation pour entrainer le réseaux de neurones de la voiture. Pour l'instant j'ai programmé des classes pour les obstacles et pour les voitures.

Le code est programmé en java avec Processing pour simplifier la construction de l'interface graphique.

j'ai d'abord essayé d'utiliser un système de coordonées cartésiennes pour définir le mouvement de la voiture mais ça s'est avéré difficile et j'ai choisi un système de coordonée dans la base de frenet (bien plus pratique pour calculer la trajéctoire d'une voiture).

Les voitures sont controlable bien que la plupart des valeurs (distance des roues, accélération et vitesse maximale ...) ne soit pas corectes par rapport à la réalité.

Je n'ai pas encore ajouté le réseaux de neurones et le raycasting (pour simuler le lidar) n'a pas encore été ajouté.

J'ai prévu d'utiliser ma propre librairie Java de réseaux de neurones que vous pouvez trouver dans un autre git <a href="https://github.com/netscape-swega/AI/tree/master/NeuralNetwork/JAVA"> ici</a>.

pour l'instant le programme place des obstacles aléatoirement et place plusieurs voiture au point de départ.

la vitesse des roues des voitures varie aléatoirement avec le temps
<img src="../../../tree/main/rapport/images/screen1.png" alt = "état actuel du logiciel">
<img src="../../../tree/main/rapport/images/screen2.png" alt = "état actuel du logiciel">

note : les traits noit sur les voitures symbolise la vitesse; les traits rouges indiques l'accélération;


le code peut être trouvé <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Simulateur\TA_simlulator"> ici</a>.
</br>
</div>
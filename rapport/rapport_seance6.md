
<h1 style="text-align: center"><u>Rapport séance 6 (07/02/2022)</u></h1>

<h2>Reconfiguration du châsis et ajout des alims</h2>
<div style="text-align: justify">

maintenant que tous les composants fonctionnent j'ai commencé à les placer correctement dans le châsis.
j'ai déplacé la carte arduino sur la plaque inferieure avec la breadboard. Le Lidar et les capteurs (accéléromètre et magnétomètre) seront sur la plaque superieure.

j'ai ensuite récuperé les alims pour le robot. Pour l'alimentation de l'arduino (et du lidar) j'ai utilisé une alim de 5V fournie par une batterie portable pour téléphone. je l'ai placé à l'arrière du robot.

Pour les moteurs on m'a donné une alim de 12V avec une rallonge pour permettre au robot de pouvoir se déplacer même s'il reste relié au secteur.


<img src="../../../tree/main/rapport/images/photo2.jpg" alt = "nouvelle configuration">
<img src="../../../tree/main/rapport/images/photo3.jpg" alt = "nouvelle configuration">

</div>

<h2>tests avancé avec les encodeurs</h2>

<div style="text-align: justify">

J'ai créé un programme pour tester la capacité à réguler la vitesse des moteurs avec les encodeurs.

Le programme fait en sorte que les moteurs se déplace à la même vitesse à l'aide des données des encodeurs et d'une feedback loop rudimentaire (à améliorer si ça s'avère insufisant).

<a href="https://youtu.be/a8dLJt0JgNo">demonstration vidéo</a>

le code est disponible <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_avancé/ligneDroite/ligneDroite.ino"> ici</a>.

</div>

<h2>avancement du côté du logiciel de simulation</h2>

<div style="text-align: justify">

j'ai ajouté le simulateur de lidar (ray casting) au programme, il ne reste plus qu'à relier le tout à un réseaux de neurones et le logiciel sera prêt pour les premières simulations.

Les rayons sont présent dans le screenshot pour montrer qu'ils fonctionnent. Bien sur il ne seront pas affichés lors des simulations pour éviter des problèmes de performance.

<img src="../../../tree/main/rapport/images/screen2.png" alt = "rendu raycasting">
<img src="../../../tree/main/rapport/images/screen3.png" alt = "rendu raycasting">

</br>
</div>

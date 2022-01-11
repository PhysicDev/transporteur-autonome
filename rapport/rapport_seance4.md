
<h1 style="text-align: center"><u>Rapport séance 4 (10/01/2022)</u></h1>

<h2>Problème d'adresse I2C (résolu)</h2>
<div style="text-align: justify">

Après quelques tests, je me suis rendu compte que le code de l'accéléromètre marchait, mais uniquement lorsque le magnétomètre était débranché. (et inversement)</br>
J'ai donc fait plusieurs recherches et j'ai fini par trouver <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/autre/scanner/scanner.ino"> ce code</a> qui permet de scaner les pins I2C et d'avoir toutes les adresses utilisée. Grâce à ce programme, j'ai pu découvrir que le magnétomère utilisait 4 adresses I2C dont l'adresse <strong>0x68</strong> qui est l'adresse utilisée par l'accéléromètre. J'ai tout d'abord cru que les deux modules était incompatible et qu'il faudrait utiliser un multiplexeur.</br>
Mais après m'être documenté sur les pins de l'accéléromètre, j'ai vu que l'on pouvait utiliser le pin AD0 pour changer l'adresse de l'accéléromètre de <strong>0x68</strong> à <strong>0x69</strong> qui n'est pas utilisée par le magnétomètre.

le seul capteur qu'il reste à configurer est donc le Lidar

le code de test est disponible <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_basiques/acceleroTest/acceleroTest.ino"> ici</a>.

</div>

<h2>Avancement du côté du lidar</h2>

<div style="text-align: justify">

après plusieurs essai infructueux j'ai finis par me résoudre à utiliser une alimentation externe de 5V pour alimenter le moteur du lidar, après cet ajou le moteur du lidar s'est mis à fonctionner correctement bien qu'il me semble plus lent que lorqu'il est branché directement à l'ordinateur.

Cependant le code d'exemple ne fonctionait toujours pas.

J'ai fait plusieurs modification du code d'exemple qui sont les suivantes :
 <ul>
    <li>retirer la partie concernant l'anneaux de led (qui ne me sert à rien)</li>
    <li>changer le Serial (Serial 1 à la place de Serial 0) pour pouvoir utiliser le Serial normalment</li></ul>

après ces modifications j'ai pu en savoir plus sur les erreurs. Il semble que l'aruino ne detecte pas le lidar à l'exception d'un test où, le lidar à sembler réussir à se connecter sans que j'ai modifé le code mais je n'ai pas réussi a réitérer l'experience.

le code de test est disponible <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_basiques/lidar_test/lidar_test.ino"> ici</a>.
</br>
</div>
j'ai aussi commencé à faire un code qui récupère toutes les données des capteurs (il est encore assez brouillon). Il se trouve <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_avancé/allSensors/allSensors.ino"> ici</a>.
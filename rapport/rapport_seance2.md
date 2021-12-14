
<h1 style="text-align: center"><u>Rapport séance 2 (13/12/2021)</u></h1>

##Changement de châssis : 
<div style="text-align: justify">
Après avoir remplacé les vis abimé le châssis s’est avéré trop usé et je suis passé sur un nouveau châssis fraîchement découpé.
J’ai visé les différentes parties du châssis ainsi que les roues, et le module du moteur.

J’ai posé deux vis provisoires pour tenir la carte Arduino sur le châssis pendant les tests

Je n’ai pas encore défini le sens du robot je verrais ce qui est le plus avantageux entre roues motrices arrière ou roues motrices avant.

J’ai ensuite branché les fils du moteur à l’Arduino et procédé aux premiers tests du moteurs. Le code d’essai vérifie que les moteurs sont branchés correctement en enclenchant marche avant puis marche arrière. Je n’ai pas encore raccordé les encodeurs. (Le code d’essai est disponible sur la page github dans le dossier code/tests_basiques/wheel_control ).
https://github.com/PhysicDev/transporteur-autonome/tree/main/code/Tests_basiques

Le lidar devra être seul sur la plateforme supérieure du robot ou bien sur une plateforme surélevée pour éviter qu’il ne détecte les éléments du robot.

J’ai aussi remarqué que la connexion avec l’Arduino méga posait quelque problème pour le téléversement du code, il y a une erreur qui survient assez souvent 
(Message d’erreur : avrdude: stk500v2_getsync(): timeout communicating with programmer ). Il s’avère que mon ordinateur ne la détecte forcément quand je la branche. Cependant j’arrive tout de même à envoyer le code après 2 ou 3 essais. 

![Etat actuel du robot.](/images/photo1.JPG)
</div>

##Tests avec le lidar :

<div style="text-align: justify">
Pendant que le châssis était en train d’être découpé j’ai continué à travailler sur le lidar

J’ai installé une librairie pour Arduino appelée RPLidarDriver : 
https://github.com/robopeak/rplidar_arduino

J’ai essayé d’envoyer les données du lidar vers l’Arduino, cependant je pensais que l’alimentation de 5V de l’Arduino suffirait pour le lidar mais il semble qu’il ait besoin d’une alimentation extérieure de 5V.

Le programme de test à tout de même renvoyé une série de caractère semblant aléatoire (je pense qu’il s’agit de données qui ne sont pas censé être en code ascii). Je ne suis pas allé plus loin car le châssis était terminé.

</div>

##Réception des capteurs :

<div style="text-align: justify">
J’ai récupéré les capteurs suivants pour le robot :

L’accéléromètre et le magnétomètre. 

Je ne les ai pas encore testés.
Avec le Lidar et les encodeurs le projet à en tout 4 capteurs différents pour se repérer.

</div>

##Prochaine Séance :

<div style="text-align: justify">
Je raccorderais les encodeurs pendant les vacances de noël et je testerais peut-être les capteurs. Durant la prochaine séance je continuerais les tests de déplacement pour la voiture et je travaillerais sur l’analyse de son environnement et de sa trajectoire (récupération des données avec les différents capteur)
</div>


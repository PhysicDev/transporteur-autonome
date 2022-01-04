
<h1 style="text-align: center"><u>Rapport séance 3 (03/01/2022)</u></h1>

<h2>travail pendant les vacances</h2>
<div style="text-align: justify">

j'ai amélioré le programme de test des roues pendant les vacances en récupérant les données des encodeurs. Le programme affiche dans la console le nombre de passage qui est converti en mètre.

j'ai utilisé ce <a href="https://automaticaddison.com/calculate-pulses-per-revolution-for-a-dc-motor-with-encoder/"> site </a> pour savoir comment brancher et programmer les encodeurs

le code de test est disponible <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_basiques/wheel_control/wheel_control.ino"> ici</a>.

</div>

<h2>Prise en main du magnétomètre :</h2>

<div style="text-align: justify">
j'ai récupéré le code de test trouvé sur le <a href="https://wiki.dfrobot.com/10_DOF_Mems_IMU_Sensor_V2.0_SKU__SEN0140"> tutoriel</a> de DFrobot pour obtenir les données du magnétomètre. Le programme fonctionnait mais ne renvoyait que des valeurs nulles. J'ai résolu le problème en changeant les pins où était branché le magnétomètre car les pin de connection SDA et SCL sont à des positions différentes sur l'arduino mega (pin <b>20 21</b> au lieu de <b>A4 et A5</b>).</br> J'ai égalment retiré la partie du code récupérant des données pour la température et la préssion puisque ça ne m'était pas utile et j'ai ajouté quelques lignes pour obtenir l'orientation du robot par rapport à son orientation de départ</br>
le code de test est disponible <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_basiques/magnetorTest/magnetorTest.ino"> ici</a>.
</br>
</br></div>

<h2>problème avec l'accéléromètre :</h2>
<div style="text-align: justify">
j'ai essayé de récupérer les données de l'accéléromètre à l'aide de ce <a href="https://www.aranacorp.com/fr/utilisation-dun-module-mpu6050-avec-arduino/"> tutoriel</a> mais le programme avait été crée à la base pour l'arduino uno et il présente une erreur pour l'arduino mega, je n'ai pas eu le temps de la corriger.
 </br>le code de test est disponible <a href="https://github.com/PhysicDev/transporteur-autonome/blob/main/code/Tests_basiques/acceleroTest/acceleroTest.ino"> ici</a>.
</br>
</br></div>

<h2>mise au point de l'algorithme </h2>
<div style="text-align: justify">
j'ai immaginé un algorithme qui permettrais au robot de se déplacer jusqu'à une cible en prenant en compte les obstacles, il consiste en un réseaux de neurones entrainé sur ordinateur, l'arduino ne devra qu'exécuter un réseaux de neurones déjà entrainé.</br></br> les données de position et d'orientation seront donné relativement à la position cible de manière à ce que l'objectif du robot soit toujours d'atteindre les coordonées 0;0</br></br> le réseaux de neurone retournera la vitesses désirée des deux roues et les encodeurs permetrons de calculer la puissance qu'il faut donner aux moteurs.
<img src="../../../tree/main/schema/algorithme.png" alt = "schéma de l'algorithme.">
</div>
</br></br>
<div>
<ul>librairies installés durant la séance : 
    <li>FreeSixIMU</li>
    <li>HMC5883</li>
    <li>BMP280</li>
</ul></div>
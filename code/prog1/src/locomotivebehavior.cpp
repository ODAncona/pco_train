//    ___  _________    ___  ___  ___   __ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  | / / //
//  / ___/ /__/ /_/ / / __// // / __/ / /  //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include "locomotivebehavior.h"
#include "ctrain_handler.h"

void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    /* A vous de jouer ! */


    // Vous pouvez appeler les méthodes de la section partagée comme ceci :
    //sharedSection->request(loco);
    //sharedSection->getAccess(loco);
    //sharedSection->leave(loco);

    int turn = 0;

    while(1) {
        turn < 2 ? attendre_contact(entree) : attendre_contact(sortie);
        sharedSection->access(loco);
        diriger_aiguillage(16, sens, 0);
        diriger_aiguillage(11, sens, 0);
        turn < 2 ? attendre_contact(sortie) : attendre_contact(entree);
        sharedSection->leave(loco);
        if (turn % 2) {
            // changement de sens
            attendre_contact(depart);
            loco.inverserSens();
        }
        turn = (turn + 1) % 4;
    }
}

void LocomotiveBehavior::printStartMessage()
{
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

void LocomotiveBehavior::printCompletionMessage()
{
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}

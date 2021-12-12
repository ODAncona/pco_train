//    ___  _________    ___  ___  ___   __ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  | / / //
//  / ___/ /__/ /_/ / / __// // / __/ / /  //
// /_/   \___/\____/ /____/\___/____//_/   //
//                                         //


#include "locomotivebehavior.h"
#include "ctrain_handler.h"

#define LOCO_EXTERNE 42

void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    int turn = 0;
    SharedSectionInterface::LocoId locoId;
    SharedSectionInterface::EntryPoint entryPoint = SharedSectionInterface::EntryPoint::EA;

    loco.numero() == LOCO_EXTERNE ? locoId = SharedSectionInterface::LocoId::LB : locoId = SharedSectionInterface::LocoId::LA;
    /*
    while(1) {
        turn < 2 ? attendre_contact(requeteA) : attendre_contact(requeteB);
        sharedSection->request(loco, locoId, entryPoint);
        loco.afficherMessage("J'attends l'entrée de la section critique");
        turn < 2 ? attendre_contact(entreeA) : attendre_contact(entreeB);
        loco.afficherMessage("J'essaie d'entrer en section critique");
        sharedSection->getAccess(loco, locoId);
        loco.afficherMessage("J'entre en section critique");
        diriger_aiguillage(16, sens, 0);
        diriger_aiguillage(11, sens, 0);
        loco.afficherMessage("J'attends la sortie de la section critique");
        turn < 2 ? attendre_contact(entreeB) : attendre_contact(entreeA);
        loco.afficherMessage("Je sors de la section critique");
        sharedSection->leave(loco, locoId);
        if (turn % 2) {
            // changement de sens
            loco.afficherMessage("J'attends sur un contact pour changer de sens");
            turn < 2 ? attendre_contact(entreeA) : attendre_contact(entreeB);
            loco.afficherMessage("Je change de sens");
            loco.inverserSens();
            entryPoint == SharedSectionInterface::EntryPoint::EA ? entryPoint = SharedSectionInterface::EntryPoint::EB : entryPoint = SharedSectionInterface::EntryPoint::EA;
        }
        turn = (turn + 1) % 4;
    }*/

    while(1) {
        if(turn < 2) {
            /* Sens 1 */

            // Requête
            attendre_contact(requeteA);
            sharedSection->request(loco, locoId, entryPoint);
            loco.afficherMessage("J'attends l'entrée de la section critique");

            // Entrée en section partagée
            attendre_contact(entreeA);
            loco.afficherMessage("J'essaie d'entrer en section critique");
            sharedSection->getAccess(loco, locoId);
            loco.afficherMessage("J'entre en section critique");
            diriger_aiguillage(16, sens, 0);
            diriger_aiguillage(11, sens, 0);

            // Sortie de la section partagée
            loco.afficherMessage("J'attends la sortie de la section critique");
            attendre_contact(entreeB);
            loco.afficherMessage("Je sors de la section critique");
            sharedSection->leave(loco, locoId);
        }else {
            /* Sens 2 */

            // Requête
            attendre_contact(requeteB);
            sharedSection->request(loco, locoId, entryPoint);
            loco.afficherMessage("J'attends l'entrée de la section critique");

            // Entrée en section partagée
            attendre_contact(entreeB);
            loco.afficherMessage("J'essaie d'entrer en section critique");
            sharedSection->getAccess(loco, locoId);
            loco.afficherMessage("J'entre en section critique");
            diriger_aiguillage(16, sens, 0);
            diriger_aiguillage(11, sens, 0);

            // Sortie de la section partagée
            loco.afficherMessage("J'attends la sortie de la section critique");
            attendre_contact(entreeA);
            loco.afficherMessage("Je sors de la section critique");
            sharedSection->leave(loco, locoId);
        }
        if (turn % 2) {
            // Changement de direction
            loco.afficherMessage("J'attends sur un contact pour changer de sens");
            turn < 2 ? attendre_contact(entreeA) : attendre_contact(entreeB);
            loco.afficherMessage("Je change de sens");
            loco.inverserSens();
            entryPoint == SharedSectionInterface::EntryPoint::EA ? entryPoint = SharedSectionInterface::EntryPoint::EB : entryPoint = SharedSectionInterface::EntryPoint::EA;
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

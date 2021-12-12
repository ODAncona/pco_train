//    ___  _________    ___  ___  ___   __ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  | / / //
//  / ___/ /__/ /_/ / / __// // / __/ / /  //
// /_/   \___/\____/ /____/\___/____//_/   //
//                                         //


#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <QDebug>

#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "sharedsectioninterface.h"

/**
 * @brief La classe SharedSection implémente l'interface SharedSectionInterface qui
 * propose les méthodes liées à la section partagée.
 */
class SharedSection final : public SharedSectionInterface
{
public:

    /**
     * @brief SharedSection Constructeur de la classe qui représente la section partagée.
     * Initialisez vos éventuels attributs ici, sémaphores etc.
     */
    SharedSection(): occupied(false), nbBlocked(0), blocking(0), mutex(1), nbRequests(0) {
        // TODO
    }

    /**
     * @brief request Méthode a appeler pour indiquer que la locomotive désire accéder à la
     * section partagée (deux contacts avant la section partagée).
     * @param loco La locomotive qui désire accéder
     * @param locoId L'identidiant de la locomotive qui fait l'appel
     * @param entryPoint Le point d'entree de la locomotive qui fait l'appel
     */
    void request(Locomotive& loco, LocoId locoId, EntryPoint entryPoint) override {
        // TODO
        mutex.acquire();
        locoId == SharedSectionInterface::LocoId::LA ? entryPointLA = entryPoint : entryPointLB = entryPoint;
        ++nbRequests;
        mutex.release();
    }

    /**
     * @brief getAccess Méthode à appeler pour accéder à la section partagée, doit arrêter la
     * locomotive et mettre son thread en attente si la section est occupée ou va être occupée
     * par une locomotive de plus haute priorité. Si la locomotive et son thread ont été mis en
     * attente, le thread doit être reveillé lorsque la section partagée est à nouveau libre et
     * la locomotive redémarée. (méthode à appeler un contact avant la section partagée).
     * @param loco La locomotive qui essaie accéder à la section partagée
     * @param locoId L'identidiant de la locomotive qui fait l'appel
     */
    void getAccess(Locomotive &loco, LocoId locoId) override {
        // TODO
        mutex.acquire();
        bool priority = true;
        if (nbRequests == 2) {
            if (locoId == SharedSectionInterface::LocoId::LA)
                priority = entryPointLA == entryPointLB;
            else
                priority = entryPointLA != entryPointLB;
        }
        --nbRequests;
        if (occupied || !priority) {
            //Loco doit attendre
            ++nbBlocked;
            mutex.release();
            loco.arreter();
            blocking.acquire();

            //Loco redémarre
            loco.demarrer();
        }else{
            //Loco peut se lancer
            occupied = true;
            mutex.release();
        }
        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString("The engine no. %1 accesses the shared section.").arg(loco.numero())));
    }

    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section
     * partagée. (reveille les threads des locomotives potentiellement en attente).
     * @param loco La locomotive qui quitte la section partagée
     * @param locoId L'identidiant de la locomotive qui fait l'appel
     */
    void leave(Locomotive& loco, LocoId locoId) override {
        // TODO
        mutex.acquire();
        if(nbBlocked) {
            --nbBlocked;
            blocking.release();
        }else {
            occupied = false;
        }
        mutex.release();
        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString("The engine no. %1 leaves the shared section.").arg(loco.numero())));
    }

    /* A vous d'ajouter ce qu'il vous faut */

private:
    // Méthodes privées ...
    // Attributes privés ...
    bool occupied;
    unsigned nbBlocked;
    PcoSemaphore blocking;
    PcoSemaphore mutex;
    unsigned nbRequests;
    EntryPoint entryPointLA;
    EntryPoint entryPointLB;
};


#endif // SHAREDSECTION_H

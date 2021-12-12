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
     */
    SharedSection(): occupied(false), nbBlocked(0), blocking(0), mutex(1), nbRequests(0) {
    }

    /**
     * @brief request Méthode a appeler pour indiquer que la locomotive désire accéder à la
     * section partagée (deux contacts avant la section partagée).
     * @param loco La locomotive qui désire accéder
     * @param locoId L'identidiant de la locomotive qui fait l'appel
     * @param entryPoint Le point d'entree de la locomotive qui fait l'appel
     */
    void request(Locomotive& loco, LocoId locoId, EntryPoint entryPoint) override {
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
        mutex.acquire();
        bool priority = true;
        //Dans le cas ou les deux locomotives font une requête d'entrée en section critique en même temps,
        //on regarde laquelle à la priorité en fonction de leur point d'entrée
        if (nbRequests == 2) {
            if (locoId == SharedSectionInterface::LocoId::LA)
                priority = entryPointLA == entryPointLB;
            else
                priority = entryPointLA != entryPointLB;
        }
        --nbRequests;
        //On regarde si la section critique est occupée ou si une locomotive de plus haute priorité veut y rentrer
        if (occupied || !priority) {
            //La locomotive doit attendre la libération de la section critique pour continuer
            ++nbBlocked;
            mutex.release();
            loco.arreter();
            blocking.acquire();

            //La locomotive peut redémarrer
            loco.demarrer();
        } else {
            //La locomotive peut entrer en section critique sans avoir besoin de s'arrêter
            occupied = true;
            mutex.release();
        }
    }

    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section
     * partagée. (reveille les threads des locomotives potentiellement en attente).
     * @param loco La locomotive qui quitte la section partagée
     * @param locoId L'identidiant de la locomotive qui fait l'appel
     */
    void leave(Locomotive& loco, LocoId locoId) override {
        mutex.acquire();
        //On regarde s'il y a des locomotives en file d'attente pour rentrer en section critique
        if(nbBlocked) {
            //On permet à la première locomotive de la file d'attente de rentrer en section critique
            --nbBlocked;
            blocking.release();
        }else {
            //On indique que la section critique est libre et disponible pour une prochaine locomotive
            occupied = false;
        }
        mutex.release();
        afficher_message(qPrintable(QString("The engine no. %1 leaves the shared section.").arg(loco.numero())));
    }

private:

    bool occupied; //Indique l'occupation ou non de la section critique par une locomotive
    unsigned nbBlocked; //Nombre de locomotives bloquées à l'entrée de la section critique
    PcoSemaphore blocking; //Permet de bloquer une locomotive à l'entrée de la section critique tant que celle-ci est occupée
    PcoSemaphore mutex; //Protège les variables partagées par plusieurs fonctions (occupied, nbBlocked, nbRequests, entryPointLA et entryPointLB)
    unsigned nbRequests; //Nombre de requêtes en cours pour entrer en section critique
    EntryPoint entryPointLA; //Point d'entrée de la locomotive A en section critique
    EntryPoint entryPointLB; //Point d'entrée de la locomotive B en section critique
};


#endif // SHAREDSECTION_H

//    ___  _________    ___  ___  ___   __ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  | / / //
//  / ___/ /__/ /_/ / / __// // / __/ / /  //
// /_/   \___/\____/ /____/\___/____//_/   //
//                                         //

#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include "locomotive.h"
#include "launchable.h"
#include "sharedsectioninterface.h"

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive
 */
class LocomotiveBehavior : public Launchable
{
public:
    /**
     * @brief LocomotiveBehavior Constructeur de la classe
     * @param loco Locomotive dont on représente le comportement
     * @param sharedSection Section partagée par les deux locomotives
     * @param direction Sens dans lequel se déplace la locomotive
     * @param entryA Premier point d'entrée/sortie de la section critique
     * @param entryB Second point d'entrée/sortie de la section critique
     * @param requestA Point de requête pour une entrée en section critique via entryA
     * @param requestB Point de requête pour une entrée en section critique via entryB
     */
    LocomotiveBehavior(Locomotive& loco, std::shared_ptr<SharedSectionInterface> sharedSection, int direction, int entryA, int entryB, int requestA, int requestB) :
        loco(loco), sharedSection(sharedSection), direction(direction), entryA(entryA), entryB(entryB), requestA(requestA), requestB(requestB) {
    }

protected:
    /*!
     * \brief run Fonction lancée par le thread, représente le comportement de la locomotive
     */
    void run() override;

    /*!
     * \brief printStartMessage Message affiché lors du démarrage du thread
     */
    void printStartMessage() override;

    /*!
     * \brief printCompletionMessage Message affiché lorsque le thread a terminé
     */
    void printCompletionMessage() override;

    /**
     * @brief loco La locomotive dont on représente le comportement
     */
    Locomotive& loco;

    /**
     * @brief sharedSection Pointeur sur la section partagée
     */
    std::shared_ptr<SharedSectionInterface> sharedSection;

    /*
     * Vous êtes libres d'ajouter des méthodes ou attributs
     *
     * Par exemple la priorité ou le parcours
     */
private:
    int direction; //sens de la locomotive
    int entryA, entryB; //extrêmités de la section critique
    int requestA, requestB; //Points de requête pour une entrée en section critique
};

#endif // LOCOMOTIVEBEHAVIOR_H

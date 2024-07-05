/**
 * \file Graphe.cpp
 * \brief Implémentation d'un graphe orienté.
 * \author IFT-2008 & ?
 * \version 0.1
 * \date Juin-Juillet 2024
 *
 *  Travail pratique numéro 2
 *
 */

#include "Graphe.h"
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2 {

    /**
     * \brief Constructeur de la classe Graphe
     * \param nbSommets Nombre de sommets dans le graphe
     */
    Graphe::Graphe(size_t nbSommets) : nbSommets(nbSommets), nbArcs(0) {
        noms.resize(nbSommets);
        listesAdj.resize(nbSommets);
    }

    /**
     * \brief Destructeur de la classe Graphe
     */
    Graphe::~Graphe() {
    }

    /**
     * \brief Redimensionne le graphe
     * \param nouvelleTaille Nouvelle taille du graphe
     */
    void Graphe::resize(size_t nouvelleTaille) {
        nbSommets = nouvelleTaille;
        noms.resize(nouvelleTaille, "");  // Resize and initialize with empty strings
        listesAdj.resize(nouvelleTaille);
    }

    /**
     * \brief Nomme un sommet
     * \param sommet Numéro du sommet
     * \param nom Nom du sommet
     * \throws std::logic_error Si le numéro de sommet est invalide
     */
    void Graphe::nommer(size_t sommet, const std::string &nom) {
        if (sommet >= nbSommets) throw std::logic_error("Méthode nommer: numéro de sommet invalide");
        noms[sommet] = nom;
    }

    /**
     * \brief Ajoute un arc entre deux sommets
     * \param source Numéro du sommet source
     * \param destination Numéro du sommet destination
     * \param duree Durée de l'arc
     * \param cout Coût de l'arc
     * \throws std::logic_error Si le sommet source ou destination n'existe pas, ou si l'arc existe déjà
     */
    void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout) {
        if (!sommetExiste(source)) throw std::logic_error("Méthode ajouterArc: sommet source inexistant.");
        if (!sommetExiste(destination)) throw std::logic_error("Méthode ajouterArc: sommet destination inexistant.");
        if (arcExistePourNumero(source, destination)) throw std::logic_error("Méthode ajouterArc: Arc entre source et destination deja existant.");
        Ponderations poids(duree, cout);
        auto& liste = listesAdj.at(source);
        liste.emplace_back(destination, poids);
        ++nbArcs;
        assert(invariant());
    }

    /**
     * \brief Enlève un arc entre deux sommets
     * \param source Numéro du sommet source
     * \param destination Numéro du sommet destination
     * \throws std::logic_error Si le sommet source ou destination n'existe pas, ou si l'arc n'existe pas
     */
    void Graphe::enleverArc(size_t source, size_t destination) {
        if (!sommetExiste(source)) throw std::logic_error("Méthode enleverArc: sommet source inexistant.");
        if (!sommetExiste(destination)) throw std::logic_error("Méthode enleverArc: sommet destination inexistant.");
        if (!arcExiste(source, destination)) throw std::logic_error("Méthode enleverArc: Arc a supprime inexistant.");
        for (auto it = listesAdj.at(source).begin(), end = listesAdj.at(source).end(); it != end; ++it) {
            if (it->destination == destination) {
                listesAdj.at(source).erase(it);
                --nbArcs;
                assert(invariant());
                return;
            }
        }
    }

    /**
     * \brief Vérifie si un arc existe entre deux sommets
     * \param source Numéro du sommet source
     * \param destination Numéro du sommet destination
     * \return true si l'arc existe, false sinon
     * \throws std::logic_error Si le sommet source ou destination est invalide ou inexistant
     */
    bool Graphe::arcExiste(size_t source, size_t destination) const {
        if (source >= nbSommets) throw std::logic_error("Méthode arcExiste: numéro de sommet source invalide");
        if (destination >= nbSommets) throw std::logic_error("Méthode arcExiste: numéro de sommet destination invalide");
        if (!sommetExiste(source)) throw std::logic_error("Méthode arcExiste: sommet source inexistant");
        if (!sommetExiste(destination)) throw std::logic_error("Méthode arcExiste: sommet destination inexistant");

        const std::list<Arc> &liste = listesAdj[source];
        auto it = std::find_if(liste.begin(), liste.end(), [destination](const Arc &arc) {
            return arc.destination == destination;
        });

        return it != liste.end();
    }

    /**
     * \brief Liste les sommets adjacents à un sommet donné
     * \param sommet Numéro du sommet
     * \return Vecteur des numéros des sommets adjacents
     * \throws std::logic_error Si le numéro de sommet est invalide
     */
    std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const {
        if (sommet >= nbSommets) throw std::logic_error("Méthode listerSommetAdjacent: numéro de sommet invalide.");
        std::vector<size_t> v;
        for (auto it = listesAdj.at(sommet).begin(); it != listesAdj.at(sommet).end(); ++it) {
            v.push_back(it->destination);
        }
        return v;
    }

    /**
     * \brief Obtient le nom d'un sommet
     * \param sommet Numéro du sommet
     * \return Nom du sommet
     * \throws std::logic_error Si le numéro de sommet est invalide
     */
    std::string Graphe::getNomSommet(size_t sommet) const {
        if (sommet >= nbSommets) throw std::logic_error("reqNomSommet: numéro de sommet invalide.");
        return noms[sommet];
    }

    /**
     * \brief Obtient le numéro d'un sommet à partir de son nom
     * \param nom Nom du sommet
     * \return Numéro du sommet
     * \throws std::logic_error Si le nom du sommet est introuvable
     */
    size_t Graphe::getNumeroSommet(const std::string &nom) const {
        for (size_t i = 0; i < noms.size(); ++i) {
            if (nom == noms.at(i)) return i;
        }
        throw std::logic_error("Méthode getNumeroSommet: Nom du sommet introuvable.");
    }

    /**
     * \brief Obtient le nombre de sommets du graphe
     * \return Nombre de sommets
     */
    int Graphe::getNombreSommets() const {
        return nbSommets;
    }

    /**
     * \brief Obtient le nombre d'arcs du graphe
     * \return Nombre d'arcs
     */
    int Graphe::getNombreArcs() const {
        return nbArcs;
    }

    /**
     * \brief Obtient les pondérations d'un arc
     * \param source Numéro du sommet source
     * \param destination Numéro du sommet destination
     * \return Pondérations de l'arc
     * \throws std::logic_error Si le sommet source ou destination est invalide ou inexistant, ou si l'arc n'existe pas
     */
    Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const {
        if (source >= nbSommets) throw std::logic_error("Méthode getPonderationsArc: numéro de sommet source invalide.");
        if (destination >= nbSommets) throw std::logic_error("Méthode getPonderationsArc: numéro de sommet destination invalide.");
        if (!arcExiste(source, destination)) throw std::logic_error("Méthode getPonderationsArc: Arc inexistant.");
        const std::list<Arc>& arcs = listesAdj.at(source);
        for (const auto& arc : arcs) {
            if (arc.destination == destination) {
                return arc.poids;
            }
        }
        throw std::logic_error("Méthode getPonderationsArc: Arc non trouvé.");
    }

    /**
     * \brief Vérifie si un sommet existe
     * \param sommet Numéro du sommet
     * \return true si le sommet existe, false sinon
     */
    bool Graphe::sommetExiste(size_t sommet) const {
        return sommet < noms.size();
    }

    /**
     * \brief Vérifie si un arc existe entre deux sommets
     * \param depart Numéro du sommet de départ
     * \param arrive Numéro du sommet d'arrivée
     * \return true si l'arc existe, false sinon
     */
    bool Graphe::arcExistePourNumero(size_t depart, size_t arrive) const {
        for (const auto& element : listesAdj.at(depart)) {
            if (element.destination == arrive) return true;
        }
        return false;
    }

    /**
     * \brief Renverse le graphe (inverse les arcs)
     * \return Graphe inversé
     */
    Graphe Graphe::inverse() const {
        Graphe inverse(nbSommets);

        for (size_t i = 0; i < nbSommets; ++i) {
            inverse.nommer(i, noms[i]);
        }

        for (size_t source = 0; source < nbSommets; ++source) {
            for (const auto& arc : listesAdj[source]) {
                inverse.ajouterArc(arc.destination, source, arc.poids.duree, arc.poids.cout);
            }
        }

        return inverse;
    }

    /**
     * \brief Ajoute un sommet au graphe
     * \param sommet Numéro du sommet
     */
    void Graphe::ajouterSommet(size_t sommet) {
        noms.push_back(reqNomSommet(sommet));
        listesAdj.emplace_back();
        assert(invariant());
    }

    /**
     * \brief Vérifie l'invariant de la classe
     * \return true si l'invariant est respecté, false sinon
     */
    bool Graphe::invariant() const {
        if (noms.size() != listesAdj.size()) return false;
        std::unordered_set<std::string> copie(noms.begin(), noms.end());
        if (copie.size() != noms.size()) return false;

        for (const auto& liste : listesAdj) {
            for (const auto& sommet : liste) {
                if (sommet.destination >= noms.size()) return false;
            }
        }
        return true;
    }

    /**
     * \brief Explore le graphe en profondeur (DFS)
     * \return Pile des sommets explorés
     */
    std::stack<size_t> Graphe::explorer_dfs() const {
        std::vector<bool> visite(noms.size(), false);
        std::stack<size_t> resultat;
        for (size_t i = 0; i < noms.size(); ++i) {
            if (!visite.at(i)) {
                explorer_dfs_en_partant_de(i, resultat, visite);
            }
        }
        return resultat;
    }

    /**
     * \brief Explore le graphe en profondeur (DFS) à partir d'un sommet donné
     * \param numero Numéro du sommet de départ
     * \param resultat Pile des sommets explorés
     * \param visite Vecteur des sommets visités
     */
    void Graphe::explorer_dfs_en_partant_de(size_t numero, std::stack<size_t> &resultat,
                                            std::vector<bool> &visite) const {
        visite.at(numero) = true;

        for (const auto& voisin : listesAdj.at(numero)) {
            if (!visite.at(voisin.destination)) {
                explorer_dfs_en_partant_de(voisin.destination, resultat, visite);
            }
        }
        resultat.push(numero);
    }

    /**
     * \brief Obtient la taille du graphe
     * \return Taille du graphe
     */
    size_t Graphe::taille() const {
        return noms.size();
    }
/**
 * \brief Obtient le nom d'un sommet.
 * \param sommet Numéro du sommet.
 * \return Le nom du sommet.
 * \throws std::logic_error Si le numéro de sommet est invalide.
 */
    std::string Graphe::reqNomSommet(size_t sommet) const {
        if (sommet >= nbSommets) throw std::logic_error("reqNomSommet: numéro de sommet invalide.");
        return noms[sommet];
    }

}//Fin du namespace

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
    //Ajoutez l'implémentation de vos méthodes ici.

    Graphe::Graphe(size_t nbSommets) : nbSommets(nbSommets), nbArcs() {}

    Graphe::~Graphe() {
    }

    void Graphe::resize(size_t nouvelleTaille) {
        if (nbSommets == 0) {
            nbSommets = nouvelleTaille;
            noms.resize(nouvelleTaille);
            listesAdj.resize(nouvelleTaille);
        }
    }

    void Graphe::nommer(size_t sommet, const std::string &nom) {
        if (sommet >= nbSommets) throw std::logic_error("Méthode nommer: numéro de sommet invalide");
        noms[sommet] = nom;
    }

    void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout) {
        if(!sommetExiste(source)) throw std::logic_error("Méthode ajouterArc: sommet source inexistant.");
        if(!sommetExiste(destination)) throw std::logic_error("Méthode ajouterArc: sommet destination inexistant.");
        if(arcExistePourNumero(source, destination)) throw std::logic_error("Méthode ajouterArc: Arc entre source et destination deja existant.");
        Ponderations poids(duree,cout);
        auto& liste = listesAdj.at(source);
        liste.emplace_back(destination, poids);
        assert(invariant());



    }

    void Graphe::enleverArc(size_t source, size_t destination) {
        if(!sommetExiste(source)) throw std::logic_error("Méthode enleverArc: sommet source inexistant.");
        if(!sommetExiste(destination)) throw std::logic_error("Méthode enleverArc: sommet destination inexistant.");
        if(!arcExiste(source,destination)) throw std::logic_error("Méthode enleverArc: Arc a supprime inexistant.");
        for(auto it = listesAdj.at(source).begin(), end = listesAdj.at(source).end(); it != end; ++it)
        {
            if(it->destination == destination)
            {
                listesAdj.at(source).erase(it);
                return;
                assert(invariant());
            }
        }
    }

    bool Graphe::arcExiste(size_t source, size_t destination) const {
        if (source >= nbSommets) throw std::logic_error("Méthode arcExiste: numéro de sommet source invalide");
        if (destination >= nbSommets) throw std::logic_error("Méthode arcExiste: numéro de sommet destination invalide");
        if(!sommetExiste(source)) throw std::logic_error("Méthode arcExiste: sommet source inexistant");
        if(!sommetExiste(destination)) throw std::logic_error("Méthode arcExiste: sommet destination inexistant");

        const std::list<Arc> &liste = listesAdj[source];
        auto it = std::find_if(liste.begin(), liste.end(), [destination](const Arc &arc) {
            return arc.destination == destination;
        });

        return it != liste.end();
    }

    std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const {
        if(sommet >= nbSommets) throw std::logic_error("Méthode listerSommetAdjacent: numéro de sommet invalide.");
        std::vector<size_t> v;
        for(auto it = listesAdj.at(sommet).begin(); it != listesAdj.at(sommet).end(); ++it)
        {
            v.push_back(it->destination);
        }
        return v;
    }

    std::string Graphe::getNomSommet(size_t sommet) const {
        if(sommet >= nbSommets) throw std::logic_error("reqNomSommet: numéro de sommet invalide.");
        return noms[sommet];

    }

    size_t Graphe::getNumeroSommet(const std::string &nom) const {
        for(size_t i = 0; i < noms.size(); ++i)
        {
            if(nom == noms.at(i)) return i;
        }
        throw std::logic_error("Méthode getNumeroSommet: Nom du sommet introuvable.");

    }

    int Graphe::getNombreSommets() const {
        return nbSommets;
    }

    int Graphe::getNombreArcs() const {
        return nbArcs;
    }

    Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const {
        if (source >= nbSommets) throw std::logic_error("Méthode getPonderationsArc: numéro de sommet source invalide.");
        if (destination >= nbSommets) throw std::logic_error("Méthode getPonderationsArc: numéro de sommet destination invalide.");
        if(!arcExiste(source,destination)) throw std::logic_error("Méthode getPonderationsArc: Arc a supprime inexistant.");
        const std::list<Arc>& arcs = listesAdj.at(source);
        for (const auto& arc : arcs) {
            if (arc.destination == destination) {
                return arc.poids;
            }
        }
    }

     std::string Graphe::reqNomSommet(size_t sommet) const {
         if(sommet >= nbSommets) throw std::logic_error("reqNomSommet: numéro de sommet invalide.");
         return noms[sommet];
    }
    bool Graphe::sommetExiste(size_t sommet) const {
        for(const auto& cle: noms) if(cle == reqNomSommet(sommet)) return true;
        return false;
    }

    bool Graphe::arcExistePourNumero(size_t depart, size_t arrive) const {
        for(auto element : listesAdj.at(depart)) if(element.destination == arrive) return true;
        return false;
    }

    Graphe Graphe::inverse() const {
        Graphe inverse;
        for (auto cle : noms) inverse.
    }

    void Graphe::ajouterSommet(size_t sommet)
    {

    }

    bool Graphe::invariant() const {
        if(noms.size() != listesAdj.size()) return false;
        std::unordered_set<std::string> copie(noms.begin(), noms.end());
        if(copie.size() != noms.size()) return false;

        for(const auto& liste: listesAdj)
        {
            for (auto sommet : liste)
            {
                if(sommet.destination >= noms.size()) return false;
            }
        }
        return true;
    }





}//Fin du namespace

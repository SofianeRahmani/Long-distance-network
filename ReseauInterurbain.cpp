/**
 * \file ReseauInterurbain.cpp
 * \brief Implémentation de la classe ReseauInterurbain.
 * \author IFT-2008 & ?
 * \version 0.1
 * \date Juin-Juillet 2024
 *
 * Travail pratique numéro 2
 *
 */
#include <sstream>
#include <fstream>
#include "ReseauInterurbain.h"
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
    /**
     * \brief Constructeur de la classe ReseauInterurbain
     * \param nomReseau Nom du réseau
     * \param nbVilles Nombre de villes dans le réseau
     */
    ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles) : nomReseau(nomReseau), unReseau(nbVilles) {}

    /**
     * \brief Destructeur de la classe ReseauInterurbain
     */
    ReseauInterurbain::~ReseauInterurbain() {}

    /**
     * \brief Redimensionne le réseau
     * \param nouvelleTaille Nouvelle taille du réseau
     */
    void ReseauInterurbain::resize(size_t nouvelleTaille) {
        if (unReseau.getNombreSommets() == nouvelleTaille) {
            unReseau.resize(nouvelleTaille);
        }
    }

    /**
     * \brief Charge le réseau à partir d'un fichier
     * \param fichierEntree Flux de fichier d'entrée
     * \throws std::logic_error Si le fichier n'est pas ouvert
     */
    void ReseauInterurbain::chargerReseau(std::ifstream & fichierEntree) {
        if (!fichierEntree.is_open())
            throw std::logic_error("ReseauInterurbain::chargerReseau: Le fichier n'est pas ouvert !");

        std::string buff;

        getline(fichierEntree, nomReseau);
        nomReseau.erase(0, 20); // Enlève: Reseau Interurbain:

        int nbVilles;

        fichierEntree >> nbVilles;
        getline(fichierEntree, buff); // villes

        unReseau.resize(nbVilles);

        getline(fichierEntree, buff); // Liste des villes

        size_t i = 0;

        getline(fichierEntree, buff); // Première ville

        while (buff != "Liste des trajets:") {
            unReseau.nommer(i, buff);
            getline(fichierEntree, buff);
            i++;
        }

        while (!fichierEntree.eof()) {
            getline(fichierEntree, buff);
            std::string source = buff;
            getline(fichierEntree, buff);
            std::string destination = buff;

            getline(fichierEntree, buff);
            std::istringstream iss(buff);

            float duree;
            iss >> duree;

            float cout;
            iss >> cout;

            unReseau.ajouterArc(unReseau.getNumeroSommet(source), unReseau.getNumeroSommet(destination), duree, cout);
        }
    }

    /**
     * \brief Recherche un chemin entre deux villes en utilisant l'algorithme de Dijkstra
     * \param origine Ville de départ
     * \param destination Ville d'arrivée
     * \param dureeCout Indicateur pour choisir entre durée et coût
     * \return Le chemin trouvé
     */
    Chemin ReseauInterurbain::rechercheCheminDijkstra(const std::string& origine, const std::string& destination, bool dureeCout) const {
        size_t source = unReseau.getNumeroSommet(origine);
        size_t dest = unReseau.getNumeroSommet(destination);

        std::vector<float> distances(unReseau.getNombreSommets(), std::numeric_limits<float>::max());
        std::vector<float> costs(unReseau.getNombreSommets(), std::numeric_limits<float>::max());
        std::vector<size_t> precedents(unReseau.getNombreSommets(), -1);
        std::priority_queue<std::pair<float, size_t>, std::vector<std::pair<float, size_t>>, std::greater<std::pair<float, size_t>>> queue;

        distances.at(source) = 0.0f;
        costs.at(source) = 0.0f;
        queue.emplace(0.0f, source);

        while (!queue.empty()) {
            size_t u = queue.top().second;
            queue.pop();

            if (u == dest) break;

            for (const auto& arc : unReseau.listerSommetsAdjacents(u)) {
                size_t v = arc;
                Ponderations ponderations = unReseau.getPonderationsArc(u, v);
                float durationThroughU = distances.at(u) + ponderations.duree;
                float costThroughU = costs.at(u) + ponderations.cout;

                if (dureeCout) {
                    if (durationThroughU < distances.at(v)) {
                        distances.at(v) = durationThroughU;
                        costs.at(v) = costThroughU;
                        precedents.at(v) = u;
                        queue.emplace(distances.at(v), v);
                    }
                } else {
                    if (costThroughU < costs.at(v)) {
                        distances.at(v) = durationThroughU;
                        costs.at(v) = costThroughU;
                        precedents.at(v) = u;
                        queue.emplace(costs.at(v), v);
                    }
                }
            }
        }

        Chemin chemin;
        chemin.dureeTotale = distances.at(dest);
        chemin.coutTotal = costs.at(dest);
        chemin.reussi = distances.at(dest) != std::numeric_limits<float>::max();
        if (chemin.reussi) {
            for (size_t at = dest; at != -1; at = precedents.at(at)) {
                chemin.listeVilles.push_back(unReseau.getNomSommet(at));
            }
            std::reverse(chemin.listeVilles.begin(), chemin.listeVilles.end());
        }
        return chemin;
    }

    /**
     * \brief Algorithme de Kosaraju pour trouver les composantes fortement connexes
     * \return Un vecteur de vecteurs de noms de villes représentant les composantes fortement connexes
     */
    std::vector<std::vector<std::string>> ReseauInterurbain::algorithmeKosaraju() {
        std::vector<std::vector<std::string>> composantes;
        std::stack<size_t> file;
        std::vector<bool> visites(unReseau.getNombreSommets(), false);

        for (size_t i = 0; i < unReseau.getNombreSommets(); ++i) {
            if (!visites[i]) {
                ordreEnfilage(i, visites, file);
            }
        }

        Graphe inverse = unReseau.inverse();
        std::fill(visites.begin(), visites.end(), false);

        while (!file.empty()) {
            size_t v = file.top();
            file.pop();

            if (!visites[v]) {
                std::stack<size_t> composante;
                inverse.explorer_dfs_en_partant_de(v, composante, visites);

                std::vector<std::string> comp;
                while (!composante.empty()) {
                    comp.push_back(inverse.getNomSommet(composante.top()));
                    composante.pop();
                }
                composantes.push_back(comp);
            }
        }

        return composantes;
    }

    /**
     * \brief Remplit la pile des sommets dans l'ordre des visites
     * \param v Numéro du sommet
     * \param visite Vecteur des sommets visités
     * \param file Pile des sommets visités
     */
    void ReseauInterurbain::ordreEnfilage(size_t v, std::vector<bool>& visite, std::stack<size_t>& file) const {
        visite[v] = true;

        for (const auto& voisin : unReseau.listerSommetsAdjacents(v)) {
            if (!visite[voisin]) {
                ordreEnfilage(voisin, visite, file);
            }
        }
        file.push(v);
    }

    /**
     * \brief Obtient l'index d'une ville
     * \param ville Nom de la ville
     * \return Index de la ville
     */
    size_t ReseauInterurbain::getVilleIndex(const std::string& ville) const {
        return unReseau.getNumeroSommet(ville);
    }

    /**
     * \brief Obtient le nom d'une ville à partir de son index
     * \param index Index de la ville
     * \return Nom de la ville
     */
    std::string ReseauInterurbain::getVilleName(size_t index) const {
        return unReseau.getNomSommet(index);
    }
    //À compléter au besoin par d'autres méthodes

}//Fin du namespace

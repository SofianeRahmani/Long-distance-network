/**
 * \file ReseauInterurbain.cpp
 * \brief Implémentattion de la classe ReseauInterurbain.
 * \author IFT-2008 & ?
 * \version 0.1
 * \date Juin-Juillet 2024
 *
 *  Travail pratique numéro 2
 *
 */
#include <sstream>
#include <fstream>
#include "ReseauInterurbain.h"
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
    ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles) : nomReseau(nomReseau), unReseau(nbVilles){}

    ReseauInterurbain::~ReseauInterurbain() {

    }

    void ReseauInterurbain::resize(size_t nouvelleTaille){
        if(unReseau.getNombreSommets() == nouvelleTaille)
        {
            unReseau.resize(nouvelleTaille);
        }

    }

    // Méthode fournie
    void ReseauInterurbain::chargerReseau(std::ifstream & fichierEntree)
    {
        if (!fichierEntree.is_open())
            throw std::logic_error("ReseauInterurbain::chargerReseau: Le fichier n'est pas ouvert !");

        std::string buff;

        getline(fichierEntree, nomReseau);
        nomReseau.erase(0, 20); // Enlève: Reseau Interurbain:

        int nbVilles;

        fichierEntree >> nbVilles;
        getline(fichierEntree, buff); //villes

        unReseau.resize(nbVilles);

        getline(fichierEntree, buff); //Liste des villes

        size_t i = 0;

        getline(fichierEntree, buff); //Premiere ville

        while(buff != "Liste des trajets:")
        {
            unReseau.nommer(i, buff);
            getline(fichierEntree, buff);
            i++;
        }

        while(!fichierEntree.eof())
        {
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

    Chemin ReseauInterurbain::rechercheCheminDijkstra(const std::string& source, const std::string& destination, bool dureeCout) const
    {
        size_t src =
    }

    std::vector<std::vector<std::string> > ReseauInterurbain::algorithmeKosaraju()
    {
        std::vector<std::vector<std::string> > composantes;
        return composantes;
    }

    //À compléter au besoin par d'autres méthodes

}//Fin du namespace

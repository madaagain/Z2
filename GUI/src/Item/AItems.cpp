#include "AItems.hpp"

AItems::AItems() {
}

AItems::~AItems() {
}

void AItems::drawWindow(sf::RenderWindow& window)
{
    /** @brief: Cette fonction soccupe de draw la taille de la map sur la window
     * @example:L'affcihage de map
    */
    window.draw(sprite);
}

void AItems::setPosition(float x, float y)
{
    /** @brief: Set la postion de l'item en fonction de ce qui est récupérer en parsing
     * @example: Va set la postion du Player ou des pierres
    */
    sprite.setPosition(x, y);
}


sf::Vector2f AItems::getPosition()
{
    /** @brief fonction qui get la postion des differentes entity
     * @example Un player sur la map ou une stone
    */
    return sprite.getPosition();
}

void AItems::update()
{
    /** @brief Update les evolution du serveur
     * @exemple: Si la position d'un player change cette fonction
     * apelle getposition et setposition la nouvelle position du player
     * et refresh la window
    */
    // Default implementation (if any)
}

void AItems::drawSprite()
{
    /** @brief: Cette fonction s'occupe d'afficher les sprite sur la window
     * @example:L'affcihage d'un Player ou de nourriture
    */
}

#include <raylib.h>

const short int SCREEN_WIDTH = 800;
const short int SCREEN_HEIGHT = 600;

const short int COLUMNS = 4;
const short int ROWS = 4;

const short int CARD_WIDTH = 100;
const short int CARD_HEIGHT = 100;

const short int FLIP_DELAY = 60; // 60 frames = 1 second at 60 FPS

//Card structure
struct 
{
    Rectangle rect;
    bool isFlippled;
    bool isMatched;
    int carId;
}typedef Card;

//Game state variables
Card cards[ROWS * COLUMNS];
int flippedCardIndices[2];
int cardFlipped = 0;
bool canFlip = true;
int numMatches = 0;
int flipDelayCounter = 0;
bool checkForMatch = false;


void ShuffleCards(int* cardIds, int size); //Helper function to shuffle cards
void InitCards(); //Initialize cards with their positions and IDs
void CheckMatch(); //Check if two flipped cards match

///////////////////////////////////////////////////////////////////----------------------------------------------------------------------------////////////////////////////////////////////////////

int main() 
{    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Memory Show");
    SetTargetFPS(60);
    
    InitCards();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BROWN);

        //Handle card flipping logic
        if (canFlip && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePosition = GetMousePosition();
            for (int i = 0; i < ROWS * COLUMNS; i++)
            {
                if(CheckCollisionPointRec(mousePosition, cards[i].rect) && !cards[i].isFlippled && !cards[i].isMatched)
                {
                    cards[i].isFlippled = true;
                    flippedCardIndices[cardFlipped] = i;
                    cardFlipped++;
                    if (cardFlipped == 2)
                    {
                        canFlip = false;
                        checkForMatch = true;
                        flipDelayCounter = 0; //Start the delay counter
                    }
                    break;
                }
            }
        }

        //Delay for flipping back cards after both are flipped
        if(checkForMatch)
        {
            flipDelayCounter++;
            if(flipDelayCounter > FLIP_DELAY)
            {
                CheckMatch(); //Check after delay is over
            }
        }

        //Draw cards
        for (int i = 0; i < ROWS * COLUMNS; i++)
        {
            if(cards[i].isFlippled || cards[i].isMatched)
            {
                DrawRectangleRec(cards[i].rect, BLUE); //Card back (visible side)
                DrawText(TextFormat("%d", cards[i].carId), cards[i].rect.x + 43, cards[i].rect.y + 40, 20, WHITE);
            }
            else
            {
                DrawRectangleRec(cards[i].rect, GRAY); //Card front (hidden)
            }
        }

        if(numMatches == ROWS * COLUMNS / 2)
        {
            DrawText("¡You Win!", 350, 280, 40, DARKGREEN);
        }
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

///////////////////////////////////////////////////////////////////----------------------------------------------------------------------------////////////////////////////////////////////////////

void ShuffleCards(int* cardIds, int size)
{
    for (int i = 0; i < size; i++)
    {
        int randomIndex = GetRandomValue(0, size - 1);
        int temp = cardIds[i];
        cardIds[i] =  cardIds[randomIndex];
        cardIds[randomIndex] = temp;
    }
}

void InitCards()
{
    int cardIds[ROWS * COLUMNS];
    for (int i = 0; i < ROWS * COLUMNS; i++)
    {
        cardIds[i] = i / 2; //Each cardIds appears twice for matching pairs
    }
    ShuffleCards(cardIds, ROWS * COLUMNS);

    int index = 0;
    for (int row = 0; row < ROWS; row++)
    {
        for (int column = 0; column < COLUMNS; column++)
        {   
            cards[index].rect = Rectangle{(float)(column * CARD_WIDTH + 50), (float)(row * CARD_HEIGHT + 50), (float)CARD_WIDTH, (float)CARD_HEIGHT};
            cards[index].isFlippled = false;
            cards[index].isMatched = false;
            cards[index].carId = cardIds[index];
            index++;
        }
    }
}

void CheckMatch()
{
    if (cards[flippedCardIndices[0]].carId == cards[flippedCardIndices[1]].carId)
    {
        cards[flippedCardIndices[0]].isMatched = true;
        cards[flippedCardIndices[1]].isMatched = true;
        numMatches++;
    }
    else
    {
        cards[flippedCardIndices[0]].isFlippled = false;
        cards[flippedCardIndices[1]].isFlippled = false;
    }
    cardFlipped = 0;
    canFlip = true;
    checkForMatch = false;
}
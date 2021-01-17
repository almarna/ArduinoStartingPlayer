#include <LiquidCrystal.h>

#define LCD_WIDTH 16
#define LCD_HEIGHT 2

#define KEY_SELECT 5
#define KEY_LEFT 4
#define KEY_RIGHT 1
#define KEY_UP 2
#define KEY_DOWN 3
#define KEY_NONE 0

#define MAX_FUNCTIONS 3

#define EIGHTBALL_ANSWERS 20

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

const int keyBorders[5] = {60, 200, 400, 600, 800};

char lcdBuffer[LCD_WIDTH + 2];
const char emptyLine[] = "                ";

char playerOrderBuffer[LCD_WIDTH + 2];
int playerOrderPlayerCount = 3;

int getCurrentKey()
{
    int key, i;
    key = analogRead(0);

    for (i = 0; i < 5; i++)
    {
        if (key < keyBorders[i])
        {
            return i + 1;
        }
    }

    return KEY_NONE;
}

int lastKey = KEY_NONE;

int KeyReleased()
{
    int key;
    key = getCurrentKey();

    int result = KEY_NONE;

    if (key != lastKey)
    {
        if (lastKey != KEY_NONE)
        {
            result = lastKey;
        }

        lastKey = key;
    }

    return result;
}

long currentFunction = 1;

long playerCount = 2;

char centeredBuffer[LCD_WIDTH + 2];

const char *centerLcdText(const char *uncentered)
{
    int len = strlen(uncentered);
    if (len > LCD_WIDTH)
    {
        return "  ERROR STRLEN  ";
    }
    strcpy(centeredBuffer, emptyLine);

    int spaceBefore = (LCD_WIDTH - len) / 2;
    int i;
    for (i = 0; i < len; i++)
    {
        centeredBuffer[i + spaceBefore] = uncentered[i];
    }

    return centeredBuffer;
}

void displayLcd(const char *line1, const char *line2)
{
    lcd.setCursor(0, 0);
    lcd.print(centerLcdText(line1));
    lcd.setCursor(0, 1);
    lcd.print(centerLcdText(line2));
}

void displayCurrentFunction()
{
    switch (currentFunction)
    {
    case 1:
        sprintf(lcdBuffer, "%d spelare", playerCount);
        displayLcd("Startspelare", lcdBuffer);
        break;
    case 2:
        sprintf(lcdBuffer, "%d spelare", playerOrderPlayerCount);
        displayLcd("Spelarordning", lcdBuffer);
        break;
    case 3:
        displayLcd("Magic 8-ball", emptyLine);
        break;
    default:
        displayLcd("* Fel funktion *", emptyLine);
        break;
    }
}

void displayPlayerCount(int count)
{
    sprintf(lcdBuffer, "%d spelare", count);
    displayLcd(lcdBuffer, emptyLine);
}

void displayStartingPlayer(long player)
{
    sprintf(lcdBuffer, "   ->> %d <<-   ", player);
    lcd.setCursor(0, 1);
    lcd.print(lcdBuffer);
}

const char *animation[6] = {
    ">             <",
    ">>           <<",
    "->>         <<-",
    " ->>       <<- ",
    "  ->>     <<-  ",
    "   ->>    <<-  "};

void displayAnimation()
{
    int i;
    for (i = 0; i < 6; i++)
    {
        lcd.setCursor(0, 1);
        lcd.print(animation[i]);
        delay(150);
    }
}

const char *playersAnimation[6] = {
    "<<<<<<<<>>>>>>>>",
    "<<<<<<<  >>>>>>>",
    "<<<<<<    >>>>>>",
    "<<<<        >>>>",
    "<<<          >>>",
    "<              >",
};

void displayPlayersAnimation()
{
    int i;
    for (i = 0; i < 6; i++)
    {
        lcd.setCursor(0, 1);
        lcd.print(playersAnimation[i]);
        delay(150);
    }
}

const char *eightballAnimation[] = {
    "*              *",
    "***          ***",
    "*****      *****",
    "*******  *******",
    "****************"};

void displayEightballAnimation()
{
    int i;
    for (i = 0; i < 5; i++)
    {
        displayLcd(eightballAnimation[i], eightballAnimation[i]);
        delay(100);
    }
    for (i = 3; i > -1; i--)
    {
        displayLcd(eightballAnimation[i], eightballAnimation[i]);
        delay(110);
    }
}

void setup()
{
    // randomSeed(analogRead(0));
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    displayCurrentFunction();
}

const char *eightballAnswers[] =
    {
        "It is", "certain",
        "It is", "decidedly so",
        "Without", "a doubt",
        "Yes", "definitely",
        "You may rely", "on it",
        "As I see it,", "yes",
        "Most", "likely",
        " Outlook", "good",
        "Yes", "",
        "Signs point", "to yes",
        "Reply hazy,", "try again",
        "Ask again", "later",
        "Better not", "tell you now",
        "Cannot", "predict now",
        "Concentrate", "and ask again",
        "Don't", "count on it",
        "My reply is", "no",
        "My sources", "say no",
        "Outlook", "not so good",
        "Very", "doubtful"};

bool randomSeeded = false;

long getRandom(int max)
{
    if (!randomSeeded)
    {
        randomSeed(millis());
        randomSeeded = true;
    }

    return random(max);
}

void handleCurrentPlayer(int currentKey)
{
    switch (currentKey)
    {
    case KEY_LEFT:
        if (++playerCount > 9)
        {
            playerCount = 2;
        }
        displayPlayerCount(playerCount);
        break;
    case KEY_RIGHT:
        displayAnimation();
        displayStartingPlayer(getRandom(playerCount) + 1);
        break;
    }
}

const char *getPlayerOrderString(int count)
{
    int randomPlayers[count];

    randomPlayers[0] = getRandom(count);

    int i;
    int j;
    for (i = 1; i < count; i++)
    {
        int randomCandidate;
        boolean isDuplicate;
        do
        {
            randomCandidate = getRandom(count);
            isDuplicate = false;
            for (j = 0; j < i; j++)
            {
                if (randomPlayers[j] == randomCandidate)
                {
                    isDuplicate = true;
                    break;
                }
            }
        } while (isDuplicate);
        randomPlayers[i] = randomCandidate;
    }

    strcpy(playerOrderBuffer, emptyLine);

    for (i = 0; i < count; i++)
    {
        playerOrderBuffer[i * 3] = randomPlayers[i] + '1';
    }

    playerOrderBuffer[(count * 3) - 2] = 0;

    return playerOrderBuffer;
}

void handlePlayerOrder(int currentKey)
{
    switch (currentKey)
    {
    case KEY_LEFT:
        if (++playerOrderPlayerCount > 5)
        {
            playerOrderPlayerCount = 3;
        }
        displayPlayerCount(playerOrderPlayerCount);
        break;
    case KEY_RIGHT:
        displayPlayersAnimation();
        const char *order = getPlayerOrderString(playerOrderPlayerCount);
        lcd.setCursor(0, 1);
        lcd.print(centerLcdText(order));
        break;
    }
}

void handleEightball(int currentKey)
{
    long randomIndex;
    if (currentKey == KEY_RIGHT)
    {
        displayLcd("", "");
        displayEightballAnimation();
        randomIndex = getRandom(EIGHTBALL_ANSWERS) * 2;
        displayLcd(eightballAnswers[randomIndex], eightballAnswers[randomIndex + 1]);
    }
}

void loop()
{
    int currentKey = KeyReleased();
    if (currentKey == KEY_SELECT)
    {
        currentFunction += 1;
        if (currentFunction > MAX_FUNCTIONS)
        {
            currentFunction = 1;
        }
        displayCurrentFunction();
    }
    else
    {
        switch (currentFunction)
        {
        case 1:
            handleCurrentPlayer(currentKey);
            break;
        case 2:
            handlePlayerOrder(currentKey);
            break;
        case 3:
            handleEightball(currentKey);
            break;
        default:
            displayLcd("Function", "not created");
            break;
        }
    }

    delay(100);
}